#pragma once
#include <iostream>

using namespace std;

struct TreeNode {
    int id;
    int value;
    int height;
    int size;
    TreeNode* left;
    TreeNode* right;
    TreeNode* parent;

    TreeNode(int nodeId, int val) {
        id = nodeId;
        value = val;
        height = 1; //Leaf height is strictly 1
        size = 1;   //Leaf size is strictly 1
        left = nullptr;
        right = nullptr;
        parent = nullptr;
    }
};