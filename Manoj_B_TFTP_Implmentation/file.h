#ifndef TFTP_FILE
#define TFTP_FILE
#include "tftp.h"


int file_open_read(const char *filename);
int file_buffer_from_pos(int fd, char *buffer, int size);
int file_close(int fd);
int file_open_write(const char *filename);
int file_buffer_to_file(int fd, char *buffer, int size);

#endif
