.PHONY: all clean

all: vd.hex

vd.hex: vd
	hexdump -C vd > vd.hex

vd: mksfs
	tr '\0' '\377' < /dev/zero | dd bs=1024 of=$@ count=7680
	sudo losetup /dev/loop6 vd
	sudo su -c './mksfs > /dev/loop6'
	sudo losetup -d /dev/loop6

mksfs: mksfs.c superblock.o
	gcc mksfs.c superblock.o -o $@

superblock.o: superblock.c superblock.in superblock.h
	gcc -c superblock.c -o $@

clean:
	-rm mksfs
	-rm superblock.o
	-rm vd
	-rm vd.hex

