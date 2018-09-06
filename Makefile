#================Auto Generated======================
MODNAME=$(shell basename $(PWD))
SOURCE_FILES:=$(shell find $(PWD) -name '*.c' -exec basename {} \; | grep -v '\.mod')
OBJ_FILES:=$(patsubst %.c, %.o, $(SOURCE_FILES))
obj-m += ${MODNAME}.o
${MODNAME}-objs := ${OBJ_FILES}
EXTRA_CFLAGS=-I$(PWD)
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
load:
	sudo insmod *.ko
unload:
	sudo rmmod ${MODNAME}
mnt:
	sudo mount -osize=100m ifs /mnt -t ifs
umt:
	sudo umount /mnt
