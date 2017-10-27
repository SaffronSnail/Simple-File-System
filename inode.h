#pragma once

#include "internal.h"

enum __attribute__((__packed__)) entry_type {
  SFS_ENTRY_VOLUME_IDENTIFIER = 0x01,
  SFS_ENTRY_STARTING_MARKER = 0x02,
  SFS_ENTRY_UNUSED = 0x10,
  SFS_ENTRY_DIRECTORY = 0x11,
  SFS_ENTRY_FILE = 0x12,
  SFS_ENTRY_UNUSABLE = 0x18,
  SFS_ENTRY_DELETED_DIRECTORY = 0x19,
  SFS_ENTRY_DELETED_FILE = 0x1A,

  // a continuation will not necessarily have this byte sequence on-disk, but
  // this is how continuations are identified elsewhere
  SFS_ENTRY_CONTINUATION = 0x20
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

int write_inode(int fd, struct inode *in,
                       const char *partial_write_message);

