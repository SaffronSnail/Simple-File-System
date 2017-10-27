#include "superblock.h"

#include <stdio.h>
#include <unistd.h>

#include "inode.h"

int setup_index_area(int fd, struct superblock *sb, const char *name) {
  struct inode starting_marker;
  struct inode id;

  memset(&starting_marker, 0, sizeof(starting_marker));
  starting_marker.type = SFS_ENTRY_STARTING_MARKER;

  lseek(fd, media_size(*sb) - sizeof(id) - sizeof(starting_marker), SEEK_SET);
  int bytes_written = write_inode(fd, &starting_marker, "Failed to finish writing volume identifier inode! Aborting.");
  if (bytes_written < 0)
    return bytes_written;

  memset(&id, 0, sizeof(id));
  id.type = SFS_ENTRY_VOLUME_IDENTIFIER;
  id.vi.time_stamp = current_time();
  strncpy(id.vi.volume_name, name, 51);
  id.vi.volume_name[51] = 0;

  lseek(fd, media_size(*sb) - sizeof(id), SEEK_SET);
  bytes_written = write_inode(fd, &id, "Failed to finish writing volume identifier inode! Aborting.");
  if (bytes_written < 0)
    return bytes_written;
  else
    return 0;
}

int main (int argc, char **argv) {
  struct superblock sb;
# warning Hardcoded values for testing
  construct_superblock(&sb, (struct superblock_initializer){
      .index_area_size = 1,
      .num_blocks      = 7 * 1024,
      .reserved_blocks = 1,
      .block_size      = 3 // 2 ^ (block_size + 7) = 1024 bytes/block
  });

  int ret;
  if ((ret = write_superblock(STDOUT_FILENO, &sb)) < sizeof(struct superblock))
    return ret;
  if ((ret = setup_index_area(STDOUT_FILENO, &sb, "TEST VOLUME")) < sizeof(struct inode))
    return ret;
  return 0;
}

