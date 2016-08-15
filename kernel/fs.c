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

uint32_t open_fs(fs_node_t* node){
    if(node->open){
        return node->open(node);
    } else {
        return 0;
    }
}

uint32_t close_fs(fs_node_t* node){
    if(node->close){
        return node->close(node);
    } else {
        return 0;
    }
}

struct dirent* readdir_fs(fs_node_t* node, uint32_t idx){
    if(node->readdir&& (node&FS_DIRECTORY) != 0){
        return node->readdir(node);
    } else {
        return NULL;
    }
}

fs_node_t* finddir_fs(fs_node_t* node, char* name){
    if(node->finddir&& (node&FS_DIRECTORY) != 0){
        return node-finddir(name);
    } else {
        return NULL;
    }
}
