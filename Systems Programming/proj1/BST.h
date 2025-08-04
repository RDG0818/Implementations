#pragma once
#include <stdio.h>

typedef struct Node {
    char* value;
    struct Node *left;
    struct Node *right;
} Node;

typedef struct BST {
    Node *root;
} BST;

Node *createNode(const char *value);
void freeNode(Node *node);

BST *createBST();
void freeBST(BST *bst);

int insertNode(Node **node, const char *value);
int searchNode(Node *node, const char *value);
int removeNode(Node **node, const char *value);

void postOrderTraversal(Node *node, FILE *output);
void copyBST(Node *srcNode, Node **dstNode);