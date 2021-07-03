#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int linux_write( int fd, const void* buffer, size_t bytes )
{
    ssize_t bytes_written = 0;
    ssize_t total_bytes_written = 0;

    while( total_bytes_written < bytes )
    {
        bytes_written = write( fd, buffer + total_bytes_written, bytes - (size_t)total_bytes_written );
        total_bytes_written += bytes_written;

        if( bytes_written < 0 )
        {
            return 1;
        }
    }

    return 0;
}