#ifndef CONNECTION_H
#define CONNECTION_H

int connect_ap(int sock, const char *ip, unsigned short port);
int create_sock(void);

#endif