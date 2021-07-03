#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main( int argc, char *argv[] )
{
    if( argc < 3 )
    {
        write( STDERR_FILENO, "Usage: seek [FILE] [r<length>|R<length>|w<string>|s<offset>]...\n", 65 );
        return 1;
    }

    const char *pathname = argv[1];

    const int open_flags = O_RDWR | O_CREAT;
    const int mode_flags = S_IRUSR | S_IWUSR;

    const int fd = open( pathname, open_flags, mode_flags );

    for( int cmd = 2; cmd + 1 < argc; cmd += 2 )
    {
        if( strcmp( argv[cmd], "-o" ) == 0 )
        {
            off_t current_offset = lseek( fd, (int)strtol( argv[cmd + 1], 0, 16 ), SEEK_SET );
            char buffer[27];
            sprintf( buffer, "0x%llx: seek\n", current_offset );

            ssize_t bytes_written = 0;
            ssize_t total_bytes_written = 0;

            while( total_bytes_written < strlen( buffer ) )
            {
                bytes_written = write( STDOUT_FILENO, buffer, strlen( buffer ) );
                total_bytes_written += bytes_written;

                if( bytes_written < 0 )
                {                    
                    return 1;
                }
            }
        }
        else if( strcmp( argv[cmd], "-a" ) == 0 )
        {
            off_t current_offset = lseek( fd, (int)strtol( argv[cmd + 1], 0, 16 ), SEEK_CUR );
            char buffer[30];
            sprintf( buffer, "0x%llx: advance\n", current_offset );

            ssize_t bytes_written = 0;
            ssize_t total_bytes_written = 0;

            while( total_bytes_written < strlen( buffer ) )
            {
                bytes_written = write( STDOUT_FILENO, buffer, strlen( buffer ) );
                total_bytes_written += bytes_written;

                if( bytes_written < 0 )
                {                    
                    return 1;
                }
            }
        }
        else if( strcmp( argv[cmd], "-r" ) == 0 )
        {
            unsigned long bytes_to_read = strtoul( argv[cmd + 1], 0, 16 );
            void *buffer = malloc( bytes_to_read + 6 );
            strcpy( buffer, "Read: " );
            ssize_t bytes_read = read( fd, buffer + 6, bytes_to_read );

            ssize_t bytes_written = 0;
            ssize_t total_bytes_written = 0;
            
            while( bytes_written != -1 && total_bytes_written < bytes_read )
            {
                bytes_written = write( STDOUT_FILENO, buffer, bytes_read + 6 );
                total_bytes_written += bytes_written;

                if( bytes_written < 0 )
                {                    
                    free( buffer );
                    return 1;
                }
            }

            free( buffer );
        }
        else if( strcmp( argv[cmd], "-w" ) == 0 )
        {
            off_t offset = lseek( fd, 0, SEEK_CUR );
            ssize_t bytes_written = write( fd, argv[cmd + 1], strlen( argv[cmd + 1] ) );
            void *buffer = malloc( strlen( argv[cmd + 1] ) + 100 );
            int result = sprintf( buffer, "Wrote %s at position 0x%llx\n", argv[cmd + 1], offset );
            bytes_written = write( STDOUT_FILENO, buffer, strlen( buffer ) );
            free( buffer );
        }
        else
        {
            write( STDERR_FILENO, "Unknown command\n", 17 );
            return 1;
        }
    }

    return 0;
}