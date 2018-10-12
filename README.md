# ifs
In memory file system implementation for learning  purpose.


Constraints :
no of files=20
file size=4k

etc etc

Features:
open()
create()
unlink()
close()
read()
write()

Design:
1. fs create [ not required as inmemory filesystem ]
2. fs mount  [ use of mount_nodev(),fill_super() ]
3. fs umount [ kill_sb(), kill_litter_super() ]
4. stat      [ getattr() and statfs() ]
5. df 	     [ same like stat ] 
6. ls        [ getattr() ] 
7. create    [ create(), new_inode() and setattr() ]
8. unlink    [ unlink()]
9. read	     [address_operations]
10. write    [address_oparations]

Implementation:
register_filesystem()
unregister_filesystem()
mount();
umount();
statistics()
list()
create()
unlink()
read()
write()

============================================
Current Complete Status:
mount, umount, stats, list, create, unlink, read, write done.
