#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "tftp.h"

int udp_bind_server(int port)

{
    int sock_fd;
    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if ((sock_fd) < 0)
    {
        printf("Error: Could not create socket\n");
        return -1;
    }
    struct sockaddr_in serverinfo;
    serverinfo.sin_family = AF_INET;
    serverinfo.sin_port = htons(port);
    serverinfo.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(sock_fd, (struct sockaddr *)&serverinfo, sizeof(serverinfo)) < 0)
    {
        perror("bind");
        close(sock_fd);
        return -1;
    }

    return sock_fd;
}

int udp_rebind_server(int port)
{
    int sock_fd;
    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if ((sock_fd) < 0)
    {
        printf("Error: Could not create socket\n");
        return -1;
    }
    struct sockaddr_in serverinfo;
    serverinfo.sin_family = AF_INET;
    serverinfo.sin_port = htons(port);
    serverinfo.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(sock_fd, (struct sockaddr *)&serverinfo, sizeof(serverinfo)) < 0)
    {
        perror("bind");
        close(sock_fd);
        return -1;
    }

    return sock_fd;
}
int udp_bind_client(void)
{
    int sock_fd;
    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if ((sock_fd) < 0)
    {
        printf("Error: Could not create socket\n");
        return -1;
    }
    struct sockaddr_in client_info;
    client_info.sin_family = AF_INET;
    client_info.sin_port = htons(0);
    client_info.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sock_fd, (struct sockaddr *)&client_info, sizeof(client_info)) < 0)
    {
        perror("bind");
        close(sock_fd);
        return -1;
    }

    return sock_fd;
}

int udp_send_packet(int sockfd, const void *packet, int packet_size, struct sockaddr_in *dest)
{
    int ret;

    ret = sendto(sockfd, packet, packet_size, 0, (struct sockaddr *)dest, sizeof(struct sockaddr_in));

    if (ret < 0)
    {
        perror("sendto");
        return -1;
    }
    printf("Successfully sent %d bytes\n", ret);
    return ret;
}

int udp_receive_packet(int sockfd, void *packet, int packet_size, struct sockaddr_in *src)
{
    int ret;
    socklen_t addrlen = sizeof(struct sockaddr_in);

    ret = recvfrom(sockfd, packet, packet_size, 0, (struct sockaddr *)src, &addrlen);
    if (ret < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            return TIMEOUT;
        }

        perror("recvfrom");
        return -1;
    }
    printf("Successfully recieved %d bytes\n", ret);
    return ret;
}

int udp_set_receive_timeout(int sockfd, int seconds)
{
    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    if ((setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout))) < 0)
    {
        perror("setsockopt");
        printf("Not able to set timeout option on the socket\n");
        return FAILURE;
    }
    return SUCCESS;
}