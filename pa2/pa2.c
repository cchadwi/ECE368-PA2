#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "hbt.h"
#include "evaluate.h"
#include "build.h"

int main(int argc, char const *argv[]) 
{
    if (!strcmp(argv[1], "-e")) // checks to see if it is evaluating
    {
        if (argc != 3) // check the number of argument.
        {
            return EXIT_FAILURE;
        }

        int indc[3] = {0};
        // argv[2]: the input filename.
        FILE* fptr = fopen(argv[2], "r");
        // 1st digit indicates if the file can be open successfully.
        if (fptr == NULL) 
        {
            fprintf(stderr, "the input file is not valid. \n");
            fprintf(stdout, "%d, %d, %d\n", indc[0], indc[1], indc[2]);
            return EXIT_FAILURE;
        }
        // now the file has been opened successfully.
        // calculate the size of input info.
        if (fseek(fptr, 0, SEEK_END) != 0) 
        {
            fprintf(stderr, "fseek fails when calculating size.\n");
            return EXIT_FAILURE;
        }
        int size = ftell(fptr) / (sizeof(int) + sizeof(char));
        if (fseek(fptr, 0, SEEK_SET) != 0) 
        {
            fprintf(stderr, "fseek fails when moving to file head.\n");
            return EXIT_FAILURE;
        }
        // malloc the info attributes and read info to it.
        int* keylist = malloc(size * sizeof(int));
        char* child = malloc(size* sizeof(char));
        read_input(fptr, keylist, child, size);
        // build the tree.
        int i = 0; // index for building the tree.
        Tnode* root = tree_build(keylist, child, &i, size - 1);
        // test if it is a BST.
        bool rtv = BST_check(root);
        // 2nd digit indicates if the tree is a valid tree.
        indc[1] = (rtv) ? 1 : 0;
        // calculate the height of each node.
        balance_cal(root);
        // determine if it is an AVL tree.
        rtv = balance_check(root);
        // 3rd digit incicates if the tree is an AVL tree.
        indc[2] = (rtv) ? 1: 0;
        // output the evaluation.
        fprintf(stdout, "%d, %d, %d\n", 1, indc[1], indc[2]);
        free(keylist);
        free(child);
        tree_destroy(root);
        fclose(fptr);
        return EXIT_SUCCESS;
    }



    else if (!strcmp(argv[1], "-b")) 
    {
        // check the number of arguments.
        if (argc != 4) 
        {
            fprintf(stderr, "check the number of argument. \n");
            return EXIT_FAILURE;
        }
        // argv[2]: the filename of input file.
        FILE* fptr = fopen(argv[2], "r");
        if (fptr == NULL) 
        {
            fprintf(stderr, "fails to open the input. \n");
            return EXIT_FAILURE;
        }
        // argv[3]: the name of output file.
        // read input from input file
        if (fseek(fptr, 0, SEEK_END) != 0) 
        {
            fprintf(stderr, "fseek fails when calculating size.\n");
            return EXIT_FAILURE;
        }
        int size = ftell(fptr) / (sizeof(int) + sizeof(char));
        if (fseek(fptr, 0, SEEK_SET) != 0) 
        {
            fprintf(stderr, "fseek fails when moving to file head.\n");
            return EXIT_FAILURE;
        }
        // malloc the info attributes and read info to it.
        int* keylist = malloc(size * sizeof(int));
        char* ops = malloc(size* sizeof(char));
        read_input(fptr, keylist, ops, size);
        fclose(fptr);
        fptr = fopen(argv[3], "w");
        if (fptr == NULL) 
        {
            fprintf(stderr, "fails to open the output. \n");
            return EXIT_FAILURE;
        }
        // build the tree.
        Tnode* root = NULL;
        int i;
        for (i = 0; i < size; i++) 
        {
            if (ops[i] == 'i') 
            {
                // insert a node.
                root = node_insert(keylist[i], root);
            }
            else if (ops[i] == 'd') 
            {
                root = node_delete(keylist[i], root);
            }
            #ifdef DEBUG
            preorder(root, NULL);
            #endif
        }
        // print the tree in preorder.
        #ifndef DEBUG
        preorder(root, fptr);
        #endif
        // release the moemory allocated.
        free(keylist);
        free(ops);
        tree_destroy(root);
        fclose(fptr);
        return EXIT_SUCCESS;
    }
    fprintf(stderr, "option not recoginized. \n");
    return EXIT_FAILURE;
}