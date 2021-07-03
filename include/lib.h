#include <stdlib.h>

/*
 * Write bytes of buffer to file descriptor
 * Return 0 on success, 1 on failure
 */
int linux_write( int fd, const void* buffer, size_t bytes );
