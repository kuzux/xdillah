#include <kernel/amfs.h>
#include <kernel/kheap.h>
#include <string.h>

fs_node_t* root;

typedef struct children_assoc {
    int inode;

    // that is a linked list
    struct children_assoc* next;
} children_assoc_t;

// those are simple arrays indexed by inode number
char** file_contents;
char** file_names;
// parents array can be backtracked to the root
// it ends up containing the filesystem tree structure
int32_t* parents;
children_assoc_t** children;
fs_node_t* files;

static uint32_t amfs_read(fs_node_t* node, uint32_t offset, uint32_t size, uint8_t* buf){
    // follow if there is a link
    while(node->flags == FS_SYMLINK){
        node = node->ptr;
    }

    // bail if we don't have a file
    if(node->flags & FS_FILE == 0){
        return 0;
    }

    if(offset+size > node->length){
        return 0;
    }

    memcpy(buf, file_contents[node->inode]+offset, size);
    return size;
}

static struct dirent* amfs_readdir(fs_node_t* node, uint32_t idx){
    // follow if there is a link
    while(node->flags == FS_SYMLINK){
        node = node->ptr;
    }

    // children list is null
    if(children[node->inode]==NULL){
        return NULL;
    }

    // if the file isn't a directory
    if(node->flags&FS_DIRECTORY == 0){
        return NULL;
    }

    // traverse the children list
    children_assoc_t* curr = children[node->inode];
    uint32_t i = 1;
    while(curr){
        if(i == idx){
            struct dirent* res = kmalloc(sizeof(struct dirent));

            strcpy(res->name, file_names[curr->inode]);
            res->ino = curr->inode;

            return res;
        }
        curr = curr->next;
        i++;
    }

    // we couldn't find a file with the index
    return NULL;
}

static fs_node_t* amfs_finddir(fs_node_t* node, char* name){
    // follow if there is a link
    while(node->flags == FS_SYMLINK){
        node = node->ptr;
    }

    // children list is null
    if(children[node->inode]==NULL){
        return NULL;
    }

    // if the file isn't a directory
    if(node->flags&FS_DIRECTORY == 0){
        return NULL;
    }

    // traverse the children list
    children_assoc_t* curr = children[node->inode];
    while(curr){
        if(file_names[curr->inode] && strcmp(file_names[curr->inode], name) == 0){
            return &files[curr->inode];
        }
        curr = curr->next;
    }

    // we couldn't find a file with the name
    return NULL;
}

fs_node_t* amfs_init(){
    root = kmalloc(sizeof(fs_node_t));
    
    file_contents = kmalloc(65536*sizeof(char*)); // max no of files is 65536
    file_names = kmalloc(65536*sizeof(char*)); 
    
    parents = kmalloc(65536*sizeof(int32_t));
    children = kmalloc(65536*sizeof(children_assoc_t*));

    files = kmalloc(65536*sizeof(fs_node_t));

    root->mask = 0;
    root->uid = root->gid = 0;
    root->flags = FS_MOUNTPOINT | FS_DIRECTORY;
    root->inode = 0;
    root->impl = 0;
    root->ptr = NULL;
    root->read = NULL;
    root->write = NULL;
    root->open = NULL;
    root->close = NULL;
    root->readdir = &amfs_readdir;
    root->finddir = &amfs_finddir;

    // first filename is an empty string
    file_names[0] = kmalloc(1);
    file_names[0][0] = '\0';

    parents[0] = -1;
    children[0] = NULL;

    return root;
}
