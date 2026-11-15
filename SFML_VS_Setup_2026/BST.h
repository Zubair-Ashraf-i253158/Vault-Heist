#pragma once
#include <iostream>
#include <string>
#include "TreeNode.h"
#include "Event.h"

using namespace std;

class BST {
public:
    TreeNode* root;
    int nextId;
    EventLog* events;
    int comparisons;

    BST();
    virtual ~BST();

    void destroy(TreeNode* node);
    void clear();
    bool isEmpty();

    void note(string type, int a, int b, int c, int oldVal, int newVal);
    bool exists(int key);

    TreeNode* insertNode(TreeNode* node, TreeNode* parent, int key);
    bool insert(int key);

    TreeNode* search(int key);

    TreeNode* deleteNode(TreeNode* node, int key);
    bool deleteKey(int key);

    TreeNode* findMin(TreeNode* node);
    TreeNode* findMax(TreeNode* node);
    TreeNode* successor(int key);
    TreeNode* predecessor(int key);

    void rangeHelper(TreeNode* node, int lo, int hi, int outArr[], int& count);
    void rangeQuery(int lo, int hi, int outArr[], int& count);

    TreeNode* kthHelper(TreeNode* node, int k, int& counter);
    TreeNode* kthSmallest(int k);
    TreeNode* lcaBST(int keyA, int keyB);

    int height(TreeNode* node);
    int treeHeight();
    int countNodes(TreeNode* node);

    void inorderHelper(TreeNode* node, int outArr[], int& count);
    void inorderKeys(int outArr[], int& count);

    bool isBST(TreeNode* node, long long minVal, long long maxVal);
    bool isValidBST();
    bool parentsOk(TreeNode* node);
    bool checkParents();
};