#ifndef ENTITY_TREE_H
#define ENTITY_TREE_H

#include "defines.h"
#include "ecs/ecs_defines.h"

typedef struct TreeNode{
    EntityID entity;
    TreeNode* right;
    TreeNode* left;
}TreeNode;
typedef struct EntityTree{
    TreeNode peak;
    u32 levels;
    u32 length;
}EntityTree;

// Function to create a new node with the given data
TreeNode* createNode(EntityID entity) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    newNode->entity = entity;
    newNode->left = 0;
    newNode->right = 0;
    return newNode;
}

// Function to insert a new element into the BST
TreeNode* insert(TreeNode* root, EntityID entity) {
    if (root == 0) {
        return createNode(entity);
    }

    if (entity < root->entity) {
        root->left = insert(root->left, entity);
    } else if (entity > root->entity) {
        root->right = insert(root->right, entity);
    }

    return root;
}

// Function to search for an element in the BST
bool search(TreeNode* root, EntityID entity) {
    if (root == 0) {
        return false;
    }

    if (entity == root->entity) {
        return true;
    } else if (entity < root->entity) {
        return search(root->left, entity);
    } else {
        return search(root->right, entity);
    }
}

// Function to deallocate the memory used by the BST
void destroyTreeNode(TreeNode* root) {
    if (root != 0) {
        destroyTreeNode(root->left);
        destroyTreeNode(root->right);
        free(root);
    }
}

void destroyTree(EntityTree* tree){
    destroyTreeNode(&tree->peak);
    tree->length = 0;
    tree->levels=0;
}

#endif /* ENTITY_TREE_H */