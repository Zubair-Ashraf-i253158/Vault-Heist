#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "BinaryTree.h"
#include "BST.h"
#include "AVL.h"
#include "MaxHeap.h"
#include "ScoreSystem.h"

using namespace std;

struct TargetHitbox {
    int id;      //Acts 1to3 node id  Act 4 array index
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

    // NEW: seed + answers that are calculated by the program
    int seed;
    int baseSeed;
    int playCount;
    int answerValue;     // correct room value (Acts 1-3)
    int answerValue2;    // second accepted value (-1 if none)
    int answerIndex;     // correct array index (Act 4)
    int rangeLo;         // Level 2.4
    int rangeHi;

    LevelManager() {
        act = 1;
        subLevel = 1;
        levelWon = false;
        levelLost = false;
        hitboxCount = 0;
        targetSeqLen = 0;
        currentSeqIndex = 0;
        baseSeed = (int)time(0);     // different every time you run the game
        playCount = 0;
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

    // ---------------- small helper functions ----------------

    // fills arr with n DIFFERENT random numbers between lo and hi
    void makeDistinct(int arr[], int n, int lo, int hi) {
        for (int i = 0; i < n; i++) {
            bool again = true;
            while (again) {
                arr[i] = lo + rand() % (hi - lo + 1);
                again = false;
                for (int j = 0; j < i; j++) {
                    if (arr[j] == arr[i]) again = true;
                }
            }
        }
    }

    void sortArray(int arr[], int n) {
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - 1 - i; j++) {
                if (arr[j] > arr[j + 1]) {
                    int t = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = t;
                }
            }
        }
    }

    // makes a preorder list for the inorder list 'in', root is picked near the middle
    // so the tree stays small and nicely drawn
    void makePre(int in[], int lo, int hi, int pre[], int& idx) {
        if (lo > hi) return;
        int r = (lo + hi) / 2 + (rand() % 3) - 1;
        if (r < lo) r = lo;
        if (r > hi) r = hi;
        pre[idx] = in[r];
        idx++;
        makePre(in, lo, r - 1, pre, idx);
        makePre(in, r + 1, hi, pre, idx);
    }

    string listToString(int arr[], int n) {
        string s = "";
        for (int i = 0; i < n; i++) s += to_string(arr[i]) + " ";
        return s;
    }

    // builds an UNBALANCED tree inside the AVL object (plain BST insert, no rotations)
    void buildPlainTree(int order[], int n) {
        act3Tree.clear();
        for (int i = 0; i < n; i++) {
            act3Tree.root = act3Tree.BST::insertNode(act3Tree.root, nullptr, order[i]);
        }
        act3Tree.recomputeAll(act3Tree.root);
    }

    void setHeap(int arr[], int n) {
        for (int i = 0; i < n; i++) act4Heap.data[i] = arr[i];
        act4Heap.total = n;
        act4Heap.heapSize = n;
    }

    // ---------------- level loading ----------------

    // new play of a level = new seed;  retry = same seed (same values again)
    void loadLevel(int a, int sl) {
        seed = baseSeed + a * 1000 + sl * 100 + playCount * 7;
        playCount++;
        buildLevel(a, sl);
    }

    void buildLevel(int a, int sl) {
        srand(seed);
        act = a;
        subLevel = sl;
        levelWon = false;
        levelLost = false;
        hitboxCount = 0;
        currentSeqIndex = 0;
        targetSeqLen = 0;
        answerValue = -1;
        answerValue2 = -1;
        answerIndex = -1;

        // ================= ACT 1 =================
        if (act == 1) {
            int in[20];
            int pre[20];
            int n = 7 + rand() % 3;          // 7 to 9 rooms

            // Level 1.3 needs the two sides of the root to have different heights
            int tries = 0;
            do {
                makeDistinct(in, n, 10, 99);
                int idx = 0;
                makePre(in, 0, n - 1, pre, idx);
                act1Tree.rebuildTree(pre, in, n);
                tries++;
            } while (subLevel == 3 && tries < 20 &&
                act1Tree.height(act1Tree.root->left) == act1Tree.height(act1Tree.root->right));

            if (subLevel == 1) {
                int type = rand() % 4;
                int order[20];
                int cnt = 0;
                string name = "";
                if (type == 0) { act1Tree.preorderIter(order, cnt);  name = "PREORDER"; }
                else if (type == 1) { act1Tree.inorderIter(order, cnt);   name = "INORDER"; }
                else if (type == 2) { act1Tree.postorderIter(order, cnt); name = "POSTORDER"; }
                else { act1Tree.levelOrder(order, cnt);                   name = "LEVEL ORDER"; }

                targetSeqLen = cnt;
                for (int i = 0; i < cnt; i++) targetSequence[i] = order[i];

                scoreSystem.reset(cnt);
                title = "Level 1.1: Guard Patrol (" + name + ")";
                objective = "Click the rooms in the exact " + name + " route the guards walk.";
                instruction = "Preorder: Root,Left,Right | Inorder: Left,Root,Right | Postorder: Left,Right,Root";
            }
            else if (subLevel == 2) {
                scoreSystem.reset(1);
                title = "Level 1.2: Blueprint Reconstruction";
                objective = "The vault was rebuilt from the two lists below. Click its ROOT room.";
                instruction = "Preorder: " + listToString(pre, n) + "| Inorder: " + listToString(in, n);
                answerValue = act1Tree.root->value;
            }
            else if (subLevel == 3) {
                scoreSystem.reset(1);
                title = "Level 1.3: Alarm Spread (Height)";
                objective = "Which child of the root has the TALLER subtree? Click it.";
                instruction = "Height = number of rooms on the longest path down from that room.";
                int lh = act1Tree.height(act1Tree.root->left);
                int rh = act1Tree.height(act1Tree.root->right);
                if (lh > rh) answerValue = act1Tree.root->left->value;
                else if (rh > lh) answerValue = act1Tree.root->right->value;
                else {   // still equal after 20 tries: accept both
                    answerValue = act1Tree.root->left->value;
                    answerValue2 = act1Tree.root->right->value;
                }
            }
            else if (subLevel == 4) {
                int i1 = rand() % n;
                int i2 = rand() % n;
                while (i2 == i1) i2 = rand() % n;
                int x = in[i1];
                int y = in[i2];
                scoreSystem.reset(1);
                title = "Level 1.4: Meeting Room (LCA)";
                objective = "Click the Lowest Common Ancestor of Room " + to_string(x) + " and Room " + to_string(y) + ".";
                instruction = "The LCA is the lowest room that has both rooms below it (or is one of them).";
                answerValue = act1Tree.getLCA(x, y);
            }
            else if (subLevel == 5) {
                act1Tree.mirror();
                scoreSystem.reset(1);
                title = "Level 1.5: Mirror the Vault";
                objective = "The vault has been MIRRORED. Click the new LEFT child of the root.";
                instruction = "Mirroring swaps the left and right child of every room.";
                answerValue = act1Tree.root->left->value;
            }
        }
        // ================= ACT 2 =================
        else if (act == 2) {
            int vals[7];
            int pre[7];
            makeDistinct(vals, 7, 10, 99);
            sortArray(vals, 7);
            int idx = 0;
            makePre(vals, 0, 6, pre, idx);

            act2Tree.clear();
            for (int i = 0; i < 7; i++) act2Tree.insert(pre[i]);

            if (subLevel == 1) {
                int target = vals[rand() % 7];
                act2Tree.comparisons = 0;
                act2Tree.search(target);
                scoreSystem.reset(act2Tree.comparisons);
                title = "Level 2.1: BST Search Heist";
                objective = "Find treasure room " + to_string(target) + ". Navigate using BST rules.";
                instruction = "Smaller keys go left, larger keys go right.";
                answerValue = target;
            }
            else if (subLevel == 2) {
                int key = 10 + rand() % 90;
                while (act2Tree.exists(key)) key = 10 + rand() % 90;

                // walk down like insert does, the last room we reach is the parent
                TreeNode* cur = act2Tree.root;
                int pathLen = 1;
                while (true) {
                    if (key < cur->value) {
                        if (cur->left == nullptr) break;
                        cur = cur->left;
                    }
                    else {
                        if (cur->right == nullptr) break;
                        cur = cur->right;
                    }
                    pathLen++;
                }
                scoreSystem.reset(pathLen);
                title = "Level 2.2: Stash Loot (Insert Leaf)";
                objective = "Key " + to_string(key) + " will be inserted. Click the room that becomes its PARENT.";
                instruction = "Compare " + to_string(key) + " with each room on the way down until a slot is free.";
                answerValue = cur->value;
            }
            else if (subLevel == 3) {
                scoreSystem.reset(1);
                title = "Level 2.3: Two-Child Delete Successor";
                objective = "Deleting the root (" + to_string(act2Tree.root->value) + ") needs a replacement. Click the SUCCESSOR.";
                instruction = "Inorder successor = smallest key in the right subtree.";
                answerValue = act2Tree.findMin(act2Tree.root->right)->value;
            }
            else if (subLevel == 4) {
                rangeLo = vals[1];
                rangeHi = vals[4];
                scoreSystem.reset(1);
                title = "Level 2.4: Range Raid [" + to_string(rangeLo) + ", " + to_string(rangeHi) + "]";
                objective = "Click any room whose key is inside the range [" + to_string(rangeLo) + ", " + to_string(rangeHi) + "].";
                instruction = "Skip whole subtrees that cannot contain keys in range.";
            }
            else if (subLevel == 5) {
                int five[5];
                makeDistinct(five, 5, 10, 99);
                sortArray(five, 5);
                act2Tree.clear();
                for (int i = 0; i < 5; i++) act2Tree.insert(five[i]);   // sorted deliveries -> corridor
                act2Tree.comparisons = 0;
                act2Tree.search(five[4]);
                scoreSystem.reset(act2Tree.comparisons);
                title = "Level 2.5: Boss - The Lazy Architect";
                objective = "Sorted deliveries made a corridor! Reach the deepest room " + to_string(five[4]) + ".";
                instruction = "Height = n, so every comparison costs alarm.";
                answerValue = five[4];
            }
        }
        // ================= ACT 3 =================
        else if (act == 3) {
            act3Tree.clear();
            act3Tree.rotationCount = 0;

            int s[3];
            makeDistinct(s, 3, 10, 99);
            sortArray(s, 3);                 // s[0] < s[1] < s[2]
            int order[3];

            int type;                         // 0 = LL, 1 = RR, 2 = LR, 3 = RL
            if (subLevel == 1) type = rand() % 4;
            else if (subLevel == 2) type = rand() % 2;
            else type = 2 + rand() % 2;

            if (type == 0) { order[0] = s[2]; order[1] = s[1]; order[2] = s[0]; }
            else if (type == 1) { order[0] = s[0]; order[1] = s[1]; order[2] = s[2]; }
            else if (type == 2) { order[0] = s[2]; order[1] = s[0]; order[2] = s[1]; }
            else { order[0] = s[0]; order[1] = s[2]; order[2] = s[1]; }

            if (subLevel == 1) {
                buildPlainTree(order, 3);
                scoreSystem.reset(1);
                title = "Level 3.1: Read the Badges";
                objective = "Find the room that violates AVL balance (|balance factor| > 1).";
                instruction = "Balance factor = height(left) - height(right).";
                answerValue = order[0];      // the first key inserted is the root, the unbalanced room
            }
            else if (subLevel == 2) {
                buildPlainTree(order, 3);
                scoreSystem.reset(1);
                title = "Level 3.2: Single Rotation Repair";
                objective = "This vault is unbalanced. After the single rotation, click the NEW subtree root.";
                instruction = "Find the case (LL or RR) first, then rotate the opposite way.";
                answerValue = s[1];          // middle key always becomes the root
            }
            else if (subLevel == 3) {
                buildPlainTree(order, 3);
                scoreSystem.reset(1);
                title = "Level 3.3: Double Rotation Repair";
                objective = "Inserted without balancing. Click the root AFTER the double rotation.";
                instruction = "LR / RL need two rotations, the middle key rises to the top.";
                answerValue = s[1];
            }
            else if (subLevel == 4) {
                int v[7];
                makeDistinct(v, 7, 10, 99);
                sortArray(v, 7);
                for (int i = 0; i < 7; i++) act3Tree.insert(v[i]);    // sorted input, AVL repairs itself
                scoreSystem.reset(1);
                title = "Level 3.4: Auto-Balanced Wing";
                objective = "Keys arrived in sorted order, but the AVL vault repaired itself. Click the NEW ROOT.";
                instruction = "Predict which key AVL rebalancing placed at the top.";
                answerValue = act3Tree.root->value;
            }
            else if (subLevel == 5) {
                int v[7];
                makeDistinct(v, 7, 10, 99);
                sortArray(v, 7);
                for (int i = 0; i < 7; i++) act3Tree.insert(v[i]);
                int k = 2 + rand() % 5;                                // 2 to 6
                scoreSystem.reset(1);
                title = "Level 3.5: Boss - Order-Statistic Heist";
                objective = "Steal the " + to_string(k) + "th SMALLEST treasure. Use the gold size badges.";
                instruction = "Compare k with (size of left subtree + 1) at every room.";
                answerValue = act3Tree.kth(k)->value;
            }
        }
        // ================= ACT 4 =================
        else if (act == 4) {
            int arr[7];
            scoreSystem.reset(1);

            if (subLevel == 1) {
                makeDistinct(arr, 7, 10, 99);
                setHeap(arr, 7);                       // raw array, NOT a heap yet
                int best = 0;
                for (int i = 1; i < 7; i++) if (arr[i] > arr[best]) best = i;
                title = "Level 4.1: Activate the Vault (Build Heap)";
                objective = "This array will become a Max Heap. Click the element that ends at the ROOT.";
                instruction = "In a Max Heap every parent is >= its children.";
                answerIndex = best;
            }
            else if (subLevel == 2) {
                makeDistinct(arr, 7, 20, 99);
                setHeap(arr, 7);
                act4Heap.buildMaxHeap();
                act4Heap.data[0] = 1 + rand() % 9;     // disturb the root with a tiny value
                title = "Level 4.2: Repair the Security Locks (Sift-Down)";
                objective = "The root is now " + to_string(act4Heap.data[0]) + ". Click the child it must swap with.";
                instruction = "Sift-down always swaps with the LARGER child.";
                if (act4Heap.data[1] > act4Heap.data[2]) answerIndex = 1;
                else answerIndex = 2;
            }
            else if (subLevel == 3) {
                makeDistinct(arr, 7, 10, 99);
                setHeap(arr, 7);
                act4Heap.buildMaxHeap();
                title = "Level 4.3: Extract the Greatest Treasure";
                objective = "Click the element that Extract-Max will remove.";
                instruction = "The maximum of a Max Heap is always at index 0.";
                answerIndex = 0;
            }
            else if (subLevel == 4) {
                makeDistinct(arr, 7, 10, 99);
                setHeap(arr, 7);
                act4Heap.buildMaxHeap();
                int t = act4Heap.data[0];              // swap root with last element
                act4Heap.data[0] = act4Heap.data[6];
                act4Heap.data[6] = t;
                act4Heap.heapSize = 6;                 // last one is now in the sorted region
                title = "Level 4.4: Race Against the Guards";
                objective = "The max went to the sorted end. Click the child the new root must swap with.";
                instruction = "Active heap size is 6. Index 6 is already sorted.";
                if (act4Heap.data[1] > act4Heap.data[2]) answerIndex = 1;
                else answerIndex = 2;
            }
            else if (subLevel == 5) {
                makeDistinct(arr, 6, 10, 99);
                setHeap(arr, 6);
                act4Heap.heapSort();
                title = "Level 4.5: Final Vault Boss - Heap Sort";
                objective = "Heap Sort is finished. Click the LARGEST element in the sorted array.";
                instruction = "Ascending order puts the maximum at the right end.";
                answerIndex = act4Heap.total - 1;
            }
        }

        title += "  [seed " + to_string(seed) + "]";
    }

    void handleNodeClick(float mouseX, float mouseY) {
        if (levelWon || levelLost) return;

        int clickedVal = -9999;
        int clickedId = -1;
        bool hit = false;

        for (int i = 0; i < hitboxCount; i++) {
            float dx = mouseX - hitboxes[i].x;
            float dy = mouseY - hitboxes[i].y;
            if (sqrt(dx * dx + dy * dy) <= hitboxes[i].radius) {
                clickedVal = hitboxes[i].value;
                clickedId = hitboxes[i].id;
                hit = true;
                break;
            }
        }

        if (!hit) return;

        scoreSystem.addStep();          // every click costs +1 alarm
        bool correct = false;

        if (act == 1 && subLevel == 1) {
            if (clickedVal == targetSequence[currentSeqIndex]) {
                currentSeqIndex++;
                instruction = "Correct! Next in patrol sequence (" + to_string(currentSeqIndex) + "/" + to_string(targetSeqLen) + ")";
                if (currentSeqIndex == targetSeqLen) correct = true;
                else return;
            }
        }
        else if (act == 4) {
            correct = (clickedId == answerIndex);
        }
        else if (act == 2 && subLevel == 4) {
            correct = (clickedVal >= rangeLo && clickedVal <= rangeHi);
        }
        else {
            correct = (clickedVal == answerValue || clickedVal == answerValue2);
        }

        if (correct) {
            if (scoreSystem.isAlarmFull()) {
                levelLost = true;
                return;
            }
            levelWon = true;
            scoreSystem.calculateStars();
            if (act == 4 && subLevel == 1) act4Heap.buildMaxHeap();   // show the finished heap
            instruction = "SUCCESS! Vault sector cleared. Press [ENTER] to advance.";
        }
        else {
            // wrong click = +1 alarm (already added). Only a wrong ROTATION answer costs extra.
            if (act == 3 && (subLevel == 2 || subLevel == 3)) scoreSystem.addPenalty(1, 50);
            else scoreSystem.addPenalty(0, 0);

            instruction = "INCORRECT ROOM (" + to_string(clickedVal) + ")! Alarm raised.";
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
        buildLevel(act, subLevel);      // SAME seed -> same values again (reproducible)
    }
};