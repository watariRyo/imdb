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

Leaf *find_last_linear(Node *parent) {
    Leaf *l;

    errno = NoError;
    assert(parent);

    if (!parent->leaf)
        reterr(NoError);

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

int main() {
    Node *n, *n2;
    Leaf *l1, *l2;
    int8 *key, *value;
    int16 size;

    n = create_node((Node *)&root, (int8 *)"/Users");
    assert(n);
    n2 = create_node(n, (int8 *)"/Users/login");
    assert(n2);

    key = (int8 *)"hoge";
    value = (int8 *)"abc1234aa";
    size = (int16)strlen((char *)value);
    l1 = create_leaf(n2, key, value, size);
    assert(l1);

    printf("%s\n", l1->value);

    key = (int8 *)"foo";
    value = (int8 *)"xyz1234xx";
    size = (int16)strlen((char *)value);
    l2 = create_leaf(n2, key, value, size);
    assert(l2);

    printf("%s\n", l2->key);

    printf("%p %p\n", n, n2);
    free(n2);
    free(n);

    return 0;
}