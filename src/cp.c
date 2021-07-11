#include <unistd.h>
#include <fcntl.h>

#include "lib.h"

int main( int argc, char *argv[] )
{
    if( argc < 3 )
    {
        return 1;
    }

    const char *src_file = argv[1];
    const char *dst_file = argv[2];

    const int sfd = open(src_file, O_RDONLY, 0 );
    const int dfd = open(dst_file, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR );

    const int page_size = getpagesize();

    char buffer[ page_size ];

    while( 1 )
    {
        ssize_t bytes_read = read( sfd, buffer, page_size );

        if( bytes_read == 0 )
        {
            break;
        }
        else if( bytes_read == -1 )
        {
            return 1;
        }
        else
        {
            if( linux_write( dfd, buffer, bytes_read ) )
            {
                return 1;
            }
        }
        off_t current_offset = lseek( sfd, 0, SEEK_CUR );
        off_t next_data = lseek( sfd, current_offset, SEEK_DATA );
        lseek( dfd, next_data, SEEK_SET );
    }    

    if( close( sfd ) == -1 )
    {
        return 1;
    }
    if( close( dfd ) == -1 )
    {
        return 1;
    }

    return 0;
}