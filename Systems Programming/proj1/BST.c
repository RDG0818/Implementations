#include "BST.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Node *createNode(const char *value) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (!newNode) return NULL;
    newNode->value = strdup(value);
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

void freeNode(Node *node) {
    if (node) {
        free(node->value);
        freeNode(node->left);
        freeNode(node->right);
        free(node);
    }
}

BST *createBST() {
    BST *bst = (BST *)malloc(sizeof(BST));
    if (!bst) return NULL;
    bst->root = NULL;
    return bst;
}

void freeBST(BST *bst) {
    if (bst) {
        freeNode(bst->root);
        free(bst);
    }
}

int insertNode(Node **node, const char *value) {
    if (*node == NULL) {
        *node = createNode(value);
        return *node != NULL;
    }
    if (strcmp(value, (*node)->value) < 0) {
        return insertNode(&(*node)->left, value);
    } else if (strcmp(value, (*node)->value) > 0) {
        return insertNode(&(*node)->right, value);
    }
    return 0; 
}

int searchNode(Node *node, const char *value) {
    if (node == NULL) return 0;
    if (strcmp(value, node->value) == 0) return 1;
    if (strcmp(value, node->value) < 0) {
        return searchNode(node->left, value);
    } else {
        return searchNode(node->right, value);
    }
    return 0;
}

int removeNode(Node **node, const char *value) {
    return 1;
}

void postOrderTraversal(Node *node, FILE *output) {
    if (node == NULL) return;
    postOrderTraversal(node->left, output);
    postOrderTraversal(node->right, output);
    fprintf(output, "%s\n", node->value);
}

void copyBST(Node *srcNode, Node **dstNode) {
    if (srcNode == NULL) return;
    insertNode(dstNode, srcNode->value);
    copyBST(srcNode->left, dstNode);
    copyBST(srcNode->right, dstNode);
}