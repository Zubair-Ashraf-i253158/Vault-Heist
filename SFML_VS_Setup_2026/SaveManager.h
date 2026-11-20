#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "TreeNode.h"
#include "MaxHeap.h"

using namespace std;

class SaveManager
{
public:
    static void serializeTreeHelper(TreeNode* node, stringstream& ss)//tree ko file ke format mein convert karega
    {
        if (node == nullptr)//empty node ko # se show karega
        {
            ss << "# ";
            return;
        }
        ss << node->value << " ";
        serializeTreeHelper(node->left, ss);//left subtree ko save karega
        serializeTreeHelper(node->right, ss);//right subtree ko save karega
    }

    static void saveTreeToFile(TreeNode* root, string filename)//tree ko file mein save karega
    {
        stringstream ss;
        serializeTreeHelper(root, ss);

        ofstream file(filename);
        if (file.is_open())//file successfully open hui ya nahi check karega
        {
            file << ss.str();
            file.close();
        }
    }

    static TreeNode* deserializeTreeHelper(stringstream& ss, int& nextId, TreeNode* parent)//file ke data se tree dobara banaye ga
    {
        string valStr;
        if (!(ss >> valStr) || valStr == "#")//empty node mile to null return karega
        {
            return nullptr;
        }

        int val = stoi(valStr);//string value ko integer mein convert karega
        TreeNode* node = new TreeNode(nextId++, val);//new node create karega
        node->parent = parent;//parent node ka link set karega

        node->left = deserializeTreeHelper(ss, nextId, node);//left subtree ko dobara banaye ga
        node->right = deserializeTreeHelper(ss, nextId, node);//right subtree ko dobara banaye ga

        return node;
    }

    static TreeNode* loadTreeFromFile(string filename, int& nextId)//file se tree load karega
    {
        ifstream file(filename);
        if (!file.is_open())//file open na ho to null return karega
            return nullptr;

        string content;
        getline(file, content);//file ka data read karega
        file.close();

        stringstream ss(content);
        return deserializeTreeHelper(ss, nextId, nullptr);//data se tree reconstruct karega
    }

    static void saveHeapToFile(MaxHeap& heap, string filename)//heap ko file mein save karega
    {
        ofstream file(filename);
        if (file.is_open())//file open hone par data save karega
        {
            file << heap.heapSize << " " << heap.total << "\n";
            for (int i = 0; i < heap.total; i++)//heap ke tamam elements save karega
            {
                file << heap.data[i] << " ";
            }
            file.close();
        }
    }

    static void loadHeapFromFile(MaxHeap& heap, string filename)//file se heap ka data load karega
    {
        ifstream file(filename);
        if (!file.is_open())//file open na ho to function end karega
            return;

        file >> heap.heapSize >> heap.total;//heap ka size aur total elements read karega
        for (int i = 0; i < heap.total; i++)//tamam heap elements read karega
        {
            file >> heap.data[i];
        }
        file.close();
    }
};