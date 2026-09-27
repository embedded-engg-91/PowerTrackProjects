#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#include "file.h"
#include "udp.h"
#include "tftp.h"

#define MAX_RETRIES 3

int NEW_SERVER_PORT = 20001;

int send_error_packet(int sock_fd,
                      struct sockaddr_in *dest,
                      ecode_t error_code,
                      const char *error_message)
{
    packet_t error_pkt;

    memset(&error_pkt, 0, sizeof(packet_t));

    error_pkt.opcode = OPCODE_ERROR;
    error_pkt.ecode = error_code;

    strncpy(error_pkt.estring,
            error_message,
            PACKETSIZE - 1);

    error_pkt.estring[PACKETSIZE - 1] = '\0';

    error_pkt.estring_length = strlen(error_pkt.estring);

    int error_length = 0;

    char *error_buffer =
        packet_form_error(&error_pkt, &error_length);

    if (error_buffer == NULL)
    {
        printf("Failed to form ERROR packet.\n");
        return FAILURE;
    }

    if (udp_send_packet(sock_fd,
                        error_buffer,
                        error_length,
                        dest) < 0)
    {
        printf("Failed to send ERROR packet.\n");

        free(error_buffer);

        return FAILURE;
    }

    free(error_buffer);

    return SUCCESS;
}

int main()
{

    signal(SIGCHLD, SIG_IGN);

    char *buffer = malloc(PACKETSIZE);

    if (buffer == NULL)
    {
        printf("Memory allocation failed\n");
        return FAILURE;
    }

    pid_t pid;

    int sock_fd;
    int bytes_rx;
    int new_port;

    struct sockaddr_in src;
    struct sockaddr_in client_addr;

    packet_t packet;

    sock_fd = udp_bind_server(20000);

    if (sock_fd < 0)
    {
        printf("Failed to bind server socket. Exiting.\n");

        free(buffer);

        return FAILURE;
    }

    while (1)
    {

        bytes_rx = udp_receive_packet(sock_fd,
                                      buffer,
                                      PACKETSIZE,
                                      &src);

        if (bytes_rx < 0)
        {
            printf("Receive failed\n");
            continue;
        }

        if (packet_parse(buffer,
                         bytes_rx,
                         &packet) == INVALID)
        {
            printf("Invalid TFTP packet\n");
            continue;
        }

        client_addr = src;

        new_port = NEW_SERVER_PORT++;

        pid = fork();

        if (pid < 0)
        {
            perror("fork");
            continue;
        }

        else if (pid == 0)
        {
            int child_sock_fd;

            close(sock_fd);

            free(buffer);

            child_sock_fd = udp_rebind_server(new_port);

#ifdef DEBUG
            printf("Child socket = %d\n", child_sock_fd);
#endif

            if (child_sock_fd == -1)
            {
                exit(EXIT_FAILURE);
            }

            udp_set_receive_timeout(child_sock_fd, 5);

            if (IS_RRQ(packet.opcode))
            {
                printf("Child received RRQ for file: %s\n",
                       packet.filename);

                int file_fd =
                    file_open_read(packet.filename);

                if (file_fd == FAILURE)
                {
                    printf("Failure to open file for reading!\n");

                    send_error_packet(child_sock_fd,
                                      &client_addr,
                                      ECODE_1,
                                      ESTRING_1);

                    close(child_sock_fd);

                    exit(EXIT_FAILURE);
                }

                int block_num = 1;
                int bytes_rd = 0;

                int is_last_block = 0;
                int retries = 0;

                bytes_rd =
                    file_buffer_from_pos(file_fd,
                                         packet.data,
                                         512);

                if (bytes_rd < 512)
                {
                    is_last_block = 1;
                }

                packet.opcode = OPCODE_DATA;
                packet.blocknum = block_num;
                packet.data_length = bytes_rd;

                int packet_length = 0;

                char *buffer_packet =
                    packet_form_data(&packet,
                                     &packet_length);

                if (buffer_packet == NULL)
                {
                    printf("Failed to form DATA packet.\n");

                    file_close(file_fd);
                    close(child_sock_fd);

                    exit(EXIT_FAILURE);
                }

#ifdef DEBUG
                printf("Sending DATA block %d (%d bytes)\n",
                       block_num,
                       bytes_rd);
#endif

                udp_send_packet(child_sock_fd,
                                buffer_packet,
                                packet_length,
                                &client_addr);

                while (1)
                {
                    char ack_buffer[PACKETSIZE];

                    packet_t ack_packet;

                    bytes_rx =
                        udp_receive_packet(child_sock_fd,
                                           ack_buffer,
                                           PACKETSIZE,
                                           &src);

                    if (bytes_rx == TIMEOUT)
                    {
                        retries++;

                        if (retries >= MAX_RETRIES)
                        {
                            printf("Client timeout waiting for "
                                   "ACK %d. Aborting.\n",
                                   block_num);

                            break;
                        }

                        printf("Timeout for ACK %d. "
                               "Retransmitting (%d/%d)...\n",
                               block_num,
                               retries,
                               MAX_RETRIES);

                        udp_send_packet(child_sock_fd,
                                        buffer_packet,
                                        packet_length,
                                        &client_addr);

                        continue;
                    }

                    if (bytes_rx < 0)
                    {
                        printf("Receive failed\n");
                        break;
                    }

                    if (packet_parse(ack_buffer,
                                     bytes_rx,
                                     &ack_packet) == INVALID)
                    {
                        continue;
                    }

                    if (ack_packet.opcode == OPCODE_ERROR)
                    {
                        printf("Client sent ERROR %d: %s\n",
                               ack_packet.ecode,
                               ack_packet.estring);

                        break;
                    }

                    if (ack_packet.opcode != OPCODE_ACK)
                    {
                        continue;
                    }

                    if (ack_packet.blocknum < block_num)
                    {
                        udp_send_packet(child_sock_fd,
                                        buffer_packet,
                                        packet_length,
                                        &client_addr);

                        continue;
                    }

                    if (ack_packet.blocknum == block_num)
                    {
                        free(buffer_packet);

                        buffer_packet = NULL;

                        if (is_last_block)
                        {
                            printf("RRQ file transfer complete.\n");

                            break;
                        }

                        block_num++;

                        retries = 0;

                        bytes_rd =
                            file_buffer_from_pos(file_fd,
                                                 packet.data,
                                                 512);

                        if (bytes_rd < 512)
                        {
                            is_last_block = 1;
                        }

                        packet.opcode = OPCODE_DATA;
                        packet.blocknum = block_num;
                        packet.data_length = bytes_rd;

                        packet_length = 0;

                        buffer_packet =
                            packet_form_data(&packet,
                                             &packet_length);

                        if (buffer_packet == NULL)
                        {
                            printf("Failed to form DATA packet.\n");
                            break;
                        }

#ifdef DEBUG
                        printf("Sending DATA block %d (%d bytes)\n",
                               block_num,
                               bytes_rd);
#endif

                        udp_send_packet(child_sock_fd,
                                        buffer_packet,
                                        packet_length,
                                        &client_addr);
                    }
                }

                if (buffer_packet != NULL)
                {
                    free(buffer_packet);
                }

                file_close(file_fd);

                close(child_sock_fd);

                exit(EXIT_SUCCESS);
            }

            else if (IS_WRQ(packet.opcode))
            {
                int block_num = 1;

                int file_fd =
                    file_open_write(packet.filename);

                if (file_fd == FAILURE)
                {
                    printf("Failure to open file for writing!\n");

                    send_error_packet(child_sock_fd,
                                      &client_addr,
                                      ECODE_2,
                                      ESTRING_2);

                    close(child_sock_fd);

                    exit(EXIT_FAILURE);
                }

                packet_t ack;

                memset(&ack, 0, sizeof(packet_t));

                ack.opcode = OPCODE_ACK;
                ack.blocknum = 0;

                int packet_length = 0;

                char *buffer_packet =
                    packet_form_ack(&ack,
                                    &packet_length);

                if (buffer_packet == NULL)
                {
                    printf("Failed to form ACK packet.\n");

                    file_close(file_fd);
                    close(child_sock_fd);

                    exit(EXIT_FAILURE);
                }

                udp_send_packet(child_sock_fd,
                                buffer_packet,
                                packet_length,
                                &client_addr);

                int retries = 0;

                while (1)
                {
                    char data_buffer[PACKETSIZE];

                    packet_t data_packet;

                    bytes_rx =
                        udp_receive_packet(child_sock_fd,
                                           data_buffer,
                                           PACKETSIZE,
                                           &src);

                    if (bytes_rx == TIMEOUT)
                    {
                        retries++;

                        if (retries >= MAX_RETRIES)
                        {
                            printf("Client timeout during WRQ. "
                                   "Terminating transfer.\n");

                            break;
                        }

                        udp_send_packet(child_sock_fd,
                                        buffer_packet,
                                        packet_length,
                                        &client_addr);

                        continue;
                    }

                    if (bytes_rx < 0)
                    {
                        printf("Receive failed\n");
                        break;
                    }

                    if (packet_parse(data_buffer,
                                     bytes_rx,
                                     &data_packet) == INVALID)
                    {
                        continue;
                    }

                    if (data_packet.opcode == OPCODE_ERROR)
                    {
                        printf("Client sent ERROR %d: %s\n",
                               data_packet.ecode,
                               data_packet.estring);

                        break;
                    }

                    if (data_packet.opcode != OPCODE_DATA)
                    {
                        continue;
                    }

                    if (data_packet.blocknum < block_num)
                    {
                        udp_send_packet(child_sock_fd,
                                        buffer_packet,
                                        packet_length,
                                        &client_addr);

                        continue;
                    }

                    if (data_packet.blocknum > block_num)
                    {
                        continue;
                    }

                    retries = 0;

                    if (file_buffer_to_file(file_fd,
                                            data_packet.data,
                                            data_packet.data_length) == FAILURE)
                    {
                        printf("Failure to write data to file.\n");

                        send_error_packet(child_sock_fd,
                                          &client_addr,
                                          ECODE_3,
                                          ESTRING_3);

                        break;
                    }

                    packet_t ack_resp;

                    memset(&ack_resp,
                           0,
                           sizeof(packet_t));

                    ack_resp.opcode = OPCODE_ACK;

                    ack_resp.blocknum =
                        data_packet.blocknum;

                    int ack_length = 0;

                    char *ack_buffer =
                        packet_form_ack(&ack_resp,
                                        &ack_length);

                    if (ack_buffer == NULL)
                    {
                        printf("Failed to form ACK packet.\n");
                        break;
                    }

                    udp_send_packet(child_sock_fd,
                                    ack_buffer,
                                    ack_length,
                                    &client_addr);

                    free(buffer_packet);

                    buffer_packet = ack_buffer;

                    packet_length = ack_length;

                    if (data_packet.data_length < 512)
                    {
                        printf("WRQ file transfer complete.\n");
                        break;
                    }

                    block_num++;
                }

                free(buffer_packet);

                file_close(file_fd);

                close(child_sock_fd);

                exit(EXIT_SUCCESS);
            }

            else
            {
                printf("Unsupported TFTP request opcode: %d\n",
                       packet.opcode);

                send_error_packet(child_sock_fd,
                                  &client_addr,
                                  ECODE_4,
                                  ESTRING_4);

                close(child_sock_fd);

                exit(EXIT_FAILURE);
            }
        }

        else
        {
        }
    }

    free(buffer);

    close(sock_fd);

    return 0;
}
