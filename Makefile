#================Auto Generated======================
MODNAME=$(shell basename $(PWD))
SOURCE_FILES:=$(shell find $(PWD) -name '*.c' -exec basename {} \; | grep -v '\.mod')
OBJ_FILES:=$(patsubst %.c, %.o, $(SOURCE_FILES))
obj-m += ${MODNAME}.o
${MODNAME}-objs := ${OBJ_FILES}
EXTRA_CFLAGS=-I$(PWD) -DDEBUG
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
load:
	sudo insmod *.ko
	sudo mount ifs /mnt -t ifs
unload:
	sudo umount /mnt
	sudo rmmod ${MODNAME}
create:
	sudo touch /mnt/abc
rm: 
	sudo rm /mnt/abc
ls:
	sudo ls /mnt/abc -l
