#pragma once
#include "TreeNode.h"

class Queue {
private:
    TreeNode* arr[300];
    int frontIdx;
    int rearIdx;
    int count;

public:
    Queue() {
        frontIdx = 0;
        rearIdx = 0;
        count = 0;
    }

    void enqueue(TreeNode* node) {
        if (count < 300) {
            arr[rearIdx] = node;
            rearIdx = (rearIdx + 1) % 300;
            count++;
        }
    }

    TreeNode* dequeue() {
        if (count > 0) {
            TreeNode* item = arr[frontIdx];
            frontIdx = (frontIdx + 1) % 300;
            count--;
            return item;
        }
        return nullptr;
    }

    bool isEmpty() {
        return (count == 0);
    }
};