#include <kernel/initrd.h>

initrd_header_t* header;
initrd_file_header_t* file_headers;

fs_node_t* initrd_root;
fs_node_t* initrd_dev;

fs_node_t* root_nodes;
int nroot_nodes;

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

fs_node_t* initrd_parse(uint32_t initrd_start){
    header = (initrd_header_t*)(initrd_start);
    file_headers = (initrd_file_header_t*)(initrd_start+sizeof(initrd_header_t));

    // todo: add more of an error handling thing than assert?
    ASSERT(header->magic == INITRD_MAGIC);

    // initialize root
    initrd_root = (fs_node_t*)kmalloc(sizeof(fs_node_t));
    strcpy(initrd_root->name, "initrd");
    initrd_root->mask = initrd_root->uid = initrd_root->gid = 0;
    initrd_root->inode = initrd_root->length = 0;
    initrd_root->flags = FS_DIRECTORY;
    initrd_root->read = NULL;
    initrd_root->write = NULL;
    initrd_root->open = NULL;
    initrd_root->close = NULL;
    initrd_root->readdir = &initrd_readdir;
    initrd_root->finddir = &initrd_finddir;
    initrd_root->ptr = initrd_root->impl = 0;

    // initialize /dev
    initrd_dev = (fs_node_t*)kmalloc(sizeof(fs_node_t));
    strcpy(initrd_root->name, "initrd");
    initrd_dev->mask = initrd_dev->uid = initrd_dev->gid = 0;
    initrd_dev->inode = initrd_dev->length = 0;
    initrd_dev->flags = FS_DIRECTORY;
    initrd_dev->read = NULL;
    initrd_dev->write = NULL;
    initrd_dev->open = NULL;
    initrd_dev->close = NULL;
    initrd_dev->readdir = &initrd_readdir;
    initrd_dev->finddir = &initrd_finddir;
    initrd_dev->ptr = initrd_dev->impl = 0;

    root_nodes = (fs_node_t*)kmalloc(sizeof(fs_node_t)*(header->numfiles));
    nroot_nodes = header->numfiles;
    int i;
    for(i=0;i<nroot_nodes;i++){
        ASSERT(file_headers[i].magic == INITRD_MAGIC);

        // offset at the header is from the start of the ramdisk
        // we convert it to memory address
        file_headers[i].offset += initrd_start;

        // initialize the file node;
        strcpy(root_nodes[i].name, &file_headers[i].name);initrd_dev->mask = initrd_dev->uid = initrd_dev->gid = 0;

        root_nodes[i].mask = root_nodes[i].uid = root_nodes[i].gid = 0;
        root_nodes[i].inode = i;
        root_nodes[i].length = file_headers[i].size;
        root_nodes[i].flags = FS_DIRECTORY;
        root_nodes[i].read = &initrd_read;
        root_nodes[i].write = NULL;
        root_nodes[i].open = NULL;
        root_nodes[i].close = NULL;
        root_nodes[i].readdir = NULL;
        root_nodes[i].finddir = NULL;
        root_nodes[i].ptr = root_nodes[i].impl = 0;
    }

    return NULL;
}
