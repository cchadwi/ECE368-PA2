#ifndef BUILD_H
#define BUILD_H
Tnode* node_insert(int key, Tnode* root);

Tnode* node_delete(int val, Tnode* root);

Tnode* rotate_left(Tnode* root);

Tnode* rotate_right(Tnode* root);

Tnode* tree_balance(Tnode* ya, Tnode* new);

void preorder(Tnode* root, FILE* fptr);
#endif