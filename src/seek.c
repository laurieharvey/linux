#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lib.h"

int main( int argc, char *argv[] )
{
    if( argc < 3 )
    {
        return 1;
    }

    const char *pathname = argv[1];

    const int open_flags = O_RDWR | O_CREAT;
    const int mode_flags = S_IRUSR | S_IWUSR;

    const int fd = open( pathname, open_flags, mode_flags );

    for( int cmd = 2; cmd + 1 < argc; cmd += 2 )
    {
        if( strcmp( argv[cmd], "-j" ) == 0 )
        {
            off_t current_offset = lseek( fd, 0, SEEK_CUR );
            char buffer[100];
            sprintf( buffer, "%#018llx: jump    %#018llx\n", current_offset, strtoll( argv[cmd + 1], 0, 16 ) );

            if( linux_write( STDOUT_FILENO, buffer, strlen( buffer ) ) )
            {
                return 1;
            }

            lseek( fd, (off_t)strtol( argv[cmd + 1], 0, 16 ), SEEK_SET );
        }
        else if( strcmp( argv[cmd], "-a" ) == 0 )
        {
            off_t current_offset = lseek( fd, 0, SEEK_CUR );
            char buffer[100];
            sprintf( buffer, "%#018llx: advance %#018llx\n", current_offset, strtoll( argv[cmd + 1], 0, 16 ) );

            if( linux_write( STDOUT_FILENO, buffer, strlen( buffer ) ) )
            {
                return 1;
            }
            
            lseek( fd, (off_t)strtol( argv[cmd + 1], 0, 16 ), SEEK_CUR );
        }
        else if( strcmp( argv[cmd], "-r" ) == 0 )
        {
            off_t current_offset = lseek( fd, 0, SEEK_CUR );
            char static_buffer[30];
            int string_size = sprintf( static_buffer, "%#018llx: read    ", current_offset );

            if( linux_write( STDOUT_FILENO, static_buffer, strlen( static_buffer ) ) )
            {
                return 1;
            }

            unsigned long bytes_to_read = strtoul( argv[cmd + 1], 0, 16 );
            void *dynamic_buffer = malloc( bytes_to_read + 1 );
            ssize_t bytes_read = read( fd, dynamic_buffer, bytes_to_read );
            ( (char*) dynamic_buffer )[ bytes_read ] = '\n';

            if( linux_write( STDOUT_FILENO, dynamic_buffer, bytes_to_read + 1 ) )
            {
                return 1;
            }

            free( dynamic_buffer );
        }
        else if( strcmp( argv[cmd], "-w" ) == 0 )
        {
            off_t current_offset = lseek( fd, 0, SEEK_CUR );
            char static_buffer[30+strlen(argv[cmd + 1])];
            int string_size = sprintf( static_buffer, "%#018llx: write   %s\n", current_offset, argv[cmd + 1] );

            if( linux_write( STDOUT_FILENO, static_buffer, strlen( static_buffer ) ) )
            {
                return 1;
            }

            linux_write( fd, argv[cmd + 1], strlen( argv[cmd + 1] ) );
        }
        else
        {
            linux_write( STDERR_FILENO, "Unknown command\n", 17 );
            return 1;
        }
    }

    if( close( fd ) == -1 )
    {
        return 1;
    }

    return 0;
}