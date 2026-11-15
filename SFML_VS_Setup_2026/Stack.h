#pragma once
#include "TreeNode.h"

class Stack {
private:
    TreeNode* arr[300];
    int topIndex;

public:
    Stack() {
        topIndex = -1;
    }

    void push(TreeNode* node) {
        if (topIndex < 299) {
            topIndex++;
            arr[topIndex] = node;
        }
    }

    TreeNode* pop() {
        if (topIndex >= 0) {
            TreeNode* temp = arr[topIndex];
            topIndex--;
            return temp;
        }
        return nullptr;
    }

    bool isEmpty() {
        return (topIndex == -1);
    }
};