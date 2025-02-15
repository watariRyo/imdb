#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/wait.h>

#define TagRoot     1 /* 00 01 */
#define TagNode     2 /* 00 10 */
#define TagLeaf     4 /* 01 00 */

#define NoError     0
#define ExampleFile  "/Users/ryota/CPP/imdb/imdb/src/wl.txt"
#define ExampleFile2 "/Users/ryota/CPP/imdb/imdb/src/wl.50k.txt"

typedef void* Nullptr;
Nullptr my_nullptr = 0;

#define find_last(x) find_last_linear(x)
#define find_leaf(x, y) find_leaf_linear(x, y)
#define lookup(x, y) lookup_linear(x, y)
#define find_node(x) finde_node_linear(x)
#define reterr(x) \
    errno = (x); \
    return my_nullptr;

#define Print(x) \
        zero(buf, 256); \
        strncpy((char *)buf, (char *)(x), 255); \
        size = (int16)strlen((char *)buf); \
        if (size) \
            write(fd, (char *)buf, size)

#pragma GCC diagnostic push

typedef unsigned int int32;
typedef unsigned short int16;
typedef unsigned char int8;
typedef unsigned char Tag;

struct s_node {
    Tag tag;
    struct s_node *up;
    struct s_node *next;
    struct s_leaf *leaf;
    int8 path[256];
};
typedef struct s_node Node;

struct s_leaf {
    Tag tag;
    union u_tree *tree;
    struct s_leaf *leaf;
    int8 key[128];
    int8 *value;
    int16 size;
};
typedef struct s_leaf Leaf;

union u_tree {
    Node n;
    Leaf l;
};
typedef union u_tree Tree;

void print_tree(int, Tree*);
int8 *indent(int16);
void zero(int8*, int16);
Leaf *find_leaf_linear(int8*, int8*);
int8 *lookup_linear(int8*, int8*);
Node *finde_node_linear(int8*);
Node *create_node(Node*, int8*);
Leaf *find_last_linear(Node*);
Leaf *create_leaf(Node*, int8*, int8*, int16);
int main(int, char**);
int32 example_searchs(int8*);
int8 *example_path(int8 c);
int32 example_leaves(void);
Tree *example_tree(void);
int8 *example_duplicate(int8 *str);

/*
    50k random searches(linearly)
    duration: 1.8866 sec

    1k random searches(linearly)
    duration: 0.0395 sec
*/

;