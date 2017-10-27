#include "superblock.h"

#include <unistd.h>

#include "inode.h"

int setup_index_area(int fd, struct superblock *sb, const char *name) {
  struct inode id;
  id.type = VOLUME_IDENTIFIER;
  id.vi.time_stamp = current_time();
  strncpy(id.vi.volume_name, name, 51);
  id.vi.volume_name[51] = 0;

  lseek(fd, -sizeof(id), SEEK_END);
  int bytes_written = write(fd, &id, sizeof(id));
  if (bytes_written < 0)
    return bytes_written;
  else if (bytes_written < sizeof(id)) {
    printf("Failed to finish writing volume identifier inode! Aborting...");
    return -EPROTO;
  }
  else
    return 0;
}

int main (int argc, char **argv) {
  struct superblock sb;
# warning Hardcoded values for testing
  construct_superblock(&sb, (struct superblock_initializer){
      .index_area_size = 1,
      .num_blocks      = 7,
      .reserved_blocks = 1,
      .block_size      = 3 // 2 ^ (block_size + 7) = 1024 bytes/block
  });

  int ret;
  if ((ret = write_superblock(STDOUT_FILENO, &sb)) < sizeof(struct superblock))
    return ret;
  if ((ret = setup_index_area(&sb)) < sizeof(struct inode))
    return ret;
  return 0;
}

