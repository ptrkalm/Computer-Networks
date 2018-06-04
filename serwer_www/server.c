//Piotr Kałmuczak 292235
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include "sys/stat.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "sockwrap.h"
#include "server.h"


int server(int port, char* catalog){
	int sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server_address;
	bzero (&server_address, sizeof(server_address));
	server_address.sin_family      = AF_INET;
	server_address.sin_port        = htons(port);
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	Bind(sockfd, &server_address, sizeof(server_address));
	char buffer[BUFFSIZE+1];

	Listen(sockfd, 64);

	while (1){
    int conn_sockfd = Accept(sockfd, NULL, NULL);
		struct timeval tv;
		tv.tv_sec = 1; tv.tv_usec = 0;
		fd_set descriptors;
		while(1){
			FD_ZERO(&descriptors);
		  FD_SET(conn_sockfd, &descriptors);
			int ready = Select(conn_sockfd+1, &descriptors, NULL, NULL, &tv);
			if (ready == 0) break;
			tv.tv_sec = 1; tv.tv_usec = 0;

			int msglen = Recv(conn_sockfd, buffer, BUFFSIZE, 0);
			if (msglen == 0) continue;
			buffer[msglen] = 0;

    	//char* code;
    	char host[256];
    	strcpy(host, catalog);
    	int cat_len = strlen(catalog);
    	host[cat_len] = '/';
    	host[cat_len+1] = '\0';
    	char connection[20];
    	char* method;
    	char* host_field;
    	char* conn_field;
    	char path[256];
			char reply_msg[BUFFSIZE];
    	int replen;
    	char* buffcpy = strdup(buffer);
    	method = strtok(buffcpy, "\n");
			int nimp = 0;
    	if (strncmp(method, "GET", 3) != 0)
      	nimp = 1;

    	sscanf(method, "%*s %s", path);

    	host_field = strstr(buffer, "Host:") + 6;
    	conn_field = strstr(buffer, "Connection:") + 12;
			if (host_field == NULL || conn_field == NULL) nimp = 1;

			if (nimp){
				puts("501 Not Implemented");
				sprintf(reply_msg, "HTTP/1.1 501 Not Implemented\nContent-Type: text/html\r\nContent-Length: %ld\r\n\r\n%s", strlen(NOTIMPL), NOTIMPL);
				Send(conn_sockfd, reply_msg, strlen(reply_msg), 0);
				continue;
			}

    	sscanf(host_field, "%s\n", host+cat_len+1);
    	host[strcspn(host, ":")] = '\0';
    	strcpy(host + strlen(host), path);
    	sscanf(conn_field, "%s\n", connection);

			if (strstr(host, "..") != NULL){
				puts("403 Forbidden");
				sprintf(reply_msg, "HTTP/1.1 403 Forbidden\nContent-Type: text/html\r\nContent-Length: %ld\r\n\r\n%s", strlen(FORBIDDEN), FORBIDDEN);
				Send(conn_sockfd, reply_msg, strlen(reply_msg), 0);
				if (strcmp(connection, "close") == 0) break;
				else continue;
			}

    	struct stat hstat = {0};
    	stat(host, &hstat);

			char* extention = strrchr(host, '.') + 1;

    	if(S_ISDIR(hstat.st_mode)){
				puts("301 Moved Permanently");
      	sprintf(reply_msg, "HTTP/1.1 301 Moved Permanently\r\nLocation: /index.html\r\n\r\n");
      	Send(conn_sockfd, reply_msg, strlen(reply_msg), 0);
    	}
    	else{
      	FILE * file = fopen(host, "rb");
      	if (!file){
					puts("404 Not Found");
        	sprintf(reply_msg, "HTTP/1.1 404 Not Found\nContent-Type: text/html\r\nContent-Length: %ld\r\n\r\n%s", strlen(NOTFOUND), NOTFOUND);
        	Send(conn_sockfd, reply_msg, strlen(reply_msg), 0);
      	}
				else {
					printf("200 OK; sending: %s\n", host);
        	stat(host, &hstat);
					if (strcmp(extention, "html") == 0)
        		sprintf(reply_msg, "HTTP/1.1 200 OK\nContent-Type: text/html\r\nContent-Length: %ld\r\n\r\n", hstat.st_size);
					else if (strcmp(extention, "css") == 0)
						sprintf(reply_msg, "HTTP/1.1 200 OK\nContent-Type: text/css\r\nContent-Length: %ld\r\n\r\n", hstat.st_size);
					else if (strcmp(extention, "txt") == 0)
						sprintf(reply_msg, "HTTP/1.1 200 OK\nContent-Type: text/plain; charset=utf-8\r\nContent-Length: %ld\r\n\r\n", hstat.st_size);
					else if (strcmp(extention, "pdf") == 0)
						sprintf(reply_msg, "HTTP/1.1 200 OK\nContent-Type: application/pdf\r\nContent-Length: %ld\r\n\r\n", hstat.st_size);
					else if (strcmp(extention, "jpg") == 0)
						sprintf(reply_msg, "HTTP/1.1 200 OK\nContent-Type: image/jpeg\r\nContent-Length: %ld\r\n\r\n", hstat.st_size);
					else if (strcmp(extention, "jpeg") == 0)
						sprintf(reply_msg, "HTTP/1.1 200 OK\nContent-Type: image/jpeg\r\nContent-Length: %ld\r\n\r\n", hstat.st_size);
					else if (strcmp(extention, "png") == 0)
						sprintf(reply_msg, "HTTP/1.1 200 OK\nContent-Type: image/png\r\nContent-Length: %ld\r\n\r\n", hstat.st_size);
					else
						sprintf(reply_msg, "HTTP/1.1 200 OK\nContent-Type: application/octet-stream\r\nContent-Length: %ld\r\n\r\n", hstat.st_size);
        	replen = strlen(reply_msg);
        	fread(reply_msg+replen, 1, hstat.st_size, file);
					reply_msg[replen + hstat.st_size] = '\0';
        	Send(conn_sockfd, reply_msg, replen + hstat.st_size + 1, 0);
      	}
      	if (file) fclose(file);
			}
			if (strcmp(connection, "close") == 0) break;
    }
		Close(conn_sockfd);
	// Konczymy komunikacje z tym gniazdem, wracamy do obslugi gniazda
	// nasluchujacego.
	}
}
