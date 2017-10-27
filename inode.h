#pragma once

#include "internal.h"

enum __attribute__((__packed__)) entry_type {
  VOLUME_IDENTIFIER = 0x01,
  STARTING_MARKER = 0x02,
  UNUSED = 0x10,
  DIRECTORY = 0x11,
  FILE = 0x12,
  UNUSABLE = 0x18,
  DELETED_DIRECTORY = 0x19,
  DELETED_FILE = 0x1A,

  // a continuation will not necisarily have this byte sequence on-disk, but
  // this is how continuations are identified elsewhere
  CONTINUATION = 0x20
};

struct __attribute__((__packed__)) volume_identifier {
  unsigned _reserved_ : BYTES(3);
  unsigned long time_stamp : BYTES(8);
  char volume_name[52];
};

struct __attribute__((__packed__)) directory {
  unsigned continuations : BYTES(1);
  unsigned long time_stamp : BYTES(8);
  char name[54];
};

struct __attribute__((__packed__)) file {
  unsigned continuations : BYTES(1);
  unsigned long time_stamp : BYTES(8);
  unsigned long data_start : BYTES(8);
  unsigned long data_end : BYTES(8);
  unsigned long file_length : BYTES(8);
  char name[30];
};

struct __attribute__((__packed__)) unusable {
  char _reserved_[9];
  unsigned starting_block_number;
  unsigned ending_block_numker;
};

struct __attribute__((__packed__)) inode {
  union {
    struct {
      enum entry_type type;
      union {
        struct volume_identifier vi;
        struct directory dir;
        struct file file;
        struct unusable unusable;
      };
    };
    char continuation[64];
  };
};

