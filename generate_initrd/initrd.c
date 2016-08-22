#include "initrd.h"

int main(int argc, char** argv){
    int n=argc-1;
    int i;

    initrd_file_header_t* file_headers = malloc(n*sizeof(initrd_file_header_t));
    initrd_header_t* header = malloc(sizeof(initrd_header_t));

    uint32_t off = sizeof(initrd_header_t)+n*sizeof(initrd_file_header_t);

    header->magic = INITRD_MAGIC;
    header->numfiles = n;
    for(i=0;i<n;i++){
        int len = strlen(argv[i+1]);
        strcpy(file_headers[i].name, argv[i+1]);
        file_headers[i].magic = INITRD_MAGIC;
        file_headers[i].offset = off;

        FILE* stream = fopen(argv[i+1], "r");
        fseek(stream, 0, SEEK_END);
        int filelen = ftell(stream);
        file_headers[i].size = filelen;
        file_headers[i].namelen = len;

        off += filelen;
        fclose(stream);
    }

    FILE* out = fopen("./initrd.img", "w");
    fwrite(header, sizeof(initrd_header_t), 1, out);
    fwrite(file_headers, sizeof(initrd_file_header_t), n, out);

    for(i=0; i<n; i++){
        FILE* stream = fopen(argv[i+1], "r");
        unsigned char* buf = malloc(file_headers[i].size);
        
        fread(buf, 1, file_headers[i].size, stream);
        fwrite(buf, 1, file_headers[i].size, out);

        int off = ftell(out);
        printf("writing file %s at offset %d\n", argv[i+1], off);

        fclose(stream);
        free(buf);
    }

    fclose(out);

    return 0;
}
