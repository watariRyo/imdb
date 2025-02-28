/* htable.h */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <fcntl.h>
#ifdef __unix__
    #include <unistd.h>
#endif


typedef unsigned int int32;
typedef unsigned short int16;
typedef unsigned char int8;

#define hash(x, y)          hash_native(x,y)

struct s_hash {
    unsigned int hash: 21;
};
typedef struct s_hash Hash;

void zero(int8*, int16);
Hash hash_native(int8*, int16);
int main(int,char**);
int16 Example_genhashes(int8*, int8*);