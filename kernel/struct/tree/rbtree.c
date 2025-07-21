#include "stdlib.h"

typedef struct rb_node {
        int key, color; // 0:red 1:black
        struct rb_node *lchild, *rchild;
} rb_node;

rb_node __NIL;
#define NIL (&__NIL)

__attribute__((constructor)) void init_NIL()
{
    NIL->key    = 0;
    NIL->color  = 1;
    NIL->lchild = NIL->rchild = NIL;
    return;
}

rb_node *get_new_rb_node(int val)
{
    rb_node *p = (rb_node *)malloc(sizeof(rb_node));
    p->key     = val;
    p->color   = 0;
    p->lchild = p->rchild = NIL;
    return p;
}

rb_node *left_rotate(rb_node *root)
{
    rb_node *temp = root->rchild;
    root->rchild  = temp->lchild;
    temp->lchild  = root;
    return temp;
}

rb_node *right_rotate(rb_node *root)
{
    rb_node *temp = root->lchild;
    root->lchild  = temp->rchild;
    temp->rchild  = root;
    return temp;
}

rb_node *insert_maintain(rb_node *root)
{
    if (!has_red_child(root)) return root;
    int flag = 0;
    if (root->lchild->color == 0 && root->rchild->color == 0) goto insert_end;
    if (root->lchild->color == 0 && has_red_child(root->lchild)) flag = 1;
    if (root->rchild->color == 0 && has_red_child(root->rchild)) flag = 2;
    if (flag == 0) return root;
    if (flag == 1) {
        if (root->lchild->rchild->color == 0) { root->lchild = left_rotate(root->lchild); }
        root = right_rotate(root);
    } else {
        if (root->rchild->lchild == 0) { root->rchild = right_rotate(root->rchild); }
        root = left_rotate(root);
    }
insert_end:
    root->color         = 0;
    root->lchild->color = root->rchild->color = 1;
    return root;
}

rb_node *__insert(rb_node *root, int val)
{
    if (root == NIL) return get_new_rb_node(val);
    if (root->key == val) return root;
    if (val < root->key)
        root->lchild = __insert(root->lchild, val);
    else
        root->rchild = __insert(root->rchild, val);
    return insert_maintain(root);
}

rb_node *insert(rb_node *root, int val)
{
    root        = __insert(root, val);
    root->color = 1;
    return root;
}

rb_node *predecessor(rb_node *root)
{
    rb_node *temp = root->lchild;
    while (temp->rchild != NIL) temp = temp->rchild;
    return temp;
}

rb_node *erase_maintain(rb_node *root)
{
    if (root->lchild->color != 2 && root->rchild->color != 2) return root;
    if (has_red_child(root)) {
        int flag    = 0;
        root->color = 0;
        if (root->lchild->color == 0) {
            root = right_rotate(root);
            flag = 1;
        } else {
            root = left_rotate(root);
            flag = 2;
        }
        root->color = 1;
        if (flag == 1)
            root->rchild = erase_maintain(root->rchild);
        else
            root->lchild = erase_maintain(root->lchild);
        return root;
    }
    if ((root->lchild->color == 2 && !has_red_child(root->rchild))
        || (root->rchild->color == 2 && !has_red_child(root->lchild))) {
        root->lchild->color -= 1;
        root->rchild->color -= 1;
        root->color += 1;
        return root;
    }
    if (root->lchild->color == 2) {
        if (root->rchild->rchild->color != 0) {
            root->rchild->color = 0;
            root->rchild        = right_rotate(root->rchild);
            root->rchild->color = 1;
        }
        root        = left_rotate(root);
        root->color = root->lchild->color;
    } else {
        if (root->lchild->lchild->color != 0) {
            root->lchild->color = 0;
            root->lchild        = left_rotate(root->lchild);
            root->lchild->color = 1;
        }
        root        = right_rotate(root);
        root->color = root->rchild->color;
    }
    root->lchild->color = root->rchild->color = 1;
    return root;
}

rb_node *__erase(rb_node *root, int val)
{
    if (root == NIL) return NIL;
    if (val < root->key) {
        root->lchild = __erase(root->lchild, val);
    } else if (val > root->key) {
        root->rchild = __erase(root->rchild, val);
    } else {
        if (root->lchild == NIL || root->rchild == NIL) {
            rb_node *temp = root->lchild != NIL ? root->lchild : root->rchild;
            temp->color += root->color;
            free(root);
            return temp;
        } else {
            rb_node *temp = predecessor(root);
            root->key     = temp->key;
            root->lchild  = __erase(root->lchild, temp->key);
        }
    }
    return erase_maintain(root);
}

rb_node *erase(rb_node *root, int val)
{
    root        = __erase(root, val);
    root->color = 1;
    return root;
}
