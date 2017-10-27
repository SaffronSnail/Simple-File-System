.PHONY: all clean

all: build/vd.hex

build/vd.hex: build/vd
	hexdump -C build/vd > build/vd.hex

build/vd: build/mkfs.simple
	tr '\0' '\377' < /dev/zero | dd bs=1024 of=$@ count=7680
	sudo losetup /dev/loop6 build/vd
	sudo su -c './build/mkfs.simple > /dev/loop6'
	sudo losetup -d /dev/loop6

build/mkfs.simple: mkfs.simple.c build/superblock.o build/inode.o
	gcc $^ -o $@

build/superblock.o: superblock.c superblock.in superblock.h internal.h
	gcc -c superblock.c -o $@

build/inode.o: inode.c inode.h internal.h
	gcc -c inode.c -o $@

clean:
	-rm build/*

