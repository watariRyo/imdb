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

#pragma GCC diagnostic pop