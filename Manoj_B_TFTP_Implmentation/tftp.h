
#ifndef TFTP
#define TFTP

#define BLIMIT 512
#define PACKETSIZE 516

#define MAX_RETRIES 3

typedef unsigned short opcode_t;
#define OPCODE_RRQ 1
#define IS_RRQ(op) ((op) == OPCODE_RRQ)
#define OPCODE_WRQ 2
#define IS_WRQ(op) ((op) == OPCODE_WRQ)
#define OPCODE_DATA 3
#define IS_DATA(op) ((op) == OPCODE_DATA)
#define OPCODE_ACK 4
#define IS_ACK(op) ((op) == OPCODE_ACK)
#define OPCODE_ERROR 5
#define IS_ERROR(op) ((op) == OPCODE_ERROR)

typedef unsigned short ecode_t;
#define ECODE_NONE 8
#define ECODE_0 0
#define IS_ECODE_0(ec) ((ec) == ECODE_0)
#define ESTRING_0 "Not defined, see error message(if any)."
#define ECODE_1 1
#define IS_ECODE_1(ec) ((ec) == ECODE_1)
#define ESTRING_1 "File not found."
#define ECODE_2 2
#define IS_ECODE_2(ec) ((ec) == ECODE_2)
#define ESTRING_2 "Access violation."
#define ECODE_3 3
#define IS_ECODE_3(ec) ((ec) == ECODE_3)
#define ESTRING_3 "Disk full or allocation exceeded."
#define ECODE_4 4
#define IS_ECODE_4(ec) ((ec) == ECODE_4)
#define ESTRING_4 "Illegal TFTP operation."
#define ECODE_5 5
#define IS_ECODE_5(ec) ((ec) == ECODE_5)
#define ESTRING_5 "Unknown transfer ID."
#define ECODE_6 6
#define IS_ECODE_6(ec) ((ec) == ECODE_6)
#define ECODE_7 7
#define IS_ECODE_7(ec) ((ec) == ECODE_7)
#define ESTRING_7 "No such user."

#define MODE_NETASCII "netascii"
#define MODE_OCTET "octet"
#define MODE_MAIL "mail"

#define TIMEOUT 1
#define TIMEOUT_LIMIT 10

typedef unsigned short bnum_t;
typedef char packetbuffer_t;

extern int NEW_SERVER_PORT;
typedef enum status
{
    SUCCESS,
    FAILURE,
    INVALID
} Status;

typedef struct tftp_packet
{
    char filename[PACKETSIZE];
    opcode_t opcode;
    char mode[PACKETSIZE];
    char data[BLIMIT];
    int data_length;
    bnum_t blocknum;
    ecode_t ecode;
    char estring[PACKETSIZE];
    int estring_length;
} packet_t;

int packet_parse(packetbuffer_t *buffer, int packet_length,
                 packet_t *packet);
void packet_extract_opcode(packetbuffer_t *buffer, packet_t *packet);

void packet_parse_rq(packetbuffer_t *buffer, int packet_length,
                     packet_t *packet);

void packet_parse_data(packetbuffer_t *buffer, int packet_length,
                       packet_t *packet);

void packet_parse_ack(packetbuffer_t *buffer, int packet_length,
                      packet_t *packet);

void packet_parse_error(packetbuffer_t *buffer, int packet_length,
                        packet_t *packet);

packetbuffer_t *packet_form_rrq(packet_t *packet, int *packet_length);
packetbuffer_t *packet_form_wrq(packet_t *packet, int *packet_length);
packetbuffer_t *packet_form_data(packet_t *packet, int *packet_length);
packetbuffer_t *packet_form_ack(packet_t *packet, int *packet_length);
packetbuffer_t *packet_form_error(packet_t *packet, int *packet_length);

void packet_free(packetbuffer_t *buffer);
void append_to_packet(packetbuffer_t *buffer, void *piece, int piece_length, int *packet_length);

int packet_receive_rrq();
int packet_receive_wrq();
int packet_receive_data();
int packet_receive_ack();
int packet_receive_error();
int packet_receive_invalid();

#endif
