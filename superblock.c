#include "superblock.h"

#include <stdio.h>
#include <unistd.h>

void construct_superblock(struct superblock *buffer,
                          struct superblock_initializer init
                         )
{
  // TODO: Make sure that the compiler actually optimizes away the superblock on
  // the stack
  struct superblock block = {
    // set all 8 bytes; a clear indication that the time stamp was not properly
    // updated when the structure was written
    .time_stamp      = 0xFFFFFFFFFFFFFFFF,
    .data_area_size  = 0,
    .index_area_size = init.index_area_size,
    .magic_number    = MAGIC,
    .version_number  = VERSION,
    .num_blocks      = init.num_blocks,
    .reserved_blocks = init.reserved_blocks,
    .block_size      = init.block_size,
    .checksum        = calculate_checksum(init.num_blocks, init.reserved_blocks,
                                          init.block_size)
  };
  *buffer = block;
}

int write_superblock(int out, struct superblock *sb)
{
  sb->time_stamp = current_time();

  lseek(out, SUPERBLOCK_BEGIN, SEEK_SET);
  int bytes_written = write(out, sb, sizeof(struct superblock));
  if (bytes_written < 0)
    return bytes_written;
  else if (bytes_written < (int)sizeof(struct superblock)) {
    printf("Wrote %d bytes, expected to write %ld bytes!", bytes_written,
           sizeof(struct superblock)
          );
    return -EPROTO; // Seems like the best fit...
  }
  else
    return bytes_written;
}

