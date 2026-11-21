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
    static void saveTreeHelper(TreeNode* node, ofstream& file)//tree ko preorder mein save karega
    {
        if (node == nullptr)
        {
            file << "# ";
            return;
        }

        file << node->value << " ";
        saveTreeHelper(node->left, file);
        saveTreeHelper(node->right, file);
    }

    static void saveTreeToFile(TreeNode* root, string filename)//tree ko file mein save karega
    {
        ofstream file(filename);
        if (file.is_open())
        {
            saveTreeHelper(root, file);
            file.close();
        }
    }

    static TreeNode* loadTreeHelper(ifstream& file, int& nextId, TreeNode* parent)//file se tree dobara banayega
    {
        string word;
        if (!(file >> word))
        {
            return nullptr;
        }
        if (word == "#")
        {
            return nullptr;
        }

        int val = stoi(word);
        TreeNode* node = new TreeNode(nextId, val);
        nextId++;
        node->parent = parent;

        node->left = loadTreeHelper(file, nextId, node);
        node->right = loadTreeHelper(file, nextId, node);

        return node;
    }

    static void recomputeStats(TreeNode* node)//tree ki height aur size dobara calculate karega
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

    static TreeNode* loadTreeFromFile(string filename, int& nextId)//file se tree load karke stats update karega
    {
        ifstream file(filename);
        if (!file.is_open())
        {
            return nullptr;
        }

        TreeNode* root = loadTreeHelper(file, nextId, nullptr);
        file.close();

        recomputeStats(root);
        return root;
    }

    static void saveHeapToFile(MaxHeap& heap, string filename)//heap ko file mein save karega
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

    static void loadHeapFromFile(MaxHeap& heap, string filename)//file se heap load karega
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

    static void saveProgress(int unlockedCount, int stars[4][5], int seed)//campaign ki progress save karega
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

    static void loadProgress(int& unlockedCount, int stars[4][5], int& seed)//campaign ki progress file se load karega
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

    static int loadLeaderboard(int scores[5])//leaderboard ke top 5 scores load karega
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

    static void saveLeaderboardScore(int newScore)//naya score leaderboard mein save karke sort karega
    {
        int scores[6];
        int count = 0;

        ifstream inFile("leaderboard.txt");
        if (inFile.is_open())
        {
            while (count < 5 && (inFile >> scores[count]))
            {
                count++;
            }
            inFile.close();
        }

        scores[count] = newScore;
        count++;

       
        for (int i = 0; i < count - 1; i++) //scores ko descending order mein arrange karega
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