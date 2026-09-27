

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include "tftp.h"
#include "file.h"

void append_to_packet(packetbuffer_t *buffer, void *piece, int piece_length, int *packet_length)
{
    memcpy(buffer + *packet_length, piece, piece_length);
    *packet_length += piece_length;
}

packetbuffer_t *packet_form_rrq(packet_t *packet, int *packet_length)
{
    packetbuffer_t *buffer = malloc(PACKETSIZE);

    if (buffer == NULL)
        return NULL;

    *packet_length = 0;

    opcode_t opcode = htons(packet->opcode);

    append_to_packet(buffer, &opcode, sizeof(opcode), packet_length);

    append_to_packet(buffer, packet->filename, strlen(packet->filename), packet_length);

    char null_char = '\0';

    append_to_packet(buffer, &null_char, 1, packet_length);

    append_to_packet(buffer, packet->mode, strlen(packet->mode), packet_length);

    append_to_packet(buffer, &null_char, 1, packet_length);

    return buffer;
}

packetbuffer_t *packet_form_wrq(packet_t *packet, int *packet_length)
{
    packetbuffer_t *buffer = malloc(PACKETSIZE);
    if (buffer == NULL)
    {
        return NULL;
    }

    *packet_length = 0;
    opcode_t opcode = htons(packet->opcode);
    append_to_packet(buffer, &opcode, sizeof(opcode), packet_length);

    append_to_packet(buffer, packet->filename, strlen(packet->filename), packet_length);

    char null_char = '\0';
    append_to_packet(buffer, &null_char, 1, packet_length);

    append_to_packet(buffer, packet->mode, strlen(packet->mode), packet_length);

    append_to_packet(buffer, &null_char, 1, packet_length);

    return buffer;
}

packetbuffer_t *packet_form_data(packet_t *packet, int *packet_length)
{
    char *data_pkt = malloc(PACKETSIZE);
    if (data_pkt == NULL)
    {
        printf("MEMORY ALLOCATION FAILURE\n");
        return NULL;
    }
    *packet_length = 0;
    opcode_t opcode = htons(packet->opcode);
    append_to_packet(data_pkt, &opcode, sizeof(opcode), packet_length);

    bnum_t block_num = htons(packet->blocknum);
    append_to_packet(data_pkt, &block_num, sizeof(block_num), packet_length);

    append_to_packet(data_pkt, packet->data, packet->data_length, packet_length);

    return data_pkt;
}

packetbuffer_t *packet_form_ack(packet_t *packet, int *packet_length)
{
    char *ack_pkt = malloc(PACKETSIZE);
    if (ack_pkt == NULL)
    {
        printf("MEMORY ALLOCATION FAILURE\n");
        return NULL;
    }
    *packet_length = 0;
    opcode_t opcode = htons(packet->opcode);
    append_to_packet(ack_pkt, &opcode, sizeof(opcode), packet_length);

    bnum_t blkno = htons(packet->blocknum);
    append_to_packet(ack_pkt, &blkno, sizeof(blkno), packet_length);

    return ack_pkt;
}

packetbuffer_t *packet_form_error(packet_t *packet, int *packet_length)
{
    char *err_pkt = malloc(PACKETSIZE);
    if (err_pkt == NULL)
    {
        printf("MEMORY ALLOCATION FAILURE\n");
        return NULL;
    }
    *packet_length = 0;
    opcode_t opcode = htons(packet->opcode);
    append_to_packet(err_pkt, &opcode, sizeof(opcode), packet_length);

    ecode_t ecode = htons(packet->ecode);
    append_to_packet(err_pkt, &ecode, sizeof(ecode), packet_length);

    append_to_packet(err_pkt, packet->estring, packet->estring_length, packet_length);

    char null_char = '\0';
    append_to_packet(err_pkt, &null_char, 1, packet_length);

    return err_pkt;
}