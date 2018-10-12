#ifndef IFS_HEADER_H
#define IFS_HEADER_H

#include <linux/kernel.h>

#ifdef DEBUG
#define DPRINTK printk
#else
#define DPRINTK(...)
#endif

#define IFS_MAGIC 0x123456789
#define MAX_IFS_FILESIZE 512

#endif
