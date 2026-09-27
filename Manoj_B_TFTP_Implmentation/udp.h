#ifndef TFTP_UDP
#define TFTP_UDP

#include <sys/socket.h>
#include <unistd.h>
#include <sys/time.h>

int udp_bind_server(int port);
int udp_rebind_server(int port);

int udp_bind_client(void);

int udp_send_packet(int sockfd, const void *packet, int packet_size, struct sockaddr_in *dest);
int udp_receive_packet(int sockfd, void *packet, int packet_size, struct sockaddr_in *src);
int udp_set_receive_timeout(int sockfd, int seconds);
#endif
