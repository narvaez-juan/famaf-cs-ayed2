#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "abb.h"

struct _s_abb {
    abb_elem elem;
    struct _s_abb *left;
    struct _s_abb *right;
};

static bool elem_eq(abb_elem a, abb_elem b) {
    return a == b;
}

static bool elem_less(abb_elem a, abb_elem b) {
    return a < b;
}

static bool invrep(abb tree) {
    bool inv = true;
    if (tree != NULL) {
        if (tree->left != NULL) {
            inv &= (elem_less(tree->left->elem, tree->elem));
        }
        if (tree->right != NULL) {
            inv &= (elem_less(tree->elem, tree->right->elem));
        }
        inv &= invrep(tree->left);
        inv &= invrep(tree->right);
    }
    return inv;
}

abb abb_empty(void) {
    abb tree = NULL;
    assert(invrep(tree) && abb_is_empty(tree));
    return tree;
}

abb abb_add(abb tree, abb_elem e) {
    assert(invrep(tree));
    if (abb_is_empty(tree)) 
    {
        tree = malloc(sizeof(struct _s_abb));
        assert(tree != NULL);
        tree->left = abb_empty();
        tree->right = abb_empty();
        tree->elem = e;
    } 
    else 
    {
        if (elem_less(e, tree->elem)) 
        {
            tree->left = abb_add(tree->left, e);
        } 
        else 
        {
            tree->right = abb_add(tree->right, e);
        }
    }
    assert(invrep(tree) && abb_exists(tree, e));
    return tree;
}

bool abb_is_empty(abb tree) {
    assert(invrep(tree));
    bool is_empty = false;
    is_empty = (tree == NULL);
    return is_empty;
}

bool abb_exists(abb tree, abb_elem e) {
    assert(invrep(tree));
    bool exists = false;
    if (!abb_is_empty(tree)) 
    {
        if (elem_eq(tree->elem, e)) {
            exists = true;
        } else if (elem_less(e, tree->elem)) {
            exists = abb_exists(tree->left, e);
        } else {    
            exists = abb_exists(tree->right, e);
        }
    }
    assert(abb_length(tree)!=0 || !exists);
    return exists;
}

unsigned int abb_length(abb tree) {
    assert(invrep(tree));
    unsigned int length = 0u;
    if (!abb_is_empty(tree)) {
        length += 1 + abb_length(tree->left) + abb_length(tree->right);
    } 
    assert(invrep(tree) && (abb_is_empty(tree) || length > 0u));
    return length;
}

abb abb_remove(abb tree, abb_elem e) {
    assert(invrep(tree));
    if (!abb_is_empty(tree)) {
        if (elem_less(e, tree->elem)) {
            tree->left = abb_remove(tree->left, e);
        } else if (elem_less(tree->elem, e)) {
            tree->right = abb_remove(tree->right, e);
        } else {
            abb tmp = NULL;
            if (tree->left == NULL) {
                tmp = tree;
                tree = tree->right;
                free(tmp);
            } else if (tree->right == NULL) {
                tmp = tree;
                tree = tree->left;
                free(tmp);
            } else {
                tmp = tree->right;
                while (tmp->left != NULL) {
                    tmp = tmp->left;
                }
                tree->elem = tmp->elem;
                tree->right = abb_remove(tree->right, tmp->elem);
            }
        }
    }
    assert(invrep(tree) && !abb_exists(tree, e));
    return tree;
}

abb_elem abb_root(abb tree) {
    abb_elem root;
    assert(invrep(tree) && !abb_is_empty(tree));
    root = tree->elem;
    assert(abb_exists(tree, root));
    return root;
}

abb_elem abb_max(abb tree) {
    abb_elem max_e;
    assert(invrep(tree) && !abb_is_empty(tree));
    max_e = abb_root(tree);
    if (!abb_is_empty(tree->right)) 
    {
        max_e = abb_max(tree->right); 
    }
    assert(invrep(tree) && abb_exists(tree, max_e));
    return max_e;
}

abb_elem abb_min(abb tree) {
    abb_elem min_e;
    assert(invrep(tree) && !abb_is_empty(tree));
    min_e = abb_root(tree);
    if (!abb_is_empty(tree->left)) 
    {
        min_e = abb_min(tree->left); 
    }
    assert(invrep(tree) && abb_exists(tree, min_e));
    return min_e;
}

void abb_dump(abb tree, abb_ordtype ord) {
    assert(invrep(tree) && (ord==ABB_IN_ORDER  || 
                            ord==ABB_PRE_ORDER ||
                            ord==ABB_POST_ORDER));
    
    if (!abb_is_empty(tree)) {
        if (ord == ABB_PRE_ORDER) {
            printf("%d ", tree->elem);
            abb_dump(tree->left, ord);
            abb_dump(tree->right, ord);
        } else if (ord == ABB_POST_ORDER) {
            abb_dump(tree->left, ord);
            abb_dump(tree->right, ord);
            printf("%d ", tree->elem); 
        } else {
            abb_dump(tree->left, ord);
            printf("%d ", tree->elem); 
            abb_dump(tree->right, ord);
        }
    }
}

abb abb_destroy(abb tree) {
    assert(invrep(tree));
    if (!abb_is_empty(tree)) {
        tree->left = abb_destroy(tree->left);
        tree->right = abb_destroy(tree->right);
        free(tree);
        tree = NULL;
    }
    assert(tree == NULL);
    return tree;
}
