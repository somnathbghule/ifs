#ifndef IFS_HEADER_H
#define IFS_HEADER_H

#include <linux/kernel.h>

#ifdef DEBUG
#define DPRINTK printk
#else
#define DPRINTK(...)
#endif
#endif

#define IFS_MAGIC 0x123456789
