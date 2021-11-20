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

        int indc[3] = {0}; //indicator array
        // argv[2]: the input filename.
        FILE* fptr = fopen(argv[2], "r");
        if (fptr == NULL) 
        {
            fprintf(stderr, "Input file is not valid. \n");
            fprintf(stdout, "%d, %d, %d\n", indc[0], indc[1], indc[2]);
            return EXIT_FAILURE;
        }
        // calculate the size of input
        if (fseek(fptr, 0, SEEK_END) != 0) 
        {
            fprintf(stderr, "fseek fails when calculating size.\n");
            return EXIT_FAILURE;
        }
        int size = ftell(fptr) / (sizeof(int) + sizeof(char));
        // moves to file head
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
        int i = 0; 
        Tnode* root = tree_build(keylist, child, &i, size - 1);
        // test if it is a BST.
        bool test = BST_check(root);
        indc[1] = (test) ? 1 : 0;

        balance_cal(root);
        test = balance_check(root); // avl tree or no?
        indc[2] = (test) ? 1: 0;

        // output 
        fprintf(stdout, "%d, %d, %d\n", 1, indc[1], indc[2]);
        free(keylist);
        free(child);
        tree_destroy(root);
        fclose(fptr);
        return EXIT_SUCCESS;
    }



    else if (!strcmp(argv[1], "-b")) // checks if it is -b for build
    {
        if (argc != 4) 
        {
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