#pragma once
#include "BST.h"

class AVL : public BST {
public:
    int rotationCount;
    string lastRotation;

    AVL();

    bool insert(int key);
    TreeNode* insertNode(TreeNode* node, TreeNode* parent, int key);
    bool deleteKey(int key);
    TreeNode* deleteNode(TreeNode* node, int key);

    int height(TreeNode* node);
    int sizeOf(TreeNode* node);
    int bigger(int a, int b);
    int balanceFactor(TreeNode* node);
    void update(TreeNode* node);

    TreeNode* rotateRight(TreeNode* y);
    TreeNode* rotateLeft(TreeNode* x);
    TreeNode* rebalance(TreeNode* node);

    TreeNode* kth(int k);
    int rank(int key);
    int countInRange(int lo, int hi);

    bool balancedCheck(TreeNode* node);
    bool isBalanced();
    bool heightsOk(TreeNode* node);
    bool checkHeights();
    bool sizesOk(TreeNode* node);
    bool checkSizes();
    bool isValidAVL();
    void recomputeAll(TreeNode* node);
};