/* htable.c */
#include "htable.h"

void zero(int8 *str, int16 size) {
    int8 *p;
    int16 n;

    for (n=0, p=str; n < size ;p++, n++) 
        *p = 0;

    return;
}

int16 Example_genhashes(int8 *infile, int8 *outfile) {
    int16 n, size;
    FILE *in;
    int out;
    Hash h;
    int8 buf[128];

    in = fopen((char *)infile, "r");
    out = open((char *)outfile, O_WRONLY|O_CREAT, 00644);

    assert(in && out);
    n = 0;
    while (fgets((char *)buf, 127, in)) {
        size = (int16)strlen((char *)buf);
        if (size < 1) {
            zero(buf, 128);
            continue;
        }
        *(buf + --size) = 0;
        h = hash(buf, size);
        dprintf(out, "0x%.05x %s\n", (unsigned int)h.hash, buf);

        n++;
        zero(buf, 128);
    }

    close(out);
    fclose(in);

    return n;
}

Hash hash_native(int8 *input, int16 size) {
    Hash hash;
    int16 x;
    int8 *p;

    hash.hash = 0;
    printf("input %s", input);
    for (p=input, x=0; x<size; p++, x++)
        hash.hash += *p;

    return hash;
}


int main(int argc, char *argv[]) {
    int8 *infile, *outfile;
    int16 x;

    if (argc < 3) {
        fprintf(stderr, "usage: %s INFILe OURDILE\n", *argv);
        return -1;
    }

    infile = (int8 *)argv[1];
    outfile = (int8 *)argv[2];
    printf("Generating hasees...\n");
    fflush(stdout);

    x = Example_genhashes(infile, outfile);
    printf("%d\n", x);

    return 0;
}