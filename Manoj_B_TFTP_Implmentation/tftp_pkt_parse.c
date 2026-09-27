#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include "tftp.h"
#include "file.h"

int packet_parse(packetbuffer_t *buffer, int packet_length, packet_t *packet)
{
    if (packet_length < 2 || packet_length > PACKETSIZE)
    {
        printf("Invalid packet size\n");
        return INVALID;
    }
    packet_extract_opcode(buffer, packet);

    switch (packet->opcode)
    {
    case OPCODE_RRQ:
    case OPCODE_WRQ:
        packet_parse_rq(buffer, packet_length, packet);
        break;

    case OPCODE_DATA:
        packet_parse_data(buffer, packet_length, packet);
        break;

    case OPCODE_ACK:
        packet_parse_ack(buffer, packet_length, packet);
        break;

    case OPCODE_ERROR:
        packet_parse_error(buffer, packet_length, packet);
        break;

    default:
        printf("The packet is having an INVALID OPCODE\n");
        return INVALID;
        break;
    }
    return SUCCESS;
}

void packet_extract_opcode(packetbuffer_t *buffer, packet_t *packet)
{

    opcode_t opcode;
    memcpy(&opcode, buffer, 2);
    opcode_t opcode_2 = ntohs(opcode);
    packet->opcode = opcode_2;
}

void packet_parse_rq(packetbuffer_t *buffer, int packet_length, packet_t *packet)
{
    char *start = buffer + 2;
    char *end = buffer + packet_length;
    char *temp = start;

    while (temp < end && *temp != '\0')
        temp++;
    if (temp == end)
    {
        return;
    }
    memcpy(packet->filename, start, temp - start);
    packet->filename[temp - start] = '\0';
    start = temp + 1;
    temp = start;

    while (temp < end && *temp != '\0')
        temp++;
    if (temp == end)
    {
        return;
    }

    memcpy(packet->mode, start, temp - start);
    packet->mode[temp - start] = '\0';
}

void packet_parse_data(packetbuffer_t *buffer, int packet_length, packet_t *packet)
{
    bnum_t blk_num;
    memcpy(&blk_num, buffer + 2, 2);
    packet->blocknum = ntohs(blk_num);

    char *start = buffer + 4;
    int data_len = packet_length - 4;

    memcpy(packet->data, start, data_len);

    packet->data_length = data_len;
}

void packet_parse_ack(packetbuffer_t *buffer, int packet_length, packet_t *packet)
{
    bnum_t blk_num;
    memcpy(&blk_num, buffer + 2, 2);
    bnum_t blk_num2 = ntohs(blk_num);
    packet->blocknum = blk_num2;
}

void packet_parse_error(packetbuffer_t *buffer, int packet_length,
                        packet_t *packet)
{
    if (packet_length < 5)
    {
        return;
    }

    ecode_t errcode;

    memcpy(&errcode, buffer + 2, 2);
    packet->ecode = ntohs(errcode);

    char *start = buffer + 4;
    char *end = buffer + packet_length;
    char *copy = start;

    while (copy < end && *copy != '\0')
    {
        copy++;
    }

    if (copy == end)
    {
        packet->estring_length = 0;
        packet->estring[0] = '\0';
        return;
    }

    packet->estring_length = copy - start;

    memcpy(packet->estring,
           start,
           packet->estring_length);

    packet->estring[packet->estring_length] = '\0';
}