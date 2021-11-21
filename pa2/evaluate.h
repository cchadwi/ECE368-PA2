#ifndef __EVALUATE_H
#define __EVALUATE_H
#include <stdbool.h>

Tnode* node_construct(int key);

Tnode* tree_build(int* keylist, char* child, int* ind, int ub);

int balance_cal(Tnode* root);

bool balance_check(Tnode* root);

bool BST_check(Tnode* root);

void read_input(FILE* fptr, int* keylist, char* child, int size);

void tree_destroy(Tnode* root);

#endif