#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main( int argc, char *argv[] )
{
    if( argc < 2 || argc > 3 )
    {
        write( STDERR_FILENO,
               "Usage: tee [OPTION]... [FILE]...\n"
               "Copy standard input to FILE, and also to standard output.\n\n"
               " -a\tappend to the given FILE, do not overwrite\n",
               140 );

        return 1;
    }

    const char *pathname = 0;
    const char *append = 0;

    int open_flags = O_WRONLY | O_CREAT;
    int mode_flags = S_IRUSR | S_IWUSR | S_IEXEC;

    if( argc == 2 )
    {
        pathname = argv[1];
    }
    else
    {
        open_flags |= O_APPEND;
        pathname = argv[2];
    }

    const int fd = open( pathname, open_flags, mode_flags );

    if( fd == -1 )
    {
        write( STDERR_FILENO, "Failed to open file\n", 21 );
        return 1;
    }

    const size_t buffer_size = 10;
    char buffer[buffer_size];

    while( 1 )
    {
        ssize_t bytes_read = read( STDIN_FILENO, buffer, buffer_size );

        if( bytes_read == -1 )
        {
            write( STDERR_FILENO, "Failed to read form STDIN\n", 27 );
            return 1;
        }
        else if( bytes_read == 0 )
        {
            break;
        }

        if( write( STDOUT_FILENO, buffer, bytes_read ) != bytes_read )
        {
            write( STDERR_FILENO, "Failed to write to STDOUT\n", 27 );
            return 1;
        }
        if( write( fd, buffer, bytes_read ) != bytes_read )
        {
            write( STDERR_FILENO, "Failed to write to file\n", 25 );
            return 1;
        }
    }

    if( close( fd ) == -1 )
    {
        write( STDERR_FILENO, "Failed to close file\n", 22 );
        return 1;
    }

    return 0;
}
