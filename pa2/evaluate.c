#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "hbt.h"

Tnode *node_construct(int key)
{
    Tnode *p = malloc(sizeof(Tnode));
    if (p == NULL)
    {
        fprintf(stderr, "fail to malloc a Tnode\n");
        return NULL;
    }

    p->right = NULL;
    p->left = NULL;
    p->balance = 0;
    p->key = key;
    return p;
}

Tnode *tree_build(int *keylist, char *child, int *ind, int ub)
{
    if (*ind > ub)
    {
        return NULL;
    }

    Tnode *p = node_construct(keylist[*ind]);
    char c = child[*ind];
    (*ind)++;
    if (c == 3)
    {
        // both children
        p->left = tree_build(keylist, child, ind, ub);
        p->right = tree_build(keylist, child, ind, ub);
        return p;
    }

    if (c == 2)
    {
        // left child
        p->left = tree_build(keylist, child, ind, ub);
        return p;
    }

    if (c == 1)
    {
        // left child
        p->right = tree_build(keylist, child, ind, ub);
        return p;
    }

    return p;
}

int balance_cal(Tnode *root)
{
    // calc height using post order
    if (root == NULL)
    {
        return -1;
    }

    int lh = balance_cal(root->left);
    int rh = balance_cal(root->right);
    int bal = lh - rh;
    root->balance = bal;
    // height = max val
    return (lh >= rh) ? (lh + 1) : (rh + 1);
}

bool BST_check(Tnode *root)
{
    if (root == NULL)
    {
        return true;
    }
    
    Tnode *lc = root->left;
    Tnode *rc = root->right;
    if ((lc != NULL) && (lc->key > root->key)) // check if this node satisfy the BST
    {
        return false;
    }

    if ((rc != NULL) && (rc->key < root->key))
    {
        return false;
    }

    bool lrtv = BST_check(root->left);
    bool rrtv = BST_check(root->right);
    return (lrtv && rrtv);
}

bool balance_check(Tnode *root)
{
    if (root == NULL)
    {
        return true;
    }
    char bal = root->balance; 
    if ((bal < -1) || (bal > 1)) //checks to see if bal is outside bounds
    {
        return false;
    }
    char lb = balance_check(root->left);
    char rb = balance_check(root->right);
    return (lb && rb);
}

void read_input(FILE *fptr, int *keylist, char *child, int size)
{
    int ind = 0;
    while (ind < size)
    {
        if (fread(&keylist[ind], sizeof(int), 1, fptr) != 1)
        {
            fprintf(stderr, "fail to read key. \n");
            return;
        }

        if (fread(&child[ind], sizeof(char), 1, fptr) != 1)
        {
            fprintf(stderr, "fail to read child info. \n");
            return;
        }
        
        ind++;
    }
}

void tree_destroy(Tnode *root)
{
    if (root == NULL)
    {
        return;
    }
    tree_destroy(root->left);
    tree_destroy(root->right);
    free(root);
}