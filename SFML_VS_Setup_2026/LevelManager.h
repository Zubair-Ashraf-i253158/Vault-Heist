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
    string hintText;
    bool usedHint;

    bool levelWon;
    bool levelLost;
    bool campaignComplete;

    bool isEndlessMode;
    int endlessTotalScore;
    int endlessSolved;
    int targetPivotVal;

    ScoreSystem scoreSystem;

    TargetHitbox hitboxes[100];
    int hitboxCount;

    // Backend models
    BinaryTree act1Tree;
    BST act2Tree;
    AVL act3Tree;
    MaxHeap act4Heap;
    EventLog logger;

    // Act 1.1 Traversal
    int expectedSequence[30];
    int seqLength;
    int currentSeqIdx;

    // Act 1.2 Reconstruction
    int slotValues[7];
    int targetSlots[7];
    int tokenBank[7];
    bool tokenUsed[7];
    int selectedTokenIdx;

    // Act 1.3 Heights & Diameter
    int enteredHeights[8];
    int expectedHeights[8];
    int selectedHeightNode;
    bool allHeightsDone;

    // Act 1.4 LCA & Path
    int lcaA, lcaB, expectedLCA;
    bool lcaFound;

    // Act 2.1, 2.2, 2.5, 3.5 Path Walking
    int pathExpected[10];
    int pathLen;
    int pathIdx;

    // Act 2.3 Deletion
    int selectedDeleteCase;

    // Act 2.4 Range Raid
    int rangeLo, rangeHi;
    bool found30, found40;

    // Act 3.2 & 3.3 Manual Rotations
    bool pivotSelected;
    int rotationStep;

    // Act 4 Interactive Swaps & Extract
    int selectedHeapIdx;
    int extractsRemaining;

    LevelManager() {
        act = 1;
        subLevel = 1;
        levelWon = false;
        levelLost = false;
        campaignComplete = false;
        usedHint = false;
        isEndlessMode = false;
        endlessTotalScore = 0;
        endlessSolved = 0;
        targetPivotVal = 30;
        hitboxCount = 0;
        selectedTokenIdx = -1;
        selectedHeightNode = -1;
        selectedHeapIdx = -1;

        act1Tree.events = &logger;
        act2Tree.events = &logger;
        act3Tree.events = &logger;
        act4Heap.events = &logger;

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

    void finishWin(string msg) {
        levelWon = true;
        scoreSystem.calculateStars();
        if (usedHint && scoreSystem.stars > 2) {
            scoreSystem.stars = 2; // PDF rule: Hint applies -1 star cap
        }
        instruction = msg;
    }

    void checkAlarm() {
        if (!levelWon && scoreSystem.isAlarmFull()) {
            levelLost = true;
        }
    }

    bool isActiveMaxHeap() {
        for (int i = 0; i < act4Heap.heapSize; i++) {
            int l = 2 * i + 1;
            int r = 2 * i + 2;
            if (l < act4Heap.heapSize && act4Heap.data[i] < act4Heap.data[l]) return false;
            if (r < act4Heap.heapSize && act4Heap.data[i] < act4Heap.data[r]) return false;
        }
        return true;
    }

    bool isArraySorted() {
        for (int i = 0; i < act4Heap.total - 1; i++) {
            if (act4Heap.data[i] > act4Heap.data[i + 1]) return false;
        }
        return true;
    }

    bool swapChildrenByVal(TreeNode* node, int val) {
        if (node == nullptr) return false;
        if (node->value == val) {
            TreeNode* temp = node->left;
            node->left = node->right;
            node->right = temp;
            return true;
        }
        return swapChildrenByVal(node->left, val) || swapChildrenByVal(node->right, val);
    }

    bool isAct1Mirrored() {
        TreeNode* r = act1Tree.root;
        if (!r || !r->left || !r->right) return false;
        if (r->left->value != 3 || r->right->value != 2) return false;
        if (r->left->left->value != 7 || r->left->right->value != 6) return false;
        if (r->right->left->value != 5 || r->right->right->value != 4) return false;
        return true;
    }

    void loadLevel(int a, int sl) {
        act = a;
        subLevel = sl;
        levelWon = false;
        levelLost = false;
        campaignComplete = false;
        usedHint = false;
        targetPivotVal = 30;
        hitboxCount = 0;
        currentSeqIdx = 0;
        seqLength = 0;
        selectedTokenIdx = -1;
        selectedHeightNode = -1;
        allHeightsDone = false;
        lcaFound = false;
        pathLen = 0;
        pathIdx = 0;
        selectedDeleteCase = 0;
        found30 = false;
        found40 = false;
        pivotSelected = false;
        rotationStep = 0;
        selectedHeapIdx = -1;
        extractsRemaining = 0;
        logger.clear();

        // ================= ACT 1 =================
        if (act == 1) {
            int pre[] = { 1, 2, 4, 5, 3, 6, 7 };
            int in[] = { 4, 2, 5, 1, 6, 3, 7 };
            act1Tree.rebuildTree(pre, in, 7);

            if (subLevel == 1) {
                seqLength = 7;
                act1Tree.preorderIter(expectedSequence, seqLength);
                scoreSystem.reset(7);
                title = "Level 1.1: Guard Patrol Route";
                objective = "Predict the patrol: Click all rooms in PREORDER.";
                instruction = "Click rooms in traversal order.";
                hintText = "HINT: Preorder visits Root first, then entire Left subtree, then Right subtree.";
            }
            else if (subLevel == 2) {
                int t[7] = { 5, 2, 8, 1, 3, 6, 9 };
                int bank[7] = { 1, 2, 3, 5, 6, 8, 9 };
                for (int i = 0; i < 7; i++) {
                    targetSlots[i] = t[i];
                    slotValues[i] = -1;
                    tokenBank[i] = bank[i];
                    tokenUsed[i] = false;
                }
                scoreSystem.reset(7);
                title = "Level 1.2: Decode the Blueprint";
                objective = "Preorder: [5,2,1,3,8,6,9] | Inorder: [1,2,3,5,6,8,9]";
                instruction = "Click a token below, then click its [?] slot in the tree.";
                hintText = "HINT: First element of Preorder is the root; Inorder splits Left and Right subtrees around it.";
            }
            else if (subLevel == 3) {
                for (int i = 0; i < 8; i++) enteredHeights[i] = -1;
                expectedHeights[4] = 0; expectedHeights[5] = 0;
                expectedHeights[6] = 0; expectedHeights[7] = 0;
                expectedHeights[2] = 1; expectedHeights[3] = 1;
                expectedHeights[1] = 2;
                scoreSystem.reset(8);
                title = "Level 1.3: Alarm Spread & Diameter";
                objective = "Click each room and press a number key to set its height, then enter Diameter.";
                instruction = "Select a room and enter its height.";
                hintText = "HINT: Leaf height = 0. Parent height = 1 + max(left, right). Diameter = longest edge path.";
            }
            else if (subLevel == 4) {
                lcaA = 4; lcaB = 7;
                expectedLCA = act1Tree.getLCA(lcaA, lcaB);
                scoreSystem.reset(2);
                title = "Level 1.4: Meeting Room (LCA)";
                objective = "Find the Lowest Common Ancestor of Room 4 and Room 7, then enter path length.";
                instruction = "Click the meeting room (LCA).";
                hintText = "HINT: LCA is the deepest node that has both target rooms in its subtrees.";
            }
            else if (subLevel == 5) {
                scoreSystem.reset(3);
                title = "Level 1.5: Mirror the Vault";
                objective = "Invert the vault: Click nodes to swap their children (or press [A] for Auto).";
                instruction = "Click parent rooms to swap left & right branches.";
                hintText = "HINT: Mirroring a tree swaps the left and right child of every internal node.";
            }
        }
        // ================= ACT 2 =================
        else if (act == 2) {
            act2Tree.clear();
            act2Tree.insert(50); act2Tree.insert(30); act2Tree.insert(70);
            act2Tree.insert(20); act2Tree.insert(40); act2Tree.insert(60); act2Tree.insert(80);

            if (subLevel == 1) {
                pathExpected[0] = 50; pathExpected[1] = 30; pathExpected[2] = 40;
                pathLen = 3;
                scoreSystem.reset(3);
                title = "Level 2.1: BST Search Heist";
                objective = "Search for Room 40 starting from the Root.";
                instruction = "Click each room along the BST search path.";
                hintText = "HINT: In a BST, go Left if target < current key, and Right if target > current key.";
            }
            else if (subLevel == 2) {
                pathExpected[0] = 50; pathExpected[1] = 30; pathExpected[2] = 20;
                pathLen = 3;
                scoreSystem.reset(3);
                title = "Level 2.2: Stash Loot (BST Insert)";
                objective = "Insert key 25: Follow the BST path from Root to its parent leaf.";
                instruction = "Click each room along the insertion path.";
                hintText = "HINT: Compare 25 at each node (smaller -> left, larger -> right) until you reach the leaf.";
            }
            else if (subLevel == 3) {
                scoreSystem.reset(2);
                title = "Level 2.3: Steal and Collapse (BST Delete)";
                objective = "Delete Root 50: Press [1] Leaf, [2] One-Child, or [3] Two-Children, then click replacement.";
                instruction = "Select the deletion case using [1], [2], or [3].";
                hintText = "HINT: A node with two children is replaced by its Inorder Successor (smallest key in right subtree).";
            }
            else if (subLevel == 4) {
                rangeLo = 25; rangeHi = 45;
                scoreSystem.reset(2);
                title = "Level 2.4: Range Raid [25, 45]";
                objective = "Harvest ALL rooms inside range [25, 45]. Skipped subtrees are greyed out.";
                instruction = "Click every room whose key is within [25, 45].";
                hintText = "HINT: BST range queries prune branches outside [lo, hi] and collect only keys where lo <= key <= hi.";
            }
            else if (subLevel == 5) {
                act2Tree.clear();
                for (int i = 10; i <= 50; i += 10) act2Tree.insert(i);
                pathExpected[0] = 10; pathExpected[1] = 20; pathExpected[2] = 30;
                pathExpected[3] = 40; pathExpected[4] = 50;
                pathLen = 5;
                scoreSystem.reset(5);
                title = "Level 2.5: Boss - The Lazy Architect";
                objective = "Sorted inserts degenerated the BST! Search for Room 50 before the alarm fills.";
                instruction = "Step down the corridor from Root to Room 50.";
                hintText = "HINT: Inserting sorted keys creates a skewed chain of height N, making search O(N).";
            }
        }
        // ================= ACT 3 =================
        else if (act == 3) {
            act3Tree.clear();
            if (subLevel == 1) {
                scoreSystem.reset(1);
                title = "Level 3.1: Read the Badges";
                objective = "Green badge = balanced. Red badge = violation. Click the unbalanced room.";
                instruction = "Inspect the badges and click the violated room.";
                hintText = "HINT: Balance Factor = height(left) - height(right). A red badge means |BF| > 1.";
                act3Tree.root = new TreeNode(1, 30); act3Tree.root->height = 3;
                act3Tree.root->left = new TreeNode(2, 20); act3Tree.root->left->height = 2;
                act3Tree.root->left->left = new TreeNode(3, 10); act3Tree.root->left->left->height = 1;
            }
            else if (subLevel == 2) {
                scoreSystem.reset(2);
                title = "Level 3.2: Manual Repair - Single Rotation";
                objective = "Repair the imbalance: Click the unbalanced pivot, then press [L] or [R] to rotate.";
                instruction = "Click the red-badged pivot, then press [L] or [R].";
                hintText = "HINT: A Left-Left (LL) heavy tree is repaired with a single Right Rotation [R].";
                act3Tree.root = new TreeNode(1, 30); act3Tree.root->height = 3;
                act3Tree.root->left = new TreeNode(2, 20); act3Tree.root->left->height = 2;
                act3Tree.root->left->left = new TreeNode(3, 10); act3Tree.root->left->left->height = 1;
            }
            else if (subLevel == 3) {
                scoreSystem.reset(2);
                title = "Level 3.3: Manual Repair - Double Rotation";
                objective = "Repair the zig-zag imbalance using two rotations ([L] / [R]).";
                instruction = "Press [L] or [R] for the 1st rotation, then the 2nd.";
                hintText = "HINT: A Left-Right (LR) zig-zag needs Left Rotation [L] on the child first, then Right Rotation [R] on the root.";
                act3Tree.root = new TreeNode(1, 30); act3Tree.root->height = 3;
                act3Tree.root->left = new TreeNode(2, 10); act3Tree.root->left->height = 2;
                act3Tree.root->left->right = new TreeNode(3, 20); act3Tree.root->left->right->height = 1;
            }
            else if (subLevel == 4) {
                scoreSystem.reset(1);
                title = "Level 3.4: Auto-Mode & Prediction";
                objective = "Key 10 is about to be inserted. Predict the rotation: [1] LL, [2] RR, [3] LR, [4] RL.";
                instruction = "Press [1], [2], [3], or [4] to predict the rebalancing case.";
                hintText = "HINT: Trace where 10 will land relative to the unbalanced ancestor (Left-Left, Right-Right, etc.).";
                act3Tree.insert(30);
                act3Tree.insert(20);
            }
            else if (subLevel == 5) {
                scoreSystem.reset(3);
                for (int i = 10; i <= 70; i += 10) act3Tree.insert(i);
                pathExpected[0] = 40; pathExpected[1] = 20; pathExpected[2] = 30;
                pathLen = 3;
                title = "Level 3.5: Boss - Order-Statistic Heist";
                objective = "Steal the 3rd SMALLEST treasure in O(log N) using subtreeSize badges.";
                instruction = "Click rooms from Root using subtree sizes.";
                hintText = "HINT: Compare k against (left subtree size + 1): if smaller go left, if equal stop, if larger go right.";
            }
        }
        // ================= ACT 4 =================
        else if (act == 4) {
            act4Heap.total = 0;
            act4Heap.heapSize = 0;
            if (subLevel == 1) {
                int arr[] = { 20, 50, 90, 40, 70, 30, 80 };
                act4Heap.total = 7; act4Heap.heapSize = 7;
                for (int i = 0; i < 7; i++) act4Heap.data[i] = arr[i];
                scoreSystem.reset(5);
                title = "Level 4.1: Activate the Vault (Build Max Heap)";
                objective = "Click two nodes to SWAP them until the Max Heap property holds.";
                instruction = "Click two active nodes to swap them.";
                hintText = "HINT: In a Max Heap, every parent must be >= both of its children. Fix bottom-up.";
            }
            else if (subLevel == 2) {
                int arr[] = { 10, 80, 70, 40, 50 };
                act4Heap.total = 5; act4Heap.heapSize = 5;
                for (int i = 0; i < 5; i++) act4Heap.data[i] = arr[i];
                scoreSystem.reset(2);
                title = "Level 4.2: Repair Security Locks (Sift-Down)";
                objective = "Restore the disturbed Max Heap by swapping nodes.";
                instruction = "Click two nodes to swap and repair the heap.";
                hintText = "HINT: Sift-down always swaps a violating parent with its LARGER child.";
            }
            else if (subLevel == 3) {
                int arr[] = { 85, 60, 75, 10, 40, 25, 30 };
                act4Heap.total = 7; act4Heap.heapSize = 7;
                for (int i = 0; i < 7; i++) act4Heap.data[i] = arr[i];
                extractsRemaining = 1;
                scoreSystem.reset(3);
                title = "Level 4.3: Extract the Greatest Treasure";
                objective = "Press [E] to extract the Max element, then restore the Max Heap.";
                instruction = "Press [E] to extract, then click nodes to heapify.";
                hintText = "HINT: Extract-Max swaps the root with the last active element, shrinks heapSize, then sifts down.";
            }
            else if (subLevel == 4) {
                int arr[] = { 95, 65, 45, 15, 35 };
                act4Heap.total = 5; act4Heap.heapSize = 5;
                for (int i = 0; i < 5; i++) act4Heap.data[i] = arr[i];
                extractsRemaining = 2;
                scoreSystem.reset(6);
                title = "Level 4.4: Race Against the Guards";
                objective = "Complete 2 Extract-Max [E] + Heapify cycles before the alarm fills.";
                instruction = "Press [E] to extract, then swap to restore the heap.";
                hintText = "HINT: Always restore the Max Heap property before pressing [E] for the next extraction.";
            }
            else if (subLevel == 5) {
                int arr[] = { 35, 15, 65, 95, 45 };
                act4Heap.total = 5; act4Heap.heapSize = 5;
                for (int i = 0; i < 5; i++) act4Heap.data[i] = arr[i];
                scoreSystem.reset(10);
                title = "Level 4.5: Final Vault Boss - Heap Sort";
                objective = "Sort the entire array ascending using Build Max Heap + Extract [E] + Heapify.";
                instruction = "Swap to build Max Heap, then repeat [E] and sift-down.";
                hintText = "HINT: Heap Sort = 1) Build Max Heap, 2) Repeatedly Extract-Max [E] and sift-down until heapSize = 1.";
            }
        }
    }

    void loadRandomEndlessPuzzle() {
        levelWon = false;
        levelLost = false;
        campaignComplete = false;
        usedHint = false;
        hitboxCount = 0;
        currentSeqIdx = 0;
        seqLength = 0;
        pathLen = 0;
        pathIdx = 0;
        pivotSelected = false;
        selectedHeapIdx = -1;
        logger.clear();

        int pick = rand() % 4;
        string header = "ENDLESS #" + to_string(endlessSolved + 1) + " (Total: " + to_string(endlessTotalScore) + ") - ";

        if (pick == 0) {
            act = 1; subLevel = 1;
            int v[7];
            v[0] = 10 + (rand() % 10);
            for (int i = 1; i < 7; i++) v[i] = v[i - 1] + 2 + (rand() % 8);
            for (int i = 0; i < 7; i++) {
                int r = rand() % 7;
                int t = v[i]; v[i] = v[r]; v[r] = t;
            }
            int pre[7] = { v[0], v[1], v[3], v[4], v[2], v[5], v[6] };
            int in[7] = { v[3], v[1], v[4], v[0], v[5], v[2], v[6] };
            act1Tree.rebuildTree(pre, in, 7);

            seqLength = 7;
            act1Tree.preorderIter(expectedSequence, seqLength);
            scoreSystem.reset(7);
            title = header + "Preorder Patrol";
            objective = "Click all rooms in PREORDER (Root -> Left -> Right).";
            instruction = "Click rooms in traversal order. [ESC] Save & Exit.";
            hintText = "HINT: Preorder visits Root, then Left subtree, then Right subtree.";
        }
        else if (pick == 1) {
            act = 2; subLevel = 1;
            act2Tree.clear();
            int vals[7];
            vals[0] = 10 + (rand() % 10);
            for (int i = 1; i < 7; i++) vals[i] = vals[i - 1] + 3 + (rand() % 9);
            for (int i = 0; i < 7; i++) {
                int r = rand() % 7;
                int t = vals[i]; vals[i] = vals[r]; vals[r] = t;
            }
            for (int i = 0; i < 7; i++) act2Tree.insert(vals[i]);

            int target = vals[rand() % 7];
            TreeNode* cur = act2Tree.root;
            pathLen = 0;
            while (cur != nullptr && pathLen < 10) {
                pathExpected[pathLen++] = cur->value;
                if (target == cur->value) break;
                else if (target < cur->value) cur = cur->left;
                else cur = cur->right;
            }

            scoreSystem.reset(pathLen);
            title = header + "BST Search";
            objective = "Search for Room " + to_string(target) + " starting from the Root.";
            instruction = "Click each room along the BST path. [ESC] Save & Exit.";
            hintText = "HINT: Go Left if target < current room, Right if target > current room.";
        }
        else if (pick == 2) {
            act = 3; subLevel = 2;
            act3Tree.clear();
            int x = 30 + (rand() % 60);
            targetPivotVal = x;

            act3Tree.root = new TreeNode(1, x); act3Tree.root->height = 3;
            act3Tree.root->left = new TreeNode(2, x - 10); act3Tree.root->left->height = 2;
            act3Tree.root->left->left = new TreeNode(3, x - 20); act3Tree.root->left->left->height = 1;

            scoreSystem.reset(2);
            title = header + "AVL LL Repair";
            objective = "Click the unbalanced red-badged pivot, then press [L] or [R] to rotate.";
            instruction = "Click pivot, then rotate. [ESC] Save & Exit.";
            hintText = "HINT: Left-Left (LL) imbalance is fixed with a Right Rotation [R].";
        }
        else {
            act = 4; subLevel = 1;
            act4Heap.total = 7;
            act4Heap.heapSize = 7;
            for (int i = 0; i < 7; i++) act4Heap.data[i] = 15 + (rand() % 80);
            act4Heap.data[0] = 5 + (rand() % 8);

            scoreSystem.reset(5);
            title = header + "Build Max Heap";
            objective = "Click two parent-child nodes to SWAP until it is a valid Max Heap.";
            instruction = "Swap parent & child nodes. [ESC] Save & Exit.";
            hintText = "HINT: Every parent must be >= both of its children.";
        }
    }

    void handleNodeClick(float mouseX, float mouseY) {
        if (levelWon || levelLost) return;

        // Act 1.2 (Token Bank + Empty Slots)
        if (act == 1 && subLevel == 2) {
            for (int i = 0; i < 7; i++) {
                if (tokenUsed[i]) continue;
                float tx = 200.0f + i * 100.0f;
                float ty = 580.0f;
                if (sqrt((mouseX - tx) * (mouseX - tx) + (mouseY - ty) * (mouseY - ty)) <= 24.0f) {
                    selectedTokenIdx = i;
                    instruction = "Selected token [" + to_string(tokenBank[i]) + "]. Click a [?] tree slot.";
                    return;
                }
            }
            if (selectedTokenIdx != -1) {
                for (int i = 0; i < hitboxCount; i++) {
                    float dx = mouseX - hitboxes[i].x;
                    float dy = mouseY - hitboxes[i].y;
                    if (sqrt(dx * dx + dy * dy) <= hitboxes[i].radius) {
                        int slotIdx = hitboxes[i].id;
                        if (slotValues[slotIdx] != -1) return;
                        if (tokenBank[selectedTokenIdx] == targetSlots[slotIdx]) {
                            scoreSystem.addStep();
                            slotValues[slotIdx] = tokenBank[selectedTokenIdx];
                            tokenUsed[selectedTokenIdx] = true;
                            selectedTokenIdx = -1;
                            instruction = "Token placed! Select the next token.";
                            bool allDone = true;
                            for (int k = 0; k < 7; k++) if (slotValues[k] == -1) allDone = false;
                            if (allDone) finishWin("BLUEPRINT DECODED! Press [ENTER].");
                        }
                        else {
                            scoreSystem.addPenalty(1, 20); // Wrong placement: +1 alarm
                            instruction = "Wrong slot! Alarm +1.";
                        }
                        checkAlarm();
                        return;
                    }
                }
            }
            return;
        }

        int clickedId = -1;
        int clickedVal = -9999;
        bool hit = false;

        for (int i = 0; i < hitboxCount; i++) {
            float dx = mouseX - hitboxes[i].x;
            float dy = mouseY - hitboxes[i].y;
            if (sqrt(dx * dx + dy * dy) <= hitboxes[i].radius) {
                clickedId = hitboxes[i].id;
                clickedVal = hitboxes[i].value;
                hit = true;
                break;
            }
        }

        if (!hit) return;

        // Act 4 Interactive Swapping
        if (act == 4) {
            if (clickedId >= act4Heap.heapSize) {
                scoreSystem.addPenalty(1, 20); // Invalid sorted-region swap: +1 alarm
                instruction = "Sorted region is locked! Alarm +1.";
                checkAlarm();
                return;
            }
            if (selectedHeapIdx == -1) {
                selectedHeapIdx = clickedId;
                instruction = "Selected [" + to_string(act4Heap.data[clickedId]) + "]. Click another active node to SWAP.";
                return;
            }
            else if (selectedHeapIdx == clickedId) {
                selectedHeapIdx = -1;
                instruction = "Selection cancelled.";
                return;
            }
            else {
                // Check if valid parent-child pair (optional strictness: allow any active swap, penalize non-parent-child)
                bool isParentChild = (selectedHeapIdx == (clickedId - 1) / 2) || (clickedId == (selectedHeapIdx - 1) / 2);
                if (!isParentChild) {
                    scoreSystem.addPenalty(1, 20); // PDF: invalid swap in Act 4 (+1 alarm)
                    selectedHeapIdx = -1;
                    instruction = "Invalid swap! Only swap a parent with its child. Alarm +1.";
                    checkAlarm();
                    return;
                }

                scoreSystem.addStep();
                int temp = act4Heap.data[selectedHeapIdx];
                act4Heap.data[selectedHeapIdx] = act4Heap.data[clickedId];
                act4Heap.data[clickedId] = temp;
                selectedHeapIdx = -1;

                if (subLevel == 1 || subLevel == 2) {
                    if (isActiveMaxHeap()) finishWin("MAX HEAP VALID! Press [ENTER].");
                    else instruction = "Swapped! Continue restoring Max Heap property.";
                }
                else if (subLevel == 3 || subLevel == 4) {
                    if (isActiveMaxHeap()) {
                        if (extractsRemaining <= 0) finishWin("EXTRACTION & HEAPIFY COMPLETE! Press [ENTER].");
                        else instruction = "Heap restored! Press [E] for next Extract-Max (" + to_string(extractsRemaining) + " left).";
                    }
                    else instruction = "Swapped! Continue sifting down.";
                }
                else if (subLevel == 5) {
                    if (act4Heap.heapSize <= 1 && isArraySorted()) finishWin("FINAL VAULT UNLOCKED! Press [ENTER].");
                    else if (isActiveMaxHeap()) instruction = "Max Heap valid! Press [E] to Extract Max.";
                    else instruction = "Swapped! Keep restoring Max Heap property.";
                }
                checkAlarm();
                return;
            }
        }

        // Act 1.3 Select node to enter height
        if (act == 1 && subLevel == 3) {
            if (!allHeightsDone) {
                selectedHeightNode = clickedVal;
                instruction = "Room " + to_string(clickedVal) + " selected. Press a number key for its height.";
            }
            return;
        }

        // Act 1.5 Manual child swap
        if (act == 1 && subLevel == 5) {
            scoreSystem.addStep();
            swapChildrenByVal(act1Tree.root, clickedVal);
            if (isAct1Mirrored()) finishWin("VAULT MIRRORED! Press [ENTER].");
            else instruction = "Swapped children of Room " + to_string(clickedVal) + ".";
            checkAlarm();
            return;
        }

        bool correct = false;

        // Act 1.1 & 1.4
        if (act == 1) {
            if (subLevel == 1) {
                if (clickedVal == expectedSequence[currentSeqIdx]) {
                    scoreSystem.addStep();
                    currentSeqIdx++;
                    instruction = "Correct! Progress: " + to_string(currentSeqIdx) + "/" + to_string(seqLength);
                    if (currentSeqIdx == seqLength) correct = true;
                    else { checkAlarm(); return; }
                }
            }
            else if (subLevel == 4) {
                if (!lcaFound && clickedVal == expectedLCA) {
                    scoreSystem.addStep();
                    lcaFound = true;
                    instruction = "Correct LCA! Now press the number key for the path length (edges).";
                    checkAlarm();
                    return;
                }
            }
        }
        // Act 2 Checks
        else if (act == 2) {
            if (subLevel == 1 || subLevel == 2 || subLevel == 5) {
                if (clickedVal == pathExpected[pathIdx]) {
                    scoreSystem.addStep();
                    pathIdx++;
                    if (pathIdx == pathLen) {
                        if (subLevel == 2) act2Tree.insert(25);
                        correct = true;
                    }
                    else {
                        instruction = "Good step! Choose the next room.";
                        checkAlarm();
                        return;
                    }
                }
            }
            else if (subLevel == 3) {
                if (selectedDeleteCase == 3 && clickedVal == 60) {
                    scoreSystem.addStep();
                    act2Tree.deleteKey(50);
                    correct = true;
                }
                else if (selectedDeleteCase != 3) {
                    instruction = "Pick the deletion case first ([1], [2], or [3]).";
                    return;
                }
            }
            else if (subLevel == 4) {
                if (clickedVal == 30 && !found30) { scoreSystem.addStep(); found30 = true; instruction = "Harvested! Find remaining in-range rooms."; }
                else if (clickedVal == 40 && !found40) { scoreSystem.addStep(); found40 = true; instruction = "Harvested! Find remaining in-range rooms."; }
                if (found30 && found40) correct = true;
                else if (clickedVal == 30 || clickedVal == 40) { checkAlarm(); return; }
            }
        }
        // Act 3 Checks
        else if (act == 3) {
            if (subLevel == 1 && clickedVal == targetPivotVal) { scoreSystem.addStep(); correct = true; }
            else if (subLevel == 2 && clickedVal == targetPivotVal) {
                scoreSystem.addStep();
                pivotSelected = true;
                instruction = "Pivot selected! Press [L] or [R] to rotate.";
                checkAlarm();
                return;
            }
            else if (subLevel == 5) {
                if (clickedVal == pathExpected[pathIdx]) {
                    scoreSystem.addStep();
                    pathIdx++;
                    if (pathIdx == pathLen) correct = true;
                    else {
                        instruction = "Correct branch! Continue down the tree.";
                        checkAlarm();
                        return;
                    }
                }
            }
        }

        if (correct) {
            finishWin("SUCCESS! Sector secured. Press [ENTER] to advance.");
        }
        else {
            scoreSystem.addPenalty(1, 20); // PDF: wrong click (+1 alarm)
            instruction = "INCORRECT ROOM (" + to_string(clickedVal) + ")! Alarm +1.";
            checkAlarm();
        }
    }

    void handleKeyPress(sf::Keyboard::Key key) {
        if (levelWon || levelLost) return;

        // Global Hint Key [H] -> Concept-only hint, -1 star cap
        if (key == sf::Keyboard::H) {
            usedHint = true;
            instruction = hintText + " (Star cap: 2)";
            return;
        }

        // Act 1.3: Heights & Diameter (supports both top row & Numpad)
        if (act == 1 && subLevel == 3) {
            int num = -1;
            if (key == sf::Keyboard::Num0 || key == sf::Keyboard::Numpad0) num = 0;
            else if (key == sf::Keyboard::Num1 || key == sf::Keyboard::Numpad1) num = 1;
            else if (key == sf::Keyboard::Num2 || key == sf::Keyboard::Numpad2) num = 2;
            else if (key == sf::Keyboard::Num3 || key == sf::Keyboard::Numpad3) num = 3;
            else if (key == sf::Keyboard::Num4 || key == sf::Keyboard::Numpad4) num = 4;
            else if (key == sf::Keyboard::Num5 || key == sf::Keyboard::Numpad5) num = 5;
            if (num == -1) return;

            if (!allHeightsDone && selectedHeightNode != -1) {
                if (num == expectedHeights[selectedHeightNode]) {
                    scoreSystem.addStep();
                    enteredHeights[selectedHeightNode] = num;
                    selectedHeightNode = -1;
                    allHeightsDone = true;
                    for (int i = 1; i <= 7; i++) if (enteredHeights[i] == -1) allHeightsDone = false;
                    if (allHeightsDone) instruction = "All heights set! Now press the tree DIAMETER (edges).";
                    else instruction = "Correct height! Click the next room.";
                }
                else {
                    scoreSystem.addPenalty(1, 20);
                    instruction = "Wrong height! Alarm +1.";
                }
                checkAlarm();
            }
            else if (allHeightsDone) {
                if (num == 4) {
                    scoreSystem.addStep();
                    finishWin("DIAMETER VERIFIED! Press [ENTER].");
                }
                else {
                    scoreSystem.addPenalty(1, 20);
                    instruction = "Wrong diameter! Alarm +1.";
                    checkAlarm();
                }
            }
        }

        // Act 1.4: Path length after LCA
        if (act == 1 && subLevel == 4 && lcaFound) {
            if (key == sf::Keyboard::Num4 || key == sf::Keyboard::Numpad4) {
                scoreSystem.addStep();
                finishWin("LCA & PATH LENGTH VERIFIED! Press [ENTER].");
            }
            else if ((key >= sf::Keyboard::Num1 && key <= sf::Keyboard::Num9) ||
                (key >= sf::Keyboard::Numpad1 && key <= sf::Keyboard::Numpad9)) {
                scoreSystem.addPenalty(1, 20);
                instruction = "Wrong path length! Alarm +1.";
                checkAlarm();
            }
        }

        // Act 1.5: Auto-Mirror
        if (act == 1 && subLevel == 5 && key == sf::Keyboard::A) {
            scoreSystem.addStep();
            act1Tree.mirror();
            finishWin("Auto-mirror complete! Press [ENTER].");
        }

        // Act 2.3: Delete case selection
        if (act == 2 && subLevel == 3) {
            if (key == sf::Keyboard::Num1 || key == sf::Keyboard::Numpad1 ||
                key == sf::Keyboard::Num2 || key == sf::Keyboard::Numpad2) {
                scoreSystem.addPenalty(1, 20);
                instruction = "Wrong deletion case! Alarm +1.";
                checkAlarm();
            }
            else if (key == sf::Keyboard::Num3 || key == sf::Keyboard::Numpad3) {
                scoreSystem.addStep();
                selectedDeleteCase = 3;
                instruction = "Correct case! Now click the replacement room.";
                checkAlarm();
            }
        }

        // Act 3.2: Manual Right Rotation on LL
        if (act == 3 && subLevel == 2 && pivotSelected) {
            if (key == sf::Keyboard::R) {
                scoreSystem.addStep();
                act3Tree.clear();
                act3Tree.insert(targetPivotVal);
                act3Tree.insert(targetPivotVal - 10);
                act3Tree.insert(targetPivotVal - 20);
                finishWin("RIGHT ROTATION COMPLETE! Press [ENTER].");
            }
            else if (key == sf::Keyboard::L) {
                scoreSystem.addPenalty(2, 50); // PDF: wrong rotation (+2 alarm, -50 pts)
                instruction = "Wrong rotation choice! Alarm +2, -50 pts.";
                checkAlarm();
            }
        }

        // Act 3.3: Manual Double Rotation (LR)
        if (act == 3 && subLevel == 3) {
            if (rotationStep == 0 && key == sf::Keyboard::L) {
                scoreSystem.addStep();
                rotationStep = 1;
                act3Tree.clear();
                act3Tree.root = new TreeNode(1, 30); act3Tree.root->height = 3;
                act3Tree.root->left = new TreeNode(2, 20); act3Tree.root->left->height = 2;
                act3Tree.root->left->left = new TreeNode(3, 10); act3Tree.root->left->left->height = 1;
                instruction = "1st rotation complete! Now perform the 2nd rotation.";
                checkAlarm();
            }
            else if (rotationStep == 1 && key == sf::Keyboard::R) {
                scoreSystem.addStep();
                act3Tree.clear();
                act3Tree.insert(30); act3Tree.insert(10); act3Tree.insert(20);
                finishWin("DOUBLE ROTATION COMPLETE! Press [ENTER].");
            }
            else if (key == sf::Keyboard::L || key == sf::Keyboard::R) {
                scoreSystem.addPenalty(2, 50); // PDF: wrong rotation (+2 alarm, -50 pts)
                instruction = "Wrong rotation choice! Alarm +2, -50 pts.";
                checkAlarm();
            }
        }

        // Act 3.4: Predict rotation before auto-insert
        if (act == 3 && subLevel == 4) {
            if (key == sf::Keyboard::Num1 || key == sf::Keyboard::Numpad1) {
                scoreSystem.addStep();
                act3Tree.insert(10);
                finishWin("CORRECT PREDICTION! Auto-rebalanced. Press [ENTER].");
            }
            else if (key == sf::Keyboard::Num2 || key == sf::Keyboard::Num3 || key == sf::Keyboard::Num4 ||
                key == sf::Keyboard::Numpad2 || key == sf::Keyboard::Numpad3 || key == sf::Keyboard::Numpad4) {
                scoreSystem.addPenalty(2, 50); // PDF: wrong rotation (+2 alarm, -50 pts)
                instruction = "Wrong prediction! Alarm +2, -50 pts.";
                checkAlarm();
            }
        }

        // Act 4.3, 4.4, 4.5: Extract Max [E]
        if (act == 4 && subLevel >= 3 && key == sf::Keyboard::E) {
            if (!isActiveMaxHeap()) {
                scoreSystem.addPenalty(1, 20); // PDF: invalid extraction (+1 alarm)
                instruction = "Invalid extraction! Restore Max Heap first. Alarm +1.";
                checkAlarm();
                return;
            }
            if (act4Heap.heapSize > 1) {
                scoreSystem.addStep();
                int last = act4Heap.heapSize - 1;
                int temp = act4Heap.data[0];
                act4Heap.data[0] = act4Heap.data[last];
                act4Heap.data[last] = temp;
                act4Heap.heapSize--;
                selectedHeapIdx = -1;

                if (subLevel == 3 || subLevel == 4) {
                    extractsRemaining--;
                    if (isActiveMaxHeap() && extractsRemaining <= 0) {
                        finishWin("EXTRACTION COMPLETE! Press [ENTER].");
                    }
                    else {
                        instruction = "Extracted Max! Sift-down to restore Max Heap.";
                    }
                }
                else if (subLevel == 5) {
                    if (act4Heap.heapSize <= 1 && isArraySorted()) {
                        finishWin("FINAL VAULT UNLOCKED! Heap Sort Complete! Press [ENTER].");
                    }
                    else if (isActiveMaxHeap()) {
                        instruction = "Extracted! Press [E] to extract next Max.";
                    }
                    else {
                        instruction = "Extracted! Swap parent-child nodes to restore Max Heap.";
                    }
                }
                checkAlarm();
            }
        }
    }

    void nextLevel() {
        if (!levelWon) return;
        if (subLevel < 5) loadLevel(act, subLevel + 1);
        else if (act < 4) loadLevel(act + 1, 1);
        else {
            campaignComplete = true;
        }
    }
    void retry() {
        loadLevel(act, subLevel);
    }
};