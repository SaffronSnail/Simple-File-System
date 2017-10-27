.PHONY: all clean

all: vd.hex

vd.hex: vd
	hexdump -C vd > vd.hex

vd: mkfs.simple
	tr '\0' '\377' < /dev/zero | dd bs=1024 of=$@ count=7680
	sudo losetup /dev/loop6 vd
	sudo su -c './mkfs.simple > /dev/loop6'
	sudo losetup -d /dev/loop6

mkfs.simple: mkfs.simple.c superblock.o
	gcc $^ -o $@

superblock.o: superblock.c superblock.in superblock.h
	gcc -c superblock.c -o $@

clean:
	-rm *.o
	-rm *.gch
	-rm mksfs
	-rm vd
	-rm vd.hex

