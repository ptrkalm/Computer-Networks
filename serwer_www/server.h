int server(int port, char* catalog);
#define NOTFOUND  "<!DOCTYPE html>\n<html>\n<body>\n<h1>404 Not Found</h1><h2>Requested resource does not exist</h2>\n</body>\n</html>\n"
#define FORBIDDEN "<!DOCTYPE html>\n<html>\n<body>\n<h1>403 Forbidden</h1><h2>You don not have permission to access requested resource</h2>\n</body>\n</html>\n"
#define NOTIMPL   "<!DOCTYPE html>\n<html>\n<body>\n<h1>501 Not Implemented</h1><h2>Server can not understand clients request</h2>\n</body>\n</html>\n\r"
#define BUFFSIZE 10000000	// 10 MB
