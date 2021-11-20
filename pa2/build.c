#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hbt.h"
#include "evaluate.h"
#include "build.h"

Tnode* rotate_l(Tnode* root) 
{
    if (root == NULL) 
    {
        return NULL;
    }
    Tnode* rc = root -> right;
    Tnode* temp = rc -> left;
    rc -> left = root;
    root -> right = temp;
    return rc;
}

Tnode* rotate_r(Tnode* root) 
{
    if (root == NULL)  
    {
        return NULL;
    }
    Tnode* lc = root -> left;
    Tnode* temp = lc -> right;
    lc -> right = root;
    root -> left = temp;
    return lc;
}

Tnode* tree_balance(Tnode* ya, Tnode* new) 
{
    int key = new -> key;
    Tnode* child = (key <= ya -> key) ? ya -> left:ya -> right;
    Tnode* root = NULL;
    Tnode* curr = ya;
    if ((ya -> balance == 2) && (child -> balance == 1)) 
    {
        // child is B. ya is A. (in lec notes)
        root = rotate_r(ya);
        ya -> balance = 0;
        child -> balance = 0;
        return root;
    }
    else if ((ya -> balance == -2) && (child -> balance == -1)) 
    {
        // same with case 1.
        root = rotate_l(ya);
        ya -> balance = 0;
        child -> balance = 0;
        return root;
    }
    else if ((ya -> balance == 2) && (child -> balance == -1))
    {
        // case 2
        curr = child -> right; // curr is C. ya is A
        ya -> left = rotate_l(child);
        root = rotate_r(ya);
        if (curr == new) 
        {
            // case 2c: curr is new node.
            ya -> balance = 0;
            child -> balance = 0;
        }
        else 
        {
            // curr -> balance == 1 case 2a
            if (curr -> balance == 1) 
            {
                ya -> balance = -1;
                child -> balance = 0;
            }
            else 
            {
                // curr -> balance  == -1: case 2b
                ya -> balance = 0;
                child -> balance = 1;
            }
            curr -> balance = 0;
        }
    }
    else if ((ya -> balance == -2) && (child -> balance == 1)) 
    {
        curr = child -> left;
        ya -> right = rotate_r(child);
        root = rotate_l(ya);
        if (curr -> balance == 0) 
        { // same with case 2c.
            ya -> balance = 0;
            child -> balance = 0;
        }
        else 
        {
            if (curr -> balance == -1) 
            {
                ya -> balance = 1;
                child -> balance = 0;
            }
            else 
            {
                ya -> balance = 0;
                child -> balance = -1;
            }
      curr -> balance = 0;
    }
  }
  #ifdef DEBUG
  preorder(ya, NULL);
  fprintf(stderr, "\n");
  #endif
  return curr;
}

Tnode* node_insert(int key, Tnode* root) {
  Tnode dummy = {
    .key = 0,
    .balance = 0,
    .left = root,
    .right = root,
  };
  Tnode* prev = NULL; // parrent
  Tnode* curr = root; // current.
  Tnode* ya = curr;
  Tnode* pya = &dummy;
  Tnode* q = NULL;
  while (curr != NULL) {
    if (key > curr -> key) {
      q = curr -> right;
    }
    else {
      q = curr -> left;
    }
    // keep record of youngest ancesstor.
    if ((q != NULL) && (q -> balance != 0)) {
      pya = curr;
      ya = q;
    }
    prev = curr;
    curr = q;
  }
  q = node_construct(key);
  if (root == NULL) {
    return q;
  }
  if (key <= prev -> key) {
    prev -> left = q;
  }
  else {
    prev -> right = q;
  }
  #ifdef DEBUG
  preorder(root, NULL);
  fprintf(stderr, "\n");
  #endif
  // update the balance.
  curr = ya;
  while (curr != q) {
    if (key <= curr -> key) {
      curr -> balance ++;
      curr = curr -> left;
    }
    else {
      curr -> balance --;
      curr = curr -> right;
    }
  }
  // check balance.
  if (key <= pya -> key) {
    pya -> left = tree_balance(ya, q);
    return dummy.left;
  }
  else {
    pya -> right = tree_balance(ya, q);
    #ifdef DEBUG
    preorder(root, NULL);
    fprintf(stderr, "\n");
    #endif
    return dummy.right;
  }
}

Tnode* node_delete(int val, Tnode* root) {
  if (root == NULL) {
    return NULL;
  }
  if (val < root -> key) {
    root -> left = node_delete(val, root -> left);
  }
  else if (val > root -> key) {
    root -> right = node_delete(val, root -> right);
  }
  // now val == root -> key.
  else {
    // root has no child.
    if ((root -> left == NULL) && (root -> right == NULL)) {
      free(root);
      return NULL;
    }
    if (root -> left == NULL) {
      // there must be right child.
      Tnode* rc = root -> right;
      free(root);
      return rc;
    }
    if (root -> right == NULL) {
      // must have left child.
      Tnode* lc = root -> left;
      free(root);
      return lc;
    }
    // root has both left and right child.
    // find the largest one in left sub-tree.
    Tnode* p = root -> left;
    while (p -> right != NULL) {
      p = p -> right;
    }
    // pass its value to root and delete it.
    root -> key = p -> key;
    p -> key = val;
    root -> left = node_delete(val, root -> left);
  }
  // update height and balance.
  // may need balance at each level.
  balance_cal(root);
  int bal = root -> balance;
  int lb = (root -> left == NULL) ? 0: root -> left -> balance;
  int rb = (root -> right == NULL) ? 0: root -> right -> balance;
  if ((bal > 1) && (lb >= 0)) {
    return rotate_r(root);
  }
  if ((bal < -1) && (rb <= 0)) {
    return rotate_l(root);
  }
  if ((bal > 1) && (lb < 0)) {
    root -> left = rotate_l(root -> left);
    return rotate_r(root);
  }
  if ((bal < -1) && (rb > 0)) {
    root -> right = rotate_r(root -> right);
    return rotate_l(root);
  }
  return root;
}

void preorder(Tnode* root, FILE* fptr) {
  if (root == NULL) {
    return;
  }
  #ifndef DEBUG
  if (fptr == NULL) {
    return;
  }
  #endif
  Tnode* lc = root -> left;
  Tnode* rc = root -> right;
  char child = ((lc == NULL) && (rc == NULL)) ? 0 :
              (lc == NULL) ? 1 : (rc == NULL) ? 2 : 3;
  #ifndef DEBUG
  int rtv = 0;
  rtv = fwrite(&(root -> key), sizeof(int), 1, fptr);
  rtv = fwrite(&child, sizeof(char), 1, fptr);
  if (rtv != 1) {
    fprintf(stderr, "fwrite fails\n");
    return;
  }
  #endif
  //fprintf(stderr, "%d %d\n", root -> key, (int)child);
  preorder(root -> left, fptr);
  preorder(root -> right, fptr);
}