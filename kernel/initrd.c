#include <kernel/initrd.h>

initrd_header_t* header;
initrd_file_header_t* file_headers;

fs_node_t* initrd_root;
fs_node_t* initrd_dev;

fs_node_t* root_nodes;
nt nroot_nodes;

fs_node_t* initrd_parse(mb_module_t* mod){
    return NULL;
}