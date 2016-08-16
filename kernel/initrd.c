#include <kernel/initrd.h>

initrd_header_t* header;
initrd_file_header_t* file_headers;

fs_node_t* initrd_root;
fs_node_t* initrd_dev;

fs_node_t* root_nodes;
nt nroot_nodes;

static uint32_t initrd_read(fs_node_t* node, uint32_t offset, uint32_t size, uint8_t* buf){
    initrd_file_header_t header = file_headers[node->inode];

    if(offset>header.size){
        return 0;
    }

    if(offset+size>header.size){
        size = header.size - offset;
    }

    memcpy(buf, (uint8_t*)(header.offset+offset), size);

    return size;
}

struct dirent dirent;

static struct dirent* initrd_readdir(fs_node_t* node, uint32_t idx){
    if(node==initrd_root && idx == 0){
        strcpy(dirent.name, "dev");
        dirent.name[3] = '\0';
        dirent.ino = 0;
        return &dirent;
    }

    if(idx-1>=nroot_nodes) return NULL;

    uint32_t namelen = strlen(root_nodes[idx-1].name);

    strcpy(dirent.name, root_nodes[idx-1].name);
    dirent.name[namelen] = '\0';
    dirent.ino = root_nodes[idx-1].inode;
    return &dirent;
}

static fs_node_t* initrd_finddir(fs_node_t* node, char* name){
    if(node==initrd_root && !strcmp(name, "dev")){
        return initrd_dev;
    }

    uint32_t i;
    for(i=0;i<nroot_nodes;i++){
        if(!strcmp(name, root_nodes[i].name)){
            return &root_nodes[i];
        }
    }
}

fs_node_t* initrd_parse(mb_module_t* mod){
    return NULL;
}
