#pragma once
#include <iostream>
#include <string>
#include "TreeNode.h"
#include "Event.h"
#include "Stack.h"
#include "Queue.h"

using namespace std;

class BinaryTree {
public:
    TreeNode* root;
    int nextId;
    EventLog* events;

    BinaryTree();
    virtual ~BinaryTree();

    void destroy(TreeNode* node);
    void note(string type, int a, int b, int c, int oldVal, int newVal);

    //Recursive Traversals
    void preorderRec(TreeNode* node, int outArr[], int& count);
    void inorderRec(TreeNode* node, int outArr[], int& count);
    void postorderRec(TreeNode* node, int outArr[], int& count);

    //Iterative Traversals Level 1.1
    void preorderIter(int outArr[], int& count);
    void inorderIter(int outArr[], int& count);
    void postorderIter(int outArr[], int& count);
    void levelOrder(int outArr[], int& count);

    //Tree Analysis Level 1.3 1.4 1.5
    int height(TreeNode* node);
    int treeHeight();
    int calcDiameter(TreeNode* node, int& maxDia);
    int getDiameter();

    TreeNode* findLCA(TreeNode* node, int a, int b);
    int getLCA(int a, int b);

    TreeNode* mirrorSubtree(TreeNode* node);
    void mirror();

    //Reconstruction Level 1.2
    TreeNode* buildPreIn(int pre[], int in[], int preStart, int preEnd, int inStart, int inEnd);
    void rebuildTree(int pre[], int in[], int size);

    bool checkSame(TreeNode* p, TreeNode* q);
    bool isSame(TreeNode* otherRoot);
};