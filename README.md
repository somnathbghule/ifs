# ifs
In memory file system implementation for learning  purpose.


Constraints :
no of files=20
file size=4k

etc etc

Features:
open()
close()
read()
write()

Design:
1. fs create [ not required as inmemory filesystem ]
2. fs mount  [ use of mount_nodev(),fill_super() ]
3. fs umount

Implementation:
register_filesystem()
unregister_filesystem()
