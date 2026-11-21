#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "TreeNode.h"
#include "MaxHeap.h"

using namespace std;

class SaveManager
{
public:
    // ================= 1. TREE SAVE & LOAD =================

    // Helper: preorder mein tree ko direct file mein likhta hai
    static void saveTreeHelper(TreeNode* node, ofstream& file)
    {
        if (node == nullptr)
        {
            file << "# "; // empty jagah ke liye # likh do
            return;
        }

        file << node->value << " ";        // pehle root likho
        saveTreeHelper(node->left, file);  // phir left side
        saveTreeHelper(node->right, file); // phir right side
    }

    static void saveTreeToFile(TreeNode* root, string filename)
    {
        ofstream file(filename); // file write karne ke liye kholo
        if (file.is_open())
        {
            saveTreeHelper(root, file);
            file.close();
        }
    }

    // Helper: file se ek ek word read karke tree wapis banata hai
    static TreeNode* loadTreeHelper(ifstream& file, int& nextId, TreeNode* parent)
    {
        string word;
        if (!(file >> word)) // file khatam ho gayi
        {
            return nullptr;
        }
        if (word == "#") // empty node tha
        {
            return nullptr;
        }

        int val = stoi(word); // string ko number banao
        TreeNode* node = new TreeNode(nextId, val);
        nextId++;
        node->parent = parent;

        node->left = loadTreeHelper(file, nextId, node);
        node->right = loadTreeHelper(file, nextId, node);

        return node;
    }

    // Load ke baad har node ki height aur size dobara calculate karta hai
    static void recomputeStats(TreeNode* node)
    {
        if (node == nullptr)
        {
            return;
        }

        recomputeStats(node->left);
        recomputeStats(node->right);

        int leftHeight = 0;
        if (node->left != nullptr) leftHeight = node->left->height;

        int rightHeight = 0;
        if (node->right != nullptr) rightHeight = node->right->height;

        if (leftHeight > rightHeight)
            node->height = 1 + leftHeight;
        else
            node->height = 1 + rightHeight;

        int leftSize = 0;
        if (node->left != nullptr) leftSize = node->left->size;

        int rightSize = 0;
        if (node->right != nullptr) rightSize = node->right->size;

        node->size = 1 + leftSize + rightSize;
    }

    static TreeNode* loadTreeFromFile(string filename, int& nextId)
    {
        ifstream file(filename); // file read karne ke liye kholo
        if (!file.is_open())
        {
            return nullptr;
        }

        TreeNode* root = loadTreeHelper(file, nextId, nullptr);
        file.close();

        recomputeStats(root); // AVL height aur size fix karo
        return root;
    }

    // ================= 2. HEAP SAVE & LOAD =================

    static void saveHeapToFile(MaxHeap& heap, string filename)
    {
        ofstream file(filename);
        if (file.is_open())
        {
            file << heap.heapSize << " " << heap.total << "\n";
            for (int i = 0; i < heap.total; i++)
            {
                file << heap.data[i] << " ";
            }
            file.close();
        }
    }

    static void loadHeapFromFile(MaxHeap& heap, string filename)
    {
        ifstream file(filename);
        if (!file.is_open())
        {
            return;
        }

        file >> heap.heapSize >> heap.total;
        for (int i = 0; i < heap.total; i++)
        {
            file >> heap.data[i];
        }
        file.close();
    }

    // ================= 3. CAMPAIGN PROGRESS SAVE & LOAD =================

    static void saveProgress(int unlockedCount, int stars[4][5], int seed)
    {
        ofstream file("savegame.txt");
        if (!file.is_open())
        {
            return;
        }

        file << unlockedCount << " " << seed << "\n";
        for (int a = 0; a < 4; a++)
        {
            for (int l = 0; l < 5; l++)
            {
                file << stars[a][l] << " ";
            }
            file << "\n";
        }
        file.close();
    }

    static void loadProgress(int& unlockedCount, int stars[4][5], int& seed)
    {
        ifstream file("savegame.txt");
        if (!file.is_open())
        {
            return;
        }

        file >> unlockedCount >> seed;
        for (int a = 0; a < 4; a++)
        {
            for (int l = 0; l < 5; l++)
            {
                file >> stars[a][l];
            }
        }
        file.close();
    }

    // ================= 4. LEADERBOARD SAVE & LOAD =================

    static int loadLeaderboard(int scores[5])
    {
        ifstream file("leaderboard.txt");
        if (!file.is_open())
        {
            return 0;
        }

        int count = 0;
        while (count < 5 && (file >> scores[count]))
        {
            count++;
        }
        file.close();
        return count;
    }

    static void saveLeaderboardScore(int newScore)
    {
        int scores[6];
        int count = 0;

        // Step 1: purane scores file se read karo
        ifstream inFile("leaderboard.txt");
        if (inFile.is_open())
        {
            while (count < 5 && (inFile >> scores[count]))
            {
                count++;
            }
            inFile.close();
        }

        // Step 2: naya score array ke end mein daalo
        scores[count] = newScore;
        count++;

        // Step 3: simple bubble sort (bara score pehle)
        for (int i = 0; i < count - 1; i++)
        {
            for (int j = 0; j < count - i - 1; j++)
            {
                if (scores[j] < scores[j + 1])
                {
                    int temp = scores[j];
                    scores[j] = scores[j + 1];
                    scores[j + 1] = temp;
                }
            }
        }

        // Step 4: top 5 scores wapis file mein likh do
        ofstream outFile("leaderboard.txt");
        if (outFile.is_open())
        {
            int limit = count;
            if (limit > 5) limit = 5;

            for (int i = 0; i < limit; i++)
            {
                outFile << scores[i] << "\n";
            }
            outFile.close();
        }
    }
};