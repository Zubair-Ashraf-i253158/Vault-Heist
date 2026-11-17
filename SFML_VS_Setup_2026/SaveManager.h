#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "TreeNode.h"
#include "MaxHeap.h"

using namespace std;

class SaveManager {
public:
    // 1. Preorder serialization helper: writes '#' for null pointers
    static void serializeTreeHelper(TreeNode* node, stringstream& ss) {
        if (node == nullptr) {
            ss << "# ";
            return;
        }
        ss << node->value << " ";
        serializeTreeHelper(node->left, ss);
        serializeTreeHelper(node->right, ss);
    }

    // Saves tree to a plain text file using Section 8 format (e.g. 50 30 # # 70 # #)
    static void saveTreeToFile(TreeNode* root, string filename) {
        stringstream ss;
        serializeTreeHelper(root, ss);

        ofstream file(filename);
        if (file.is_open()) {
            file << ss.str();
            file.close();
        }
    }

    // 2. Preorder deserialization helper
    static TreeNode* deserializeTreeHelper(stringstream& ss, int& nextId, TreeNode* parent) {
        string valStr;
        if (!(ss >> valStr) || valStr == "#") {
            return nullptr;
        }

        int val = stoi(valStr);
        TreeNode* node = new TreeNode(nextId++, val);
        node->parent = parent;

        node->left = deserializeTreeHelper(ss, nextId, node);
        node->right = deserializeTreeHelper(ss, nextId, node);

        return node;
    }

    static TreeNode* loadTreeFromFile(string filename, int& nextId) {
        ifstream file(filename);
        if (!file.is_open()) return nullptr;

        string content;
        getline(file, content);
        file.close();

        stringstream ss(content);
        return deserializeTreeHelper(ss, nextId, nullptr);
    }

    // 3. Heap serialization (flat array + active heapSize)
    static void saveHeapToFile(MaxHeap& heap, string filename) {
        ofstream file(filename);
        if (file.is_open()) {
            file << heap.heapSize << " " << heap.total << "\n";
            for (int i = 0; i < heap.total; i++) {
                file << heap.data[i] << " ";
            }
            file.close();
        }
    }

    static void loadHeapFromFile(MaxHeap& heap, string filename) {
        ifstream file(filename);
        if (!file.is_open()) return;

        file >> heap.heapSize >> heap.total;
        for (int i = 0; i < heap.total; i++) {
            file >> heap.data[i];
        }
        file.close();
    }
};