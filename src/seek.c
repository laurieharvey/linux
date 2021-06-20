#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

int main( int argc, char *argv[] )
{
    if( argc < 3 )
    {
        write( STDERR_FILENO, "Usage: seek [FILE] [r<length>|R<length>|w<string>|s<offset>]...\n", 65 );
    }

    const char *pathname = argv[1];

    const int open_flags = O_WRONLY | O_CREAT;
    const int mode_flags = S_IRUSR | S_IWUSR;

    const int fd = open( pathname, open_flags, mode_flags );

    for( int cmd = 2; cmd + 1 < argc; cmd += 2 )
    {
        if( argv[cmd] == "-o" )
        {
            off_t current_offset = lseek( fd, (int)strtol( argv[cmd + 1], 0, 10 ), SEEK_SET );
            char buffer[100];
            printf( buffer, "Seeked to position %Xu", current_offset );
            ssize_t bytes_written = write( STDOUT_FILENO, buffer, sizeof( buffer ) );
        }
        else if( argv[cmd] == "-a" )
        {
            off_t current_offset = lseek( fd, (int)strtol( argv[cmd + 1], 0, 10 ), SEEK_CUR );
            char buffer[100];
            printf( buffer, "Seeked to position %Xu", current_offset );
            ssize_t bytes_written = write( STDOUT_FILENO, buffer, sizeof( buffer ) );
        }
        else if( argv[cmd] == "-r" )
        {
            unsigned long bytes_to_read = strtoul( argv[cmd + 1], 0, 10 );
            void *buffer = malloc( bytes_to_read );
            ssize_t bytes_read = read( fd, buffer, bytes_to_read );
            ssize_t bytes_written = write( STDOUT_FILENO, buffer, bytes_to_read );
        }
        else if( argv[cmd] == "-w" )
        {
            off_t offset = lseek( fd, 0, SEEK_CUR );
            ssize_t bytes_written = write( fd, argv[cmd + 1], strlen( argv[cmd + 1] ) );
            void *buffer = malloc( strlen( argv[cmd + 1] ) + 100 );
            printf( buffer, "Wrote %s at position %Xll", argv[cmd + 1], offset );
            bytes_written = write( STDOUT_FILENO, buffer, strlen( buffer ) );
        }
        else
        {
            write( STDERR_FILENO, "Unknown command\n", 17 );
            return 1;
        }
    }

    return 0;
}