#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "tftp.h"
#include "file.h"
int file_open_read(const char *filename)
{
    int open_fd = open(filename, O_RDONLY);
    if (open_fd == -1)
    {
        perror("open");
        return FAILURE;
    }
    return open_fd;
}
int file_buffer_from_pos(int fd, char *buffer, int size)
{
    int bytes_rd = read(fd, buffer, size);
    return bytes_rd;
}
int file_close(int fd)
{
    if (close(fd) == -1)
    {
        perror("close");
        return FAILURE;
    }

    return SUCCESS;
}

int file_open_write(const char *filename)
{
    int open_fd = open(filename, O_WRONLY | O_CREAT | O_EXCL | O_APPEND, 0664);
    int new_fd;
    if (open_fd == -1)
    {
        new_fd = open(filename, O_APPEND | O_WRONLY, 0664);
        if (new_fd == -1)
        {
            perror("open");
            return FAILURE;
        }
        return new_fd;
    }
    return open_fd;
}
int file_buffer_to_file(int fd, char *buffer, int size)
{
    int bytes_written = write(fd, buffer, size);
    if (bytes_written == -1)
    {
        perror("write");
        return FAILURE;
    }
    return bytes_written;
}