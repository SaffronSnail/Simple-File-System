#include "superblock.h"

#include <unistd.h>

void setup_index_area(struct superblock *sb) {}

int main () {
  struct superblock sb;
  construct_superblock(&sb, (struct superblock_initializer){
      .index_area_size = 0,
      .num_blocks      = 0,
      .reserved_blocks = 0,
      .block_size      = 0
  });

  int ret;
  if ((ret = write_superblock(STDOUT_FILENO, &sb)) < sizeof(struct superblock))
    return ret;
  setup_index_area(&sb);
}

