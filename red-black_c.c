#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef long long int lli;

void* allocate(size_t size) {
    void *p = malloc(size);
    if (p == NULL) {
        abort();
    } else {
        return p;
    }
}

void whitespace(FILE *inf) {
    char c;
    while ((c = getc(inf)), c == ' ' || c == '\t' || c == '\n');
    if (c != EOF) ungetc(c, inf);
}

// General Red-Black trees
typedef int branch;
typedef lli tkey;
typedef lli tvalue;

struct rbtree {
    tkey key;
    tvalue val;
    struct rbtree **child;
    struct rbtree *parent;
    bool color; // 0 - black, 1 - red
};
typedef struct rbtree* rbtree;

rbtree new_node(tkey key, tvalue val) {
    rbtree v = (rbtree) allocate(sizeof(struct rbtree));

    v->key = key;
    v->val = val;
    v->child = (rbtree *) allocate(2 * sizeof(rbtree));
    v->child[0] = NULL; // Left
    v->child[1] = NULL; // Right
    v->parent = NULL;
    v->color = 0;

    return v;
}

void destroy_node(rbtree node) {
    free(node->child);
    free(node);
}

void clear(rbtree root) {
    if (root != NULL) {
        clear(root->child[0]);
        clear(root->child[1]);
        destroy_node(root);
    }
}

void swap_content(rbtree a, rbtree b) {
    tkey k;
    k = a->key;
    a->key = b->key;
    b->key = k;

    tvalue v;
    v = a->val;
    a->val = b->val;
    b->val = v;
}

/*void print_r(rbtree root, int spaces) {
    if (root != NULL) {
        print_r(root->child[1], spaces + 2);
        for (int i = 0; i < spaces; i++) {
            printf(" ");
        }
        printf("%d ", root->key);
        if (root->color) printf("red\n");
        else printf("black\n");
        print_r(root->child[0], spaces + 2);
    }
}

void print(rbtree root) {
    print_r(root, 0);
    printf("\n");
}*/

rbtree find_node(rbtree node, tkey key) {
    if (node == NULL) return node;

    branch dir;
    while (node->key != key) {
        dir = node->key < key;
        if (node->child[dir] == NULL) return node;
        else node = node->child[dir];
    }

    return node;
}

branch son(rbtree a, rbtree b) { // Assumes that b IS a son of a
    if (a->child[0] == b) return 0;
    else return 1;
}

void rotate(rbtree root, bool dir) { // 0 - right, 1 - left
    rbtree a, b;
    a = root->child[dir];
    b = root;

    b->child[dir] = a->child[1 - dir];
    if (b->child[dir] != NULL) b->child[dir]->parent = b;

    a->child[1 - dir] = b;
    a->parent = b->parent;
    if (a->parent != NULL) a->parent->child[son(b->parent, b)] = a;
    b->parent = a;
}

void rebuild_on_insert(rbtree node, branch c) { // Rebuild node with its node->child[c]
    while ((node != NULL) && node->color && node->child[c]->color) { // Rebuilding needed only if both are red; parent is not NULL because of 5'th req
        rbtree parent = node->parent;
        branch c_anc = son(parent, node); // Ancestor child direction
        rbtree uncle = parent->child[1 - c_anc];

        if (uncle != NULL && uncle->color) { // If uncle is colored red
            parent->color = 1;
            node->color = 0;
            uncle->color = 0;

            node = parent->parent;
            if (node != NULL) c = son(node, parent); // Go up
            else parent->color = 0; // Reached the root
        } else {
            if (c == c_anc) { // If makes one line
                parent->color = 1;
                node->color = 0;

                rotate(parent, c);
            } else { // Zig-zag
                parent->color = 1;
                node->child[c]->color = 0;

                rotate(node, c);
                rotate(parent, 1 - c);
            }
            node = NULL; // Break from while
        }
    }
}

void rebuild_on_erase(rbtree parent, rbtree child, branch c, bool color) {
    bool next_step = 1;
    while (next_step) {
        next_step = 0;

        if (color == 0) { // Need to do something only if node is black
            if (parent == NULL) { // 1. No parent
                if (child != NULL) child->color = 0;
            } else { // 2. Parent exists
                if (child != NULL && child->color == 1) { // 2.1. Red child exists
                    child->color = 0;
                } else { // 2.2 Black or no children. Brother is guaranteed to exist
                    branch c2 = 1 - c;
                    rbtree bro = parent->child[c2];

                    // *** Cases 1 - 5 ***
                    if (bro->color == 0) {
                        if ((bro->child[0] == NULL || bro->child[0]->color == 0) &&
                            (bro->child[1] == NULL || bro->child[1]->color == 0)) {
                            if (parent->color == 1) { // Case 1
                                parent->color = 0;
                                bro->color = 1;
                            } else { // Case 5
                                bro->color = 1;

                                child = parent;
                                parent = child->parent;
                                if (parent != NULL) c = son(parent, child);

                                next_step = 1;
                            }
                        } else if (bro->child[c2] != NULL && bro->child[c2]->color == 1) { // Case 2
                            bro->child[c2]->color = 0; // Step 1: color black
                            bool tmpcol = bro->color; // Step 2: swap colors
                            bro->color = parent->color;
                            parent->color = tmpcol;
                            rotate(parent, c2); // Step 3: rotate
                        } else { // Case 3
                            bro->child[c]->color = 0; // Step 1.1: color black
                            bro->color = 1; // Step 1.2: color red
                            rotate(bro, c); // Step 1.3: rotate

                            bro = bro->parent;

                            bro->child[c2]->color = 0; // Step 2.1: color black
                            bool tmpcol = bro->color; // Step 2.2: swap colors
                            bro->color = parent->color;
                            parent->color = tmpcol;
                            rotate(parent, c2); // Step 2.3: rotate
                        }
                    } else { // Case 4
                        parent->color = 1 - parent->color;
                        bro->color = 1 - bro->color;
                        rotate(parent, c2);

                        next_step = 1;
                    }
                    // ***
                }
            }
        }
    }
}

void erase_node(rbtree *root, rbtree node) {
    if (node != NULL) {
        if (node->child[0] != NULL && node->child[1] != NULL) { // Both sons present
            rbtree left = node->child[0];
            while (left->child[1] != NULL) left = left->child[1];

            swap_content(left, node);
            erase_node(root, left);
        } else { // One child or no children
            // Prepare root restoring
            rbtree can1, can2; // New root candidates
            can1 = *root;
            if ((*root)->child[1] == NULL) can2 = (*root)->child[0];
            else can2 = (*root)->child[1];

            // Retie links
            branch parent_c = 0;
            bool color = node->color;

            rbtree parent = node->parent;
            bool node_c = (node->child[0] == NULL);

            rbtree child = node->child[node_c];
            if (child != NULL) child->parent = parent;
            if (parent != NULL) {
                parent_c = son(parent, node);
                parent->child[son(parent, node)] = node->child[node_c];
            }

            // Destroy and fix
            destroy_node(node);
            rebuild_on_erase(parent, child, parent_c, color);

            // Restore correct root
            if (can1 == node) can1 = can2;
            if (can1 == NULL) {
                *root = NULL;
            } else {
                while (can1->parent != NULL) can1 = can1->parent;
                *root = can1;
            }
        }
    }
}

// Public handles

bool find(rbtree node, tkey key) {
    node = find_node(node, key);

    if (node == NULL) return 0;
    else if (node->key == key) return 1;
    else return 0;
}

bool insert(rbtree *root, tkey key, tvalue val) {
    if (*root == NULL) {
        *root = new_node(key, val); // Created new root
        return 0;
    }

    rbtree node = find_node(*root, key); // Find a child
    if (node->key == key) {
        node->val = val;
        return 1;
    } else {
        branch dir = key > node->key;

        node->child[dir] = new_node(key, val);
        node->child[dir]->parent = node;
        node->child[dir]->color = 1;

        rebuild_on_insert(node, dir);
        while ((*root)->parent != NULL) (*root) = (*root)->parent;

        return 0;
    }
}

bool erase(rbtree *root, tkey key) {
    rbtree node = find_node(*root, key);
    if (node == NULL || node->key != key) {
        return 0;
    } else {
        erase_node(root, node);
        return 1;
    }
}


int main(void) {
    rbtree T = NULL;

    char op = 'A';
    lli a, b;
    while (1) {
        whitespace(stdin);
        scanf("%c", &op);
        if (op == 'A') {
            scanf("%lld %lld", &a, &b);
            insert(&T, a, b);
        } else if (op == 'D') {
            scanf("%lld", &a);
            erase(&T, a);
        } else if (op == 'S') {
            scanf("%lld", &a);
            rbtree V = find_node(T, a);
            if (V != NULL && V->key == a) {
                printf("%lld %lld\n", V->key, V->val);
            }
        } else {
            break;
        }
    }

    clear(T);

    return 0;
}
