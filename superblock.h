/* Here we define a struct (and associated operations) which has the exact
 * layout that the superblock is specified to have on-disk (sans beginning and
 * end padding)
 */
#pragma once

#include "internal.h"

// This structure needs to be packed in order to faithfully represent the bytes
// as they should appear on-disk
struct __attribute__((__packed__)) superblock {
#define SUPERBLOCK_ENTRY(offset, size, name) unsigned long name : BYTES(size);
#include "superblock.in"
#undef SUPERBLOCK_ENTRY
};

struct superblock_initializer {
  unsigned index_area_size;
  unsigned num_blocks;
  unsigned reserved_blocks;
  unsigned block_size;
};

void construct_superblock(struct superblock *buffer,
                          struct superblock_initializer);

int write_superblock(int out, struct superblock *sb);

