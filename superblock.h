#pragma once

#include <time.h>

#define MAGIC 0x534653
#define VERSION 0x10

#define BITS_PER_BYTE 8
struct superblock {
  // 404 bytes of padding for compatibility with boot code and BIOS
  unsigned long time_stamp      : 8 * BITS_PER_BYTE;
  unsigned long date_area_size  : 8 * BITS_PER_BYTE;
  unsigned long index_area_size : 8 * BITS_PER_BYTE;
  unsigned      magic_number    : 3 * BITS_PER_BYTE;
  unsigned      version_number  : 1 * BITS_PER_BYTE;
  unsigned long num_blocks      : 8 * BITS_PER_BYTE;
  unsigned      reserved_blocks : 4 * BITS_PER_BYTE;
  unsigned      block_size      : 1 * BITS_PER_BYTE;
  unsigned      checksum        : 1 * BITS_PER_BYTE;
  // 66 bytes of padding for compatibility with partition tables and a boot
  // signature
};

inline unsigned current_time() {
  struct timespec time;
  clock_gettime(CLOCK_MONOTONIC, &time);
  return ((double)time.tv_sec + (1.0e-9 * time.tv_nsec)) * 65536;
}

inline unsigned calculate_checksum(unsigned num_blocks,
                            unsigned reserved_blocks,
                            unsigned block_size) {
#warning Can I do arithmetic with a number this big?
  return 0x100000000 - MAGIC - VERSION - num_blocks - reserved_blocks
                     - block_size;
}

struct superblock construct_superblock(unsigned date_area_size,
                                       unsigned index_area_size,
                                       unsigned num_blocks,
                                       unsigned reserved_blocks,
                                       unsigned block_size
                                      ) {
  struct superblock ret = {
    .time_stamp = current_time(),
    .date_area_size = date_area_size,
    .index_area_size = index_area_size,
    .magic_number    = MAGIC,
    .version_number  = VERSION,
    .num_blocks      = num_blocks,
    .reserved_blocks = reserved_blocks,
    .block_size      = block_size,
    .checksum        = calculate_checksum(num_blocks, reserved_blocks,
                                          block_size)
  };
  return ret;
}



