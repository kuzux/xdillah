#include <kernel/fs.h>

fs_node_t* fs_root = NULL;

uint32_t read_fs(fs_node_t* node, uint32_t offset, uint32_t size, uint8_t* buf){
    if(node->read){
        return node->read(node, offset, size, buf);
    } else {
        return 0;
    }
}

uint32_t write_fs(fs_node_t* node, uint32_t offset, uint32_t size, uint8_t* buf){
    if(node->write){
        return node->write(node, offset, size, buf);
    } else {
        return 0;
    }
}

void open_fs(fs_node_t* node){
    if(node->open){
        node->open(node);
    }
}

void close_fs(fs_node_t* node){
    if(node->close){
        node->close(node);
    }
}

struct dirent* readdir_fs(fs_node_t* node, uint32_t idx){
    if(node->readdir&& (node->flags&FS_DIRECTORY) != 0){
        return node->readdir(node, idx);
    } else {
        return NULL;
    }
}

fs_node_t* finddir_fs(fs_node_t* node, char* name){
    if(node->finddir&& (node->flags&FS_DIRECTORY) != 0){
        return node->finddir(node, name);
    } else {
        return NULL;
    }
}
