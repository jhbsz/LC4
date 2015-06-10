
int open_tcp_server(unsigned short port);
int wait_for_connection_serv(int listenfd);
int close_connection_serv(int connfd);
int close_tcp_server(int listenfd);

