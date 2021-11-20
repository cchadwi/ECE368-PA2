#ifndef BUILD_H
#define BUILD_H
Tnode* node_insert(int key, Tnode* root);

Tnode* node_delete(int val, Tnode* root);

Tnode* rotate_l(Tnode* root);

Tnode* rotate_r(Tnode* root);

Tnode* tree_balance(Tnode* ya, Tnode* new);

void preorder(Tnode* root, FILE* fptr);
#endif