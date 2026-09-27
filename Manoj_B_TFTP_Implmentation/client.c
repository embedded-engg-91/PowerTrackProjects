#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "udp.h"
#include "tftp.h"
#include "file.h"

#define MAX_DUPLICATES 5

int handle_error_packet(packet_t *packet)
{
    if (IS_ERROR(packet->opcode))
    {
        printf("TFTP ERROR %d: %s\n", packet->ecode, packet->estring);
        return FAILURE;
    }
    return SUCCESS;
}

int main()
{
    char input[100];
    char *command;
    char *argument;
    struct sockaddr_in server;

    int sock_fd;
    int connected = 0;

    sock_fd = udp_bind_client();
    if (sock_fd == -1)
    {
        return FAILURE;
    }
    if (udp_set_receive_timeout(sock_fd, 3) == FAILURE)
    {
        close(sock_fd);
        return FAILURE;
    }

    while (1)
    {
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }

        input[strcspn(input, "\r\n")] = '\0';

        command = strtok(input, " \t");
        argument = strtok(NULL, " \t");
        if (command == NULL)
            continue;

        if (strcmp(command, "connect") == 0)
        {
            if (argument == NULL)
            {
                printf("Please enter a server IP address.\n");
                continue;
            }
            server.sin_family = AF_INET;
            server.sin_port = htons(20000);
            if (inet_pton(AF_INET, argument, &server.sin_addr) != 1)
            {
                printf("Invalid IP Address.\n");
                printf("Please try again\n");
                continue;
            }
            printf("Connected to Server %s\n", argument);
            connected = 1;
        }
        else if (strcmp(command, "get") == 0)
        {
            if (!connected)
            {
                printf("Please connect to a server first.\n");
                continue;
            }
            if (argument == NULL)
            {
                printf("Please enter a filename.\n");
                continue;
            }

            int file_fd = file_open_write(argument);
            if (file_fd == FAILURE)
            {
                printf("Failure to open the local file.\n");
                continue;
            }

            packet_t packet;
            packet.opcode = OPCODE_RRQ;
            strcpy(packet.filename, argument);
            strcpy(packet.mode, "octet");

            int packet_len = 0;
            char *buffer_pkt = packet_form_rrq(&packet, &packet_len);
            if (buffer_pkt == NULL)
            {
                printf("Failure to form an RRQ packet\n");
                file_close(file_fd);
                continue;
            }

            char data_buffer[PACKETSIZE];
            struct sockaddr_in src;
            struct sockaddr_in transfer_dst;
            int bytes_rx = 0;
            int retry_count = 0;
            int rrq_success = 0;

            while (retry_count < MAX_RETRIES)
            {
                if (udp_send_packet(sock_fd, buffer_pkt, packet_len, &server) < 0)
                {
                    printf("Failed to send RRQ.\n");
                    break;
                }

                printf("Waiting for DATA...\n");

                bytes_rx = udp_receive_packet(sock_fd, data_buffer, PACKETSIZE, &src);

                if (bytes_rx == TIMEOUT)
                {
                    retry_count++;
                    printf("Timeout. Retransmitting RRQ... Attempt %d/%d\n", retry_count, MAX_RETRIES);
                    continue;
                }

                if (bytes_rx < 0)
                {
                    printf("Failed to receive DATA\n");
                    break;
                }

                packet_t first_pkt;

                if (packet_parse(data_buffer, bytes_rx, &first_pkt) == INVALID)
                {
                    printf("Invalid packet received from server.\n");
                    break;
                }

                if (handle_error_packet(&first_pkt) == FAILURE)
                {
                    break;
                }

                if (first_pkt.opcode != OPCODE_DATA)
                {
                    printf("Expected DATA packet.\n");
                    break;
                }

                transfer_dst = src;
                rrq_success = 1;
                break;
            }

            free(buffer_pkt);
            buffer_pkt = NULL;

            if (!rrq_success)
            {
                printf("RRQ failed or maximum retries reached.\n");
                file_close(file_fd);
                continue;
            }

            int expected_block = 1;
            int transfer_failed = 0;
            int duplicate_count = 0;

            while (1)
            {
                packet_t data_pkt;

                if (bytes_rx == TIMEOUT)
                {
                    printf("Timeout waiting for block %d.\n", expected_block);
                    transfer_failed = 1;
                    break;
                }
                if (bytes_rx < 0)
                {
                    printf("Failed to receive block %d.\n", expected_block);
                    transfer_failed = 1;
                    break;
                }

                if (src.sin_port != transfer_dst.sin_port || src.sin_addr.s_addr != transfer_dst.sin_addr.s_addr)
                {
                    printf("Ignoring packet from unknown TID.\n");
                    bytes_rx = udp_receive_packet(sock_fd, data_buffer, PACKETSIZE, &src);
                    continue;
                }

                if (packet_parse(data_buffer, bytes_rx, &data_pkt) == INVALID)
                {
                    printf("Invalid Data Packet Received\n");
                    transfer_failed = 1;
                    break;
                }

                if (handle_error_packet(&data_pkt) == FAILURE)
                {
                    transfer_failed = 1;
                    break;
                }

                if (data_pkt.opcode != OPCODE_DATA)
                {
                    printf("Expecting a DATA packet\n");
                    transfer_failed = 1;
                    break;
                }

                if (data_pkt.blocknum == expected_block - 1)
                {
                    if (++duplicate_count > MAX_DUPLICATES)
                    {
                        printf("Too many duplicate DATA blocks, aborting.\n");
                        transfer_failed = 1;
                        break;
                    }

                    printf("Duplicate DATA block %d received. Resending ACK...\n", data_pkt.blocknum);
                    packet_t ack_pkt;
                    ack_pkt.opcode = OPCODE_ACK;
                    ack_pkt.blocknum = data_pkt.blocknum;

                    int ack_len = 0;
                    char *ack_buffer = packet_form_ack(&ack_pkt, &ack_len);
                    if (ack_buffer)
                    {
                        udp_send_packet(sock_fd, ack_buffer, ack_len, &transfer_dst);
                        free(ack_buffer);
                    }
                }
                else if (data_pkt.blocknum != expected_block)
                {
                    printf("Unexpected DATA block: %d, expected: %d\n", data_pkt.blocknum, expected_block);
                    transfer_failed = 1;
                    break;
                }
                else
                {
                    duplicate_count = 0;

                    if (file_buffer_to_file(file_fd, data_pkt.data, data_pkt.data_length) == FAILURE)
                    {
                        printf("Failure to write data to file.\n");
                        transfer_failed = 1;
                        break;
                    }

                    packet_t ack_pkt;
                    ack_pkt.opcode = OPCODE_ACK;
                    ack_pkt.blocknum = data_pkt.blocknum;

                    int ack_len = 0;
                    char *ack_buffer = packet_form_ack(&ack_pkt, &ack_len);
                    if (ack_buffer == NULL)
                    {
                        printf("Failure to form ACK packet.\n");
                        transfer_failed = 1;
                        break;
                    }

                    udp_send_packet(sock_fd, ack_buffer, ack_len, &transfer_dst);
                    free(ack_buffer);

                    if (data_pkt.data_length < 512)
                    {
                        break;
                    }

                    expected_block++;
                }

                bytes_rx = udp_receive_packet(sock_fd, data_buffer, PACKETSIZE, &src);
            }

            file_close(file_fd);

            if (transfer_failed)
            {
                printf("GET failed.\n");
            }
            else
            {
                printf("GET successful.\n");
            }
        }
        else if (strcmp(command, "put") == 0)
        {
            if (!connected)
            {
                printf("Please connect to a server first.\n");
                continue;
            }

            if (argument == NULL)
            {
                printf("Please enter a filename.\n");
                continue;
            }

            int file_fd = file_open_read(argument);
            if (file_fd == FAILURE)
            {
                printf("Failure to open the local file.\n");
                continue;
            }

            packet_t wrq_pkt;
            wrq_pkt.opcode = OPCODE_WRQ;
            strcpy(wrq_pkt.filename, argument);
            strcpy(wrq_pkt.mode, "octet");

            int wrq_len = 0;
            char *wrq_buffer = packet_form_wrq(&wrq_pkt, &wrq_len);

            if (wrq_buffer == NULL)
            {
                printf("Failure to form WRQ packet.\n");
                file_close(file_fd);
                continue;
            }

            char ack_buffer[PACKETSIZE];
            struct sockaddr_in src;
            struct sockaddr_in transfer_dst;
            int bytes_rx;
            int wrq_success = 0;
            int retry_count = 0;
            packet_t ack_pkt;

            while (retry_count < MAX_RETRIES)
            {
                if (udp_send_packet(sock_fd, wrq_buffer, wrq_len, &server) < 0)
                {
                    printf("Failed to send WRQ.\n");
                    break;
                }

                printf("Waiting for ACK 0...\n");

                bytes_rx = udp_receive_packet(sock_fd, ack_buffer, PACKETSIZE, &src);

                if (bytes_rx == TIMEOUT)
                {
                    retry_count++;
                    printf("Timeout. Retransmitting WRQ... Attempt %d/%d\n", retry_count, MAX_RETRIES);
                    continue;
                }

                if (bytes_rx < 0)
                {
                    printf("Failed to receive ACK 0.\n");
                    break;
                }

                if (packet_parse(ack_buffer, bytes_rx, &ack_pkt) == INVALID)
                {
                    printf("Invalid packet received.\n");
                    continue;
                }

                if (handle_error_packet(&ack_pkt) == FAILURE)
                {
                    break;
                }

                if (ack_pkt.opcode != OPCODE_ACK)
                {
                    printf("Expected ACK packet.\n");
                    continue;
                }

                if (ack_pkt.blocknum != 0)
                {
                    printf("Unexpected ACK %d. Expected ACK 0.\n", ack_pkt.blocknum);
                    continue;
                }

                printf("ACK 0 received.\n");

                transfer_dst = src;
                wrq_success = 1;
                break;
            }

            free(wrq_buffer);
            wrq_buffer = NULL;

            if (!wrq_success)
            {
                printf("WRQ failed.\n");
                file_close(file_fd);
                continue;
            }

            int block_num = 1;
            char data_buffer[512];
            int transfer_failed = 0;

            while (1)
            {
                int bytes_read = file_buffer_from_pos(file_fd, data_buffer, 512);

                if (bytes_read < 0)
                {
                    printf("Failed to read file.\n");
                    transfer_failed = 1;
                    break;
                }

                packet_t data_pkt;
                data_pkt.opcode = OPCODE_DATA;
                data_pkt.blocknum = block_num;
                data_pkt.data_length = bytes_read;

                memcpy(data_pkt.data, data_buffer, bytes_read);

                int data_len = 0;
                char *data_packet = packet_form_data(&data_pkt, &data_len);

                if (data_packet == NULL)
                {
                    printf("Failure to form DATA packet.\n");
                    transfer_failed = 1;
                    break;
                }

                int ack_received = 0;
                retry_count = 0;

                while (retry_count < MAX_RETRIES)
                {
                    if (udp_send_packet(sock_fd, data_packet, data_len, &transfer_dst) < 0)
                    {
                        printf("Failed to send DATA %d.\n", block_num);
                        break;
                    }

                    printf("Waiting for ACK %d...\n", block_num);

                    bytes_rx = udp_receive_packet(sock_fd, ack_buffer, PACKETSIZE, &src);

                    if (bytes_rx == TIMEOUT)
                    {
                        retry_count++;
                        printf("Timeout. Retransmitting DATA %d... Attempt %d/%d\n", block_num, retry_count, MAX_RETRIES);
                        continue;
                    }

                    if (bytes_rx < 0)
                    {
                        printf("Failed to receive ACK.\n");
                        break;
                    }

                    if (src.sin_port != transfer_dst.sin_port || src.sin_addr.s_addr != transfer_dst.sin_addr.s_addr)
                    {
                        printf("Ignoring packet from unknown TID.\n");
                        continue;
                    }

                    if (packet_parse(ack_buffer, bytes_rx, &ack_pkt) == INVALID)
                    {
                        printf("Invalid ACK packet received.\n");
                        continue;
                    }

                    if (handle_error_packet(&ack_pkt) == FAILURE)
                    {
                        transfer_failed = 1;
                        break;
                    }

                    if (ack_pkt.opcode != OPCODE_ACK)
                    {
                        printf("Expected ACK packet.\n");
                        continue;
                    }

                    if (ack_pkt.blocknum != block_num)
                    {
                        printf("Unexpected ACK %d. Expected ACK %d.\n", ack_pkt.blocknum, block_num);
                        continue;
                    }

                    printf("ACK %d received.\n", block_num);
                    ack_received = 1;
                    break;
                }

                free(data_packet);
                data_packet = NULL;

                if (!ack_received || transfer_failed)
                {
                    printf("Failed to transfer DATA %d.\n", block_num);
                    transfer_failed = 1;
                    break;
                }

                if (bytes_read < 512)
                {
                    printf("File transfer completed.\n");
                    break;
                }

                block_num++;
            }

            file_close(file_fd);

            if (transfer_failed)
            {
                printf("PUT failed.\n");
            }
            else
            {
                printf("PUT successful.\n");
            }
        }
        else if ((strcmp(command, "quit") == 0) || (strcmp(command, "bye") == 0))
        {
            break;
        }
        else
        {
            printf("Invalid Command entered.\n");
            printf("Please try again\n");
        }
    }

    close(sock_fd);
    return 0;
}