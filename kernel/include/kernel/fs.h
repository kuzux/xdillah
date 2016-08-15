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
    char name[128];  // filename
    uint32_t mask;   // permissions mask
    uint32_t uid;    // owner user
    uint32_t gid;    // owner group
    uint32_t flags;  // see the #define s above
    uint32_t inode;  // provides a way for the fs to identify the file
    uint32_t length; // length of the file in bytes

    // various functions relating to the file 
    read_type_t read;
    write_type_t write;
    open_type_t open;
    close_type_t close;
    readdir_type_t readdir;
    finddir_type_t finddir; // find a child file by name

    struct fs_node *ptr; // used by symlinks and mount points
} fs_node_t;

struct dirent{
    char name[128];
    uint32_t ino; // inode
};

extern fs_node_t *fs_root; // The root of the filesystem.

uint32_t read_fs(fs_node_t* node, uint32_t offset, uint32_t size, uint8_t* buf);
uint32_t write_fs(fs_node_t* node, uint32_t offset, uint32_t size, uint8_t* buf);
uint32_t open_fs(fs_node_t* node);
uint32_t close_fs(fs_node_t* node);
struct dirent* readdir_fs(fs_node_t* node, uint32_t idx);
fs_node_t* finddir_fs(fs_node_t* node, char* name);

#endif