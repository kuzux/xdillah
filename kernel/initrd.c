#include <kernel/initrd.h>
#include <stdio.h>
/**
 * Functions that deal with initializing and reading from the initial ramdisk
 * The initrd is generated with the generate_initrd executable separate 
 * from the kernel. The initrd is a read-only filesystem for storing various 
 * kernel modules, executables separate from the kernel itself, as well as
 * configuration files that will be read by the kernel. It is separate from the 
 * main hdd reader, it is loaded in the main executable of the kernel, because
 * the functions responsible for loading the hard disk will be located in a 
 * separate executable file still working in supervisor mode, but in a separate 
 * executable. Initrd is a flat, non-hierarchical filesystem, it is loaded as a 
 * multiboot module, its location is read from the multiboot header that the 
 * bootloader provides, its header contains a magic number, the number, names 
 * and lengths of the files. Then the file contents themselves are stored. In the 
 * in the initrd filesystem, /dev is a subdirectory, but it just redirects 
 * everything to the main (and only) directory
**/

initrd_header_t* header;
initrd_file_header_t* file_headers;

fs_node_t* initrd_root;
fs_node_t* initrd_dev;

fs_node_t* root_nodes;
int nroot_nodes;
char** filenames;

static uint32_t initrd_read(fs_node_t* node, uint32_t offset, uint32_t size, uint8_t* buf){
    initrd_file_header_t header = file_headers[node->inode-2];

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

    uint32_t namelen = strlen(filenames[idx+1]);

    strcpy(dirent.name, filenames[idx+1]);
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
        if(!strcmp(name, filenames[i+2])){
            return &(root_nodes[i]);
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
    initrd_dev->mask = initrd_dev->uid = initrd_dev->gid = 0;
    initrd_dev->inode = 1;
    initrd_dev->length = 0;
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
    
    // init the filenames array
    filenames = kmalloc(nroot_nodes*sizeof(char*));
    
    // init the root directory names
    filenames[0] = kmalloc(4);
    filenames[1] = kmalloc(7);
    
    strcpy(filenames[0], "dev");
    filenames[0][3] = 0;
    strcpy(filenames[1], "initrd");
    filenames[1][6] = 0;
    
    int i;
    for(i=0;i<nroot_nodes;i++){
        ASSERT(file_headers[i].magic == INITRD_MAGIC);

        // offset at the header is from the start of the ramdisk
        // we convert it to memory address
        file_headers[i].offset += initrd_start;

        // initialize the file node;
        initrd_dev->mask = initrd_dev->uid = initrd_dev->gid = 0;

        root_nodes[i].mask = root_nodes[i].uid = root_nodes[i].gid = 0;
        root_nodes[i].inode = i+2;
        root_nodes[i].length = file_headers[i].size;
        root_nodes[i].flags = FS_DIRECTORY;
        root_nodes[i].read = &initrd_read;
        root_nodes[i].write = NULL;
        root_nodes[i].open = NULL;
        root_nodes[i].close = NULL;
        root_nodes[i].readdir = NULL;
        root_nodes[i].finddir = NULL;
        root_nodes[i].ptr = root_nodes[i].impl = 0;

        filenames[i+2] = kmalloc(file_headers[i].namelen);
        strcpy(filenames[i+2], file_headers[i].name);
        filenames[i+2][file_headers[i].namelen] = 0;
    }

    return initrd_root;
}
