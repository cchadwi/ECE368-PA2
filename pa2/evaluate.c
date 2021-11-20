#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "hbt.h"

Tnode* node_construct(int key) 
{
    Tnode* p = malloc(sizeof(Tnode));
    if (p == NULL) 
    {
        fprintf(stderr, "fail to malloc a Tnode\n");
        return NULL;
    }
  // initialize the node's attributes.
    p -> left = NULL;
    p -> right = NULL;
    p -> balance = 0;
    p -> key = key;
    return p;
}

Tnode* tree_build(int* keylist, char* children, int* ind, int ub) {
  if (*ind > ub) {
    // reach the end of the array.
    return NULL;
  }
  Tnode* p = node_construct(keylist[*ind]);
  char child = children[*ind];
  (*ind) ++;
  if (child == 3) {
    // this node has both children.
    p -> left = tree_build(keylist, children, ind, ub);
    p -> right = tree_build(keylist, children, ind, ub);
    return p;
  }
  if (child == 2) {
    // this node has only left child.
    p -> left = tree_build(keylist, children, ind, ub);
    return p;
  }
  if (child == 1) {
    // this node only has left child.
    p -> right = tree_build(keylist, children, ind, ub);
    return p;
  }
  // this node does not have any child.
  return p;
}

int balance_cal(Tnode* root) {
  // use post-order to calculate the height.
  if (root == NULL) {
    return -1;
  }
  int lh = balance_cal(root -> left);
  int rh = balance_cal(root -> right);
  int bal = lh - rh;
  root -> balance = bal;
  // the height is the maximum value among lh and rh
  return(lh >= rh) ? (lh + 1) : (rh + 1);
}

bool BST_check(Tnode* root) {
  if (root == NULL) {
    return true;
  }
  Tnode* lc = root -> left;
  Tnode* rc = root -> right;
  // check if this node satisfy the BST.
  if ((lc != NULL) && (lc -> key > root -> key)) {
    return false;
  }
  if ((rc != NULL) && (rc -> key < root -> key)) {
    return false;
  }
  bool lrtv = BST_check(root -> left);
  bool rrtv = BST_check(root -> right);
  return (lrtv && rrtv);
}

bool balance_check(Tnode* root) {
  if (root == NULL) {
    return true;
  }
  char bal = root -> balance;
  if ((bal < -1) || (bal > 1)) {
    return false;
  }
  char lb = balance_check(root -> left);
  char rb = balance_check(root -> right);
  return (lb && rb);
}

void read_input(FILE* fptr, int* keylist, char* child, int size) {
  int ind = 0;
  while (ind < size) {
    if (fread(&keylist[ind], sizeof(int), 1, fptr) != 1) {
      fprintf(stderr, "fail to read key. \n");
      return;
    }
    if (fread(&child[ind], sizeof(char), 1, fptr) != 1) {
      fprintf(stderr, "fail to read child info. \n");
      return;
    }
    ind ++;
  }
}

void tree_destroy(Tnode* root) {
  if (root == NULL) {
    return;
  }
  tree_destroy(root -> left);
  tree_destroy(root -> right);
  free(root);
}