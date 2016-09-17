#ifndef _FS_H
#define _FS_H

#include <stddef.h>
#include <stdint.h>

#define FS_FILE        0x01
#define FS_DIRECTORY   0x02
#define FS_CHARDEVICE  0x03
#define FS_BLOCKDEVICE 0x04
#define FS_PIPE        0x05
#define FS_SYMLINK     0x06
#define FS_MOUNTPOINT  0x08 // Is the file an active mountpoint?

struct fs_node; // we need to declare but not define it yet

typedef uint32_t (*read_type_t)(struct fs_node*, uint32_t, uint32_t, uint8_t*);
typedef uint32_t (*write_type_t)(struct fs_node*, uint32_t, uint32_t, uint8_t*);
typedef void (*open_type_t)(struct fs_node*);
typedef void (*close_type_t)(struct fs_node*);
typedef struct dirent* (*readdir_type_t)(struct fs_node*, uint32_t);
typedef struct fs_node* (*finddir_type_t)(struct fs_node*, char*);

typedef struct fs_node{
    // various properties of the file
    uint32_t mask;   // permissions mask
    uint32_t uid;    // owner user
    uint32_t gid;    // owner group
    uint32_t flags;  // see the #define s above
    uint32_t inode;  // provides a way for the fs to identify the file
    uint32_t length; // length of the file in bytes
    uint32_t impl;   // An implementation defined number
    struct fs_node *ptr; // used by symlinks and mount points

    // direct pointers
    // 12 direct block pointers
    // indirect points to an array of 4 blocks
    // double indirect points to an array of 4 pointer structures
    // triple indirect points to an array of 4 double pointer structures
    uint32_t direct_blocks[12];
    uint32_t** indirect;
    uint32_t*** dbl_indirect;
    uint32_t**** triple_indirect;
    
    // various functions relating to the file 
    read_type_t read;
    write_type_t write;
    open_type_t open;
    close_type_t close;
    readdir_type_t readdir;
    finddir_type_t finddir; // find a child file by name

} fs_node_t;

struct dirent{
    char name[128];
    uint32_t ino; // inode
};

extern fs_node_t *fs_root; // The root of the filesystem.

#endif