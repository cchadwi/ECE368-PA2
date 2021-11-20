#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hbt.h"
#include "evaluate.h"
#include "build.h"

Tnode *rotate_right(Tnode *root) // rotates tree right from the root
{
    if (root == NULL)
    {
        return NULL;
    }
    Tnode *lc = root->left;
    Tnode *temp = lc->right;
    lc->right = root;
    root->left = temp;
    return lc;
}

Tnode *rotate_left(Tnode *root) // rotates the tree left
{
    if (root == NULL)
    {
        return NULL;
    }
    // rotates root
    Tnode *rc = root->right;
    Tnode *temp = rc->left;
    rc->left = root;
    root->right = temp;
    return rc;
}

Tnode *tree_balance(Tnode *ya, Tnode *new) // ya is youngest ancestor
{
    int key = new->key;
    Tnode *child = (key <= ya->key) ? ya->left : ya->right;
    Tnode *root = NULL;
    Tnode *curr = ya;
    if ((ya->balance == 2) && (child->balance == 1)) // checks to see if there is a sibling node due to ya and that the child node has balance of 1
    {
        // case 1
        // got a lot of this from lec nots
        root = rotate_right(ya);
        ya->balance = 0;
        child->balance = 0;
        return root;
    }

    else if ((ya->balance == -2) && (child->balance == -1)) // left rotation needed to balance
    {
        root = rotate_left(ya);
        ya->balance = 0;
        child->balance = 0;
        return root;
    }

    else if ((ya->balance == 2) && (child->balance == -1))
    {
        // case 2
        curr = child->right;
        ya->left = rotate_left(child);
        root = rotate_right(ya);
        if (curr == new)
        {
            // curr is new node.
            ya->balance = 0;
            child->balance = 0;
        }

        else
        {
            // curr -> balance == 1 
            if (curr->balance == 1)
            {
                ya->balance = -1;
                child->balance = 0;
            }

            else
            {
                // curr -> balance  == -1
                ya->balance = 0;
                child->balance = 1;
            }
            curr->balance = 0;
        }
    }
    else if ((ya->balance == -2) && (child->balance == 1)) // need to rotate child right and ya left
    {
        curr = child->left;
        ya->right = rotate_right(child);
        root = rotate_left(ya);
        if (curr->balance == 0)
        {
            ya->balance = 0;
            child->balance = 0;
        }

        else
        {
            if (curr->balance == -1)
            {
                ya->balance = 1;
                child->balance = 0;
            }

            else
            {
                ya->balance = 0;
                child->balance = -1;
            }
            curr->balance = 0;
        }
    }
    #ifdef DEBUG
    preorder(ya, NULL);
    fprintf(stderr, "\n");
    #endif
    return curr;
}

Tnode *node_insert(int key, Tnode *root)
{
    Tnode dummy = {
            .key = 0,
            .balance = 0,
            .left = root,
            .right = root,
        };
    Tnode *prev = NULL; 
    Tnode *curr = root; 
    Tnode *ya = curr;
    Tnode *pya = &dummy;
    Tnode *q = NULL;
    while (curr != NULL)
    {
        if (key > curr->key)
        {
            q = curr->right;
        }

        else
        {
            q = curr->left;
        }

        // keep record of ya
        if ((q != NULL) && (q->balance != 0))
        {
            pya = curr;
            ya = q;
        }
        prev = curr;
        curr = q;
    }
    q = node_build(key);
    if (root == NULL)
    {
        return q;
    }

    if (key <= prev->key)
    {
        prev->left = q;
    }

    else
    {
        prev->right = q;
    }

    #ifdef DEBUG
    preorder(root, NULL);
    fprintf(stderr, "\n");
    #endif
    // update
    curr = ya;
    while (curr != q)
    {
        if (key <= curr->key)
        {
            curr->balance++;
            curr = curr->left;
        }

        else
        {
            curr->balance--;
            curr = curr->right;
        }
    }
    // check balance
    if (key <= pya->key)
    {
        pya->left = tree_balance(ya, q);
        return dummy.left;
    }

    else
    {
        pya->right = tree_balance(ya, q);
        #ifdef DEBUG
        preorder(root, NULL);
        fprintf(stderr, "\n");
        #endif
        return dummy.right;
    }
}

Tnode *node_delete(int val, Tnode *root)
{
    if (root == NULL)
    {
        return NULL;
    }

    if (val < root->key)
    {
        root->left = node_delete(val, root->left);
    }

    else if (val > root->key)
    {
        root->right = node_delete(val, root->right);
    }

    else
    {
        if ((root->left == NULL) && (root->right == NULL))
        {
            free(root);
            return NULL;
        }

        if (root->left == NULL) //right child
        {
            Tnode *rc = root->right;
            free(root);
            return rc;
        }

        if (root->right == NULL) //left child
        {
            Tnode *lc = root->left;
            free(root);
            return lc;
        }

        // finds the largest child in left sub-tree
        Tnode *p = root->left;
        while (p->right != NULL)
        {
            p = p->right;
        }
        // pass val to root and delete it
        root->key = p->key;
        p->key = val;
        root->left = node_delete(val, root->left);
    }
    // update height and balance.
    balance_cal(root);
    int bal = root->balance;
    int lb = (root->left == NULL) ? 0 : root->left->balance;
    int rb = (root->right == NULL) ? 0 : root->right->balance;
    if ((bal > 1) && (lb >= 0))
    {
        return rotate_right(root);
    }

    if ((bal < -1) && (rb <= 0))
    {
        return rotate_left(root);
    }

    if ((bal > 1) && (lb < 0))
    {
        root->left = rotate_left(root->left);
        return rotate_right(root);
    }

    if ((bal < -1) && (rb > 0))
    {
        root->right = rotate_right(root->right);
        return rotate_left(root);
    }

    return root;
}

void preorder(Tnode *root, FILE *fptr)
{
    if (root == NULL)
    {
        return;
    }

    #ifndef DEBUG
    if (fptr == NULL)
    {
        return;
    }

    #endif
    Tnode *lc = root->left;
    Tnode *rc = root->right;
    char child = ((lc == NULL) && (rc == NULL)) ? 0 : (lc == NULL) ? 1 : (rc == NULL) ? 2 : 3;
    #ifndef DEBUG
    int rtv = 0;
    rtv = fwrite(&(root->key), sizeof(int), 1, fptr);
    rtv = fwrite(&child, sizeof(char), 1, fptr);
    if (rtv != 1)
    {
        fprintf(stderr, "fwrite fails\n");
        return;
    }
    #endif
    preorder(root->left, fptr);
    preorder(root->right, fptr);
}