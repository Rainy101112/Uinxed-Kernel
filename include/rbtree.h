#ifndef INCLUDE_RBTREE_H_
#define INCLUDE_RBTREE_H_

typedef struct rb_node {
        int key, color; // 0:red 1:black
        struct rb_node *lchild, *rchild;
} rb_node;

void rbtree_init_NIL();
rb_node *rbtree_new_node(int val);
rb_node *rbtree_left_rotate(rb_node *root);
rb_node *rbtree_right_rotate(rb_node *root);
rb_node *rbtree_insert(rb_node *root, int val);
rb_node *rbtree_erase(rb_node *root, int val);
void rbtree_clear(rb_node *root);

#endif // INCLUDE_RBTREE_H_
