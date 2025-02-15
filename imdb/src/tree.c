#include "tree.h"

Tree root = { 
    .n = {
        .tag = (TagRoot | TagNode),
        .up = (Node *)&root,
        .next = 0,
        .leaf = 0,
        .path = { '/' }
    }
};

/* 
struct s_node {
    Tag tag;
    struct s_node *up;
    struct s_node *next;
    struct s_leaf *leaf;
    int8 path[256];
};
*/

void print_tree(int fd, Tree *_root) {
    int8 indentation;
    int8 buf[256];
    int16 size;
    Node *n;
    Leaf *l, *first;

    indentation = 0;
    for (n = (Node *)_root; n; n = n->next) {
        Print(indent(indentation++));
        Print(n->path);
        Print("\n");
        if (n->leaf) {
            first = n->leaf;
            if (first)
                for (l = first; l; l = (Leaf *)l->leaf) {
                    Print(indent(indentation));
                    Print(n->path); 
                    Print("/");              
                    Print(l->key);
                    Print(" -> '");
                    write(fd, (char *)l->value, (int)l->size);
                    Print("'\n");
                }
        }
    }

    return;
}

int8 *indent(int16 n) {
    int16 i;
    static int8 buf[256];
    int8 *p;

    if (n < 1)
        return (int8 *)"";
    assert((n < 120));
    zero(buf, 256);

    for (i=0, p=buf; i < n; i++, p+=2)
        strncpy((char *)p, "  ", 2);

    return buf;
}

// 初期化
void zero(int8 *str, int16 size) {
    int8 *p;
    int16 n;

    for (n=0, p=str; n < size ;p++, n++) 
        *p = 0;

    

    return;
}

Node *create_node(Node *parent, int8 *path) {
    Node *n;
    int16 size;

    errno = NoError;
    assert(parent);
    size = sizeof(struct s_node);
    n = (Node *)malloc((int)size);
    zero((int8 *)n, size);

    parent->next = n;
    n->tag = TagNode;
    n->up = parent;
    strncpy((char *)n->path, (char *)path, 255);

    return n;
}


Node *finde_node_linear(int8 *path) {
    Node *p, *ret;

    for (ret=(Node *)0, p = (Node *)&root; p; p = p->next)
        if (!strcmp((char *)p->path, (char *)path)) {
            ret = p;
            break;
        }

    return ret;
}

Leaf *find_leaf_linear(int8 *path, int8 *key) {
    Node *n;
    Leaf *l, *ret;

    n = find_node(path);
    if (!n)
        return (Leaf *)0;

    for (ret=(Leaf *)0, l=n->leaf; l; l=l->leaf)
        if (!strcmp((char *)l->key, (char *)key)) {
            ret = l;
            break;
        }

    return ret;
}

int8 *lookup_linear(int8 *path, int8 *key) {
    Leaf *p;

    p = find_leaf_linear(path, key);

    return (p) ? p->value : (int8 *)0;
}

Leaf *find_last_linear(Node *parent) {
    Leaf *l;

    errno = NoError;
    assert(parent);

    if (!parent->leaf)
        return (Leaf *)0;

    for (l = parent->leaf; l->leaf; l = l->leaf) 
        assert(l);
    
    return l;
}

Leaf *create_leaf(Node *parent, int8 *key, int8 *value, int16 count) {
    Leaf *l, *new;
    int16 size;

    assert(parent);
    l = find_last(parent);

    size = sizeof(struct s_leaf);
    new = (Leaf *)malloc((int)size);
    assert(new);

    if (!l) {
        // direct connect
        parent->leaf = new;
    } else {
        // leaf
        l->leaf = new;
    }

    zero((int8 *)new, size);
    new->tag = TagLeaf;
    new->tree = (l) ? (Tree *)l : (Tree *)parent;

    strncpy((char *)new->key, (char *)key, 127);

    new->value = (int8 *)malloc(count + 1);
    zero(new->value, count + 1);
    assert(new->value);
    strncpy((char *)new->value, (char *)value, count);
    new->value[count] = '\0';
    new->size = count;

    return new;
}

Tree *example_tree() {
    int8 c;
    Node *n, *p;
    int8 path[256];
    int32 x;

    zero(path, 256);
    x = 0;

    for (n = (Node *)&root, c='a'; c <= 'z'; c++) {
        x = (int32)strlen((char *)path);
        *(path + x++) = '/';
        *(path + x) = c;

        p = n;
        n = create_node(p, path);
    }

    return (Tree *)&root;
}

int8 *example_path(int8 path) {
    int32 x;
    static int8 buf[256];
    int8 c;

    zero(buf, 256);
    for (c = 'a'; c <= path; c++) {
        x = (int32)strlen((char *)buf);
        *(buf + x++) = '/';
        *(buf + x) = c;
    }

    return buf;
}

int8 *example_duplicate(int8 *str) {
    int16 n, x;
    static int8 buf[256];

    zero(buf, 256);
    strncpy((char *)buf, (char *)str, 255);
    n = (int8)strlen((char *)buf);
    x = (n*2);
    if (x > 254)
        return buf;
    else
        strncpy((char *)buf+n, strdup((char *)buf), 255);

    return buf;
}

int32 example_leaves() {
    // int fd;
    FILE *fd;
    int32 x, y;
    int8 buf[256];
    int8 *path, *val;
    // Leaf *l;
    Node *n;
    // int8 c;s

    // fd = open(ExampleFile, O_RDONLY);
    fd = fopen(ExampleFile, "r");
    assert(fd);

    zero(buf, 256);
    y = 0;
    while (fgets((char *)buf, 255, fd)) {
        x = (int32)strlen((char *)buf);
        *(buf+x-1) = 0;
        // buf[*(buf+x-1)] = 0;
        path = example_path(*buf);
        n = find_node(path);
        if (!n) {
            zero(buf, 256);
            continue;
        }

        val=example_duplicate(buf);
#if 0
        printf("\n");
        printf("node = '%s'\n", n->path);
        printf("buf = '%s'\n", buf);
        printf("val = '%s'\n", val);
        printf("len = '%d'\n", (int16)strlen((char *)val));
        printf("\n");
#endif
        // l = create_leaf(n, buf, val, (int16)strlen((char *)val));
        create_leaf(n, buf, val, (int16)strlen((char *)val));
        y++;
        zero(buf, 256);
    }

    fclose(fd);

    return y;
}

int32 example_searches(int8 *file) {
    FILE *fd;
    int8 buf[64];
    int8 *path, *value;
    int16 size;
    int32 n;

    fd = fopen((char *)file, "r");
    assert(fd);

    zero(buf, 64);
    n = 0;
    while (fgets((char *)buf, 63, fd)) {
        size = (int16)strlen((char *)buf);
        assert(size);
        size--; // 改行削除
        buf[size] = 0;

        path = example_path(*buf);
        value = lookup(path, buf);
        if (value) {
            printf("%s -> '%s'\n", buf, value);
            fflush(stdout);
            n++;
        }
        zero(buf, 64);
    }
    fclose(fd);
    return n;
}

int main(int argc, char *argv[]) {
    Tree *example;
    int32 x, y;
    // int8 *p;
    struct rusage usage;
    float duration;
    int8 *file;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s < <INFILE>\n", *argv);
        return -1;
    } else
        file = (int8 *)argv[1];


    example = example_tree();
    x = example_leaves();
    (void)x;
    (void)example;

    if (fork()) {
        wait(0);
    } else {
        example_searches(file);
        exit(0);
    }
    // print_tree(1, example);

    y = (int32)getrusage(RUSAGE_CHILDREN, &usage);
    if (y)
        perror("getrusage()");
    else {
        duration = 0;
        duration = (float)usage.ru_utime.tv_usec;
        duration /= (float)1000000.0;
        duration += (float)usage.ru_utime.tv_sec;
        // duration = usage.ru_utime.tv_sec + (usage.ru_utime.tv_usec / 1000000);

        printf("Duration: %.04f\n", duration);
        printf("%lu\n", usage.ru_utime.tv_usec);
    }

    return 0;
}

#pragma GCC diagnostic pop