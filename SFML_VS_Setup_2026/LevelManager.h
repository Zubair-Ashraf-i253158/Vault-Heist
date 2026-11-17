#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <cmath>
#include "BinaryTree.h"
#include "BST.h"
#include "AVL.h"
#include "MaxHeap.h"
#include "ScoreSystem.h"

using namespace std;

struct TargetHitbox {
    int id;
    int value;
    float x;
    float y;
    float radius;
};

class LevelManager {
public:
    int act;
    int subLevel;
    string title;
    string objective;
    string instruction;

    bool levelWon;
    bool levelLost;

    ScoreSystem scoreSystem;

    TargetHitbox hitboxes[100];
    int hitboxCount;

    // Model structures
    BinaryTree act1Tree;
    BST act2Tree;
    AVL act3Tree;
    MaxHeap act4Heap;

    // Interactive tracking for sequence/patrol objectives
    int targetSequence[20];
    int targetSeqLen;
    int currentSeqIndex;

    LevelManager() {
        act = 1;
        subLevel = 1;
        levelWon = false;
        levelLost = false;
        hitboxCount = 0;
        targetSeqLen = 0;
        currentSeqIndex = 0;
        loadLevel(1, 1);
    }

    void registerHitbox(int id, int val, float x, float y, float r) {
        if (hitboxCount < 100) {
            hitboxes[hitboxCount].id = id;
            hitboxes[hitboxCount].value = val;
            hitboxes[hitboxCount].x = x;
            hitboxes[hitboxCount].y = y;
            hitboxes[hitboxCount].radius = r;
            hitboxCount++;
        }
    }

    void loadLevel(int a, int sl) {
        act = a;
        subLevel = sl;
        levelWon = false;
        levelLost = false;
        hitboxCount = 0;
        currentSeqIndex = 0;
        targetSeqLen = 0;

        // ================= ACT 1 =================
        if (act == 1) {
            if (subLevel == 1) {
                scoreSystem.reset(7);
                title = "Level 1.1: Guard Patrol Traversal";
                objective = "Click rooms in exact PREORDER route: 1 -> 2 -> 4 -> 5 -> 3 -> 6 -> 7";
                instruction = "Preorder visits: Root -> Left -> Right. Click each room in sequence.";

                int pre[] = { 1, 2, 4, 5, 3, 6, 7 };
                int in[] = { 4, 2, 5, 1, 6, 3, 7 };
                act1Tree.rebuildTree(pre, in, 7);

                targetSeqLen = 7;
                for (int i = 0; i < 7; i++) targetSequence[i] = pre[i];
            }
            else if (subLevel == 2) {
                scoreSystem.reset(1);
                title = "Level 1.2: Blueprint Reconstruction";
                objective = "Vault reconstructed from Preorder & Inorder streams. Click the ROOT vault.";
                instruction = "Preorder starts at the root. Identify and click the true root.";

                int pre[] = { 5, 2, 1, 3, 8, 6, 9 };
                int in[] = { 1, 2, 3, 5, 6, 8, 9 };
                act1Tree.rebuildTree(pre, in, 7);
            }
            else if (subLevel == 3) {
                scoreSystem.reset(1);
                title = "Level 1.3: Alarm Spread & Diameter";
                objective = "Identify the Longest Escape Path (Diameter). Tree Diameter = 4.";
                instruction = "Click the root node that anchors this maximum path.";

                int pre[] = { 10, 20, 40, 50, 30, 60, 70 };
                int in[] = { 40, 20, 50, 10, 60, 30, 70 };
                act1Tree.rebuildTree(pre, in, 7);
            }
            else if (subLevel == 4) {
                scoreSystem.reset(1);
                title = "Level 1.4: Meeting Room (LCA)";
                objective = "Click the Lowest Common Ancestor meeting room of Room 40 & Room 50.";
                instruction = "LCA of 40 and 50 is Room 20. Select it.";

                int pre[] = { 10, 20, 40, 50, 30, 60, 70 };
                int in[] = { 40, 20, 50, 10, 60, 30, 70 };
                act1Tree.rebuildTree(pre, in, 7);
            }
            else if (subLevel == 5) {
                scoreSystem.reset(1);
                title = "Level 1.5: Mirror the Vault";
                objective = "Vault has been inverted! Click the new Left Child of the Root.";
                instruction = "Original right branch (30) has inverted to the left.";

                int pre[] = { 10, 20, 40, 50, 30, 60, 70 };
                int in[] = { 40, 20, 50, 10, 60, 30, 70 };
                act1Tree.rebuildTree(pre, in, 7);
                act1Tree.mirror();
            }
        }
        // ================= ACT 2 =================
        else if (act == 2) {
            act2Tree.clear();
            act2Tree.insert(50);
            act2Tree.insert(30);
            act2Tree.insert(70);
            act2Tree.insert(20);
            act2Tree.insert(40);
            act2Tree.insert(60);
            act2Tree.insert(80);

            if (subLevel == 1) {
                scoreSystem.reset(3);
                title = "Level 2.1: BST Search Heist";
                objective = "Find treasure room 40. Navigate using BST rules.";
                instruction = "Smaller go left, larger go right. Click Room 40.";
            }
            else if (subLevel == 2) {
                scoreSystem.reset(2);
                title = "Level 2.2: Stash Loot (Insert Leaf)";
                objective = "Where would key 25 be inserted? Click its parent room.";
                instruction = "25 < 50 (left), 25 < 30 (left), 25 > 20 (right). Parent is 20.";
            }
            else if (subLevel == 3) {
                scoreSystem.reset(1);
                title = "Level 2.3: Two-Child Delete Successor";
                objective = "Deleting root 50 requires a successor. Click the Inorder Successor.";
                instruction = "Smallest node in the right subtree: click Room 60.";
            }
            else if (subLevel == 4) {
                scoreSystem.reset(1);
                title = "Level 2.4: Range Raid [25, 45]";
                objective = "Select a valid room inside range [25, 45].";
                instruction = "Rooms 30 and 40 fall in range. Click either.";
            }
            else if (subLevel == 5) {
                scoreSystem.reset(5);
                title = "Level 2.5: Boss - The Lazy Architect";
                objective = "Degenerate skewed corridor! Height = N. Click the deep end: Room 50.";
                instruction = "Corridor search under strict alarm limit.";

                act2Tree.clear();
                for (int i = 10; i <= 50; i += 10) act2Tree.insert(i);
            }
        }
        // ================= ACT 3 =================
        else if (act == 3) {
            act3Tree.clear();

            if (subLevel == 1) {
                scoreSystem.reset(1);
                title = "Level 3.1: Read the Badges";
                objective = "Find the room violating AVL balance (|BF| >= 2).";
                instruction = "Node 30 has left height 2 and right height 0 (BF = +2). Click 30.";

                // Manually construct unbalanced tree
                act3Tree.root = new TreeNode(1, 30);
                act3Tree.root->height = 3;
                act3Tree.root->left = new TreeNode(2, 20);
                act3Tree.root->left->parent = act3Tree.root;
                act3Tree.root->left->height = 2;
                act3Tree.root->left->left = new TreeNode(3, 10);
                act3Tree.root->left->left->parent = act3Tree.root->left;
            }
            else if (subLevel == 2) {
                scoreSystem.reset(1);
                title = "Level 3.2: Single Rotation (LL Repair)";
                objective = "Click the Pivot node that becomes the new subtree root after Right Rotation.";
                instruction = "On an LL imbalance at 30, rotating right promotes child 20 to the top.";

                act3Tree.insert(30);
                act3Tree.insert(20);
                act3Tree.insert(10);
            }
            else if (subLevel == 3) {
                scoreSystem.reset(1);
                title = "Level 3.3: Double Rotation (LR Repair)";
                objective = "Keys 30, 10, 20 inserted. Click the balanced root after LR rotation.";
                instruction = "Double rotation promotes middle value 20 to root.";

                act3Tree.insert(30);
                act3Tree.insert(10);
                act3Tree.insert(20);
            }
            else if (subLevel == 4) {
                scoreSystem.reset(2);
                title = "Level 3.4: Auto-Balanced Wing";
                objective = "Fully auto-balanced AVL vault. Click Room 40.";
                instruction = "Balanced tree ensures O(log n) path.";

                for (int i = 10; i <= 70; i += 10) act3Tree.insert(i);
            }
            else if (subLevel == 5) {
                scoreSystem.reset(1);
                title = "Level 3.5: Boss - Order-Statistic Heist";
                objective = "Find the 3rd SMALLEST room in O(log N) using subtree size!";
                instruction = "Keys: 10, 20, 30, 40, 50, 60, 70. 3rd smallest is Room 30.";

                for (int i = 10; i <= 70; i += 10) act3Tree.insert(i);
            }
        }
        // ================= ACT 4 =================
        else if (act == 4) {
            act4Heap.total = 0;
            act4Heap.heapSize = 0;

            if (subLevel == 1) {
                scoreSystem.reset(1);
                title = "Level 4.1: Activate the Vault (Build Heap)";
                objective = "Array converted to Max Heap. Click the ROOT maximum element.";
                instruction = "In a Max Heap, the root at index 0 must be the highest value (90).";

                int arr[] = { 20, 50, 90, 40, 70, 30, 80 };
                for (int i = 0; i < 7; i++) act4Heap.insert(arr[i]);
            }
            else if (subLevel == 2) {
                scoreSystem.reset(1);
                title = "Level 4.2: Repair the Security Locks (Sift-Down)";
                objective = "Root element is disturbed (value = 10). Which child should it swap with?";
                instruction = "Sift-down swaps with the LARGEST child (80 vs 70). Click 80.";

                act4Heap.total = 3;
                act4Heap.heapSize = 3;
                act4Heap.data[0] = 10;
                act4Heap.data[1] = 80;
                act4Heap.data[2] = 70;
            }
            else if (subLevel == 3) {
                scoreSystem.reset(1);
                title = "Level 4.3: Extract Highest Treasure";
                objective = "Extract the current maximum priority root.";
                instruction = "Click the highest authority room (85).";

                int arr[] = { 25, 10, 60, 40, 85, 30, 75 };
                for (int i = 0; i < 7; i++) act4Heap.insert(arr[i]);
            }
            else if (subLevel == 4) {
                scoreSystem.reset(1);
                title = "Level 4.4: Race Against Guards";
                objective = "Root extracted! Last element swapped to root. Click the new root to sift-down.";
                instruction = "Click the active root at index [0].";

                int arr[] = { 15, 35, 65, 45, 95 };
                for (int i = 0; i < 5; i++) act4Heap.insert(arr[i]);
                act4Heap.extractMax();
            }
            else if (subLevel == 5) {
                scoreSystem.reset(1);
                title = "Level 4.5: Final Vault Boss - Heap Sort";
                objective = "Heap Sort complete! Click the absolute maximum element in the sorted array.";
                instruction = "Sorted array in ascending order. Maximum is at the right end (95).";

                int arr[] = { 15, 35, 65, 45, 95 };
                for (int i = 0; i < 5; i++) act4Heap.insert(arr[i]);
                act4Heap.heapSort();
            }
        }
    }

    void handleNodeClick(float mouseX, float mouseY) {
        if (levelWon || levelLost) return;

        int clickedVal = -9999;
        bool hit = false;

        for (int i = 0; i < hitboxCount; i++) {
            float dx = mouseX - hitboxes[i].x;
            float dy = mouseY - hitboxes[i].y;
            if (sqrt(dx * dx + dy * dy) <= hitboxes[i].radius) {
                clickedVal = hitboxes[i].value;
                hit = true;
                break;
            }
        }

        if (!hit) return;

        scoreSystem.addStep();
        bool correct = false;

        // Check conditions per Act and subLevel
        if (act == 1) {
            if (subLevel == 1) {
                if (clickedVal == targetSequence[currentSeqIndex]) {
                    currentSeqIndex++;
                    instruction = "Correct node! Next in patrol sequence (" + to_string(currentSeqIndex) + "/7)";
                    if (currentSeqIndex == targetSeqLen) correct = true;
                    else return;
                }
            }
            else if (subLevel == 2 && clickedVal == 5) correct = true;
            else if (subLevel == 3 && clickedVal == 10) correct = true;
            else if (subLevel == 4 && clickedVal == 20) correct = true;
            else if (subLevel == 5 && clickedVal == 30) correct = true;
        }
        else if (act == 2) {
            if (subLevel == 1 && clickedVal == 40) correct = true;
            else if (subLevel == 2 && clickedVal == 20) correct = true;
            else if (subLevel == 3 && clickedVal == 60) correct = true;
            else if (subLevel == 4 && (clickedVal == 30 || clickedVal == 40)) correct = true;
            else if (subLevel == 5 && clickedVal == 50) correct = true;
        }
        else if (act == 3) {
            if (subLevel == 1 && clickedVal == 30) correct = true;
            else if (subLevel == 2 && clickedVal == 20) correct = true;
            else if (subLevel == 3 && clickedVal == 20) correct = true;
            else if (subLevel == 4 && clickedVal == 40) correct = true;
            else if (subLevel == 5 && clickedVal == 30) correct = true;
        }
        else if (act == 4) {
            if (subLevel == 1 && clickedVal == 90) correct = true;
            else if (subLevel == 2 && clickedVal == 80) correct = true;
            else if (subLevel == 3 && clickedVal == 85) correct = true;
            else if (subLevel == 4 && clickedVal == act4Heap.data[0]) correct = true;
            else if (subLevel == 5 && clickedVal == 95) correct = true;
        }

        if (correct) {
            levelWon = true;
            scoreSystem.calculateStars();
            instruction = "SUCCESS! Vault sector cleared. Press [ENTER] to advance.";
        }
        else {
            scoreSystem.addPenalty(1, 50);
            instruction = "INCORRECT ROOM (" + to_string(clickedVal) + ")! Alarm triggered! Noise +1";
            if (scoreSystem.isAlarmFull()) {
                levelLost = true;
            }
        }
    }

    void nextLevel() {
        if (!levelWon) return;
        if (subLevel < 5) {
            loadLevel(act, subLevel + 1);
        }
        else if (act < 4) {
            loadLevel(act + 1, 1);
        }
        else {
            instruction = "CAMPAIGN COMPLETE! All 20 levels cleared.";
        }
    }

    void retry() {
        loadLevel(act, subLevel);
    }
};