#include "inode.h"

#include <unistd.h>
#include <stdio.h>

int write_inode(int fd, struct inode *in,
                       const char *partial_write_message)
{
  int bytes_written = write(fd, in, sizeof(*in));
  if (bytes_written < 0 || bytes_written == sizeof(*in))
    return bytes_written;
  else {
    if (partial_write_message == NULL)
      printf("Error: inode didn't finish writing!");
    else
      printf("%s", partial_write_message);
    return -EPROTO;
  }
}


