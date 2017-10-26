#pragma once

#include <time.h>

#define MAGIC 0x534653
#define VERSION 0x10

struct superblock {
#define BITS_PER_BYTE 8
#define SUPERBLOCK_ENTRY(offset, size, name) unsigned long name : size * BITS_PER_BYTE;
#include "superblock.in"
#undef SUPERBLOCK_ENTRY
#undef BITS_PER_BYTE
};

inline unsigned current_time() {
  struct timespec time;
  clock_gettime(CLOCK_MONOTONIC, &time);
  return ((double)time.tv_sec + (1.0e-9 * time.tv_nsec)) * 65536;
}

inline unsigned calculate_checksum(unsigned num_blocks,
                            unsigned reserved_blocks,
                            unsigned block_size) {
  return 0b100000000 - MAGIC - VERSION - num_blocks - reserved_blocks
                       - block_size;
}

void construct_superblock(struct superblock *buffer,
                          unsigned data_area_size,
                          unsigned index_area_size,
                          unsigned num_blocks,
                          unsigned reserved_blocks,
                          unsigned block_size
                         ) {
  *buffer = {
    .time_stamp = current_time(),
    .data_area_size = data_area_size,
    .index_area_size = index_area_size,
    .magic_number    = MAGIC,
    .version_number  = VERSION,
    .num_blocks      = num_blocks,
    .reserved_blocks = reserved_blocks,
    .block_size      = block_size,
    .checksum        = calculate_checksum(num_blocks, reserved_blocks,
                                          block_size)
  };
}



