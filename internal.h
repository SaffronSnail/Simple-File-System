#pragma once

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAGIC 0x534653
#define VERSION 0x10

#define BYTES(num) (8 * num)

static inline unsigned current_time() {
  struct timespec time;
  clock_gettime(CLOCK_MONOTONIC, &time);
  return (unsigned)(((double)time.tv_sec + (1.0e-9 * time.tv_nsec)) * 65536);
}

static inline unsigned calculate_checksum(unsigned num_blocks,
                            unsigned reserved_blocks,
                            unsigned block_size) {
  return 0b100000000 - MAGIC - VERSION - num_blocks - reserved_blocks
                       - block_size;
}

static inline char *to_unicode(const char *source) {
  char *ret = (char *)malloc(strlen(source) + 1);
  strcpy(ret, source);
  return ret;
}

