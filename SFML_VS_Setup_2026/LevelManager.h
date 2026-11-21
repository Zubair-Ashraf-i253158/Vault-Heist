#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <cmath>
#include <ctime>
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

    // Endless Mode state
    bool isEndlessMode;
    int endlessTotalScore;
    int endlessSolved;

    ScoreSystem scoreSystem;

    TargetHitbox hitboxes[100];
    int hitboxCount;

    // Backend models
    BinaryTree act1Tree;
    BST act2Tree;
    AVL act3Tree;
    MaxHeap act4Heap;
    EventLog logger;

    // Seeded random generator (independent of main.cpp's rand())
    int seed;
    int baseSeed;
    int playCount;
    unsigned int rngState;

    // Act 1 common data (random tree)
    int treeN;
    int treePre[20];
    int treeIn[20];
    BinaryTree act1Ref;

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

    // Act 1.3 Heights & Diameter (indexed by room VALUE 10..99)
    int enteredHeights[100];
    int expectedHeights[100];
    int selectedHeightNode;
    bool allHeightsDone;
    int expectedDiameter;

    // Act 1.4 LCA & Path
    int lcaA, lcaB, expectedLCA;
    int expectedPathLen;
    bool lcaFound;

    // Act 2.1, 2.2, 2.5, 3.5 Path Walking
    int pathExpected[10];
    int pathLen;
    int pathIdx;
    int newKey;

    // Act 2.3 Deletion
    int selectedDeleteCase;
    int deleteTarget;
    int deleteCase;
    int replacementValue;

    // Act 2.4 Range Raid
    int rangeLo, rangeHi;
    bool found30, found40;
    bool harvested[100];
    int rangeTotal, rangeFound;

    // Act 3 Rotations
    int pivotValue;
    bool pivotSelected;
    int rotationStep;
    sf::Keyboard::Key expectedKey1;
    sf::Keyboard::Key expectedKey2;
    int rotOrder[3];
    int rotKeys[3];
    int rotType;
    int predictedCase;

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
        hitboxCount = 0;
        selectedTokenIdx = -1;
        selectedHeightNode = -1;
        selectedHeapIdx = -1;
        playCount = 0;
        baseSeed = (int)time(0);
        seed = baseSeed;
        rngState = 1;

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

    // ================= SEEDED RANDOM HELPERS =================
    void seedRng(int s) {
        rngState = (unsigned int)s * 2654435761u + 12345u;
        rnd(10);
        rnd(10);
    }

    int rnd(int n) {
        rngState = rngState * 1664525u + 1013904223u;
        return (int)((rngState >> 8) % (unsigned int)n);
    }

    void makeDistinct(int arr[], int n, int lo, int hi) {
        for (int i = 0; i < n; i++) {
            bool again = true;
            while (again) {
                arr[i] = lo + rnd(hi - lo + 1);
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

    void shuffleArray(int arr[], int n) {
        for (int i = n - 1; i > 0; i--) {
            int j = rnd(i + 1);
            int t = arr[i];
            arr[i] = arr[j];
            arr[j] = t;
        }
    }

    void makePre(int in[], int lo, int hi, int pre[], int& idx) {
        if (lo > hi) return;
        int r = (lo + hi) / 2 + rnd(3) - 1;
        if (r < lo) r = lo;
        if (r > hi) r = hi;
        pre[idx] = in[r];
        idx++;
        makePre(in, lo, r - 1, pre, idx);
        makePre(in, r + 1, hi, pre, idx);
    }

    string listToString(int arr[], int n) {
        string s = "[";
        for (int i = 0; i < n; i++) {
            s += to_string(arr[i]);
            if (i < n - 1) s += ",";
        }
        return s + "]";
    }

    int keyToDigit(sf::Keyboard::Key key) {
        if (key >= sf::Keyboard::Num0 && key <= sf::Keyboard::Num9) return (int)key - (int)sf::Keyboard::Num0;
        if (key >= sf::Keyboard::Numpad0 && key <= sf::Keyboard::Numpad9) return (int)key - (int)sf::Keyboard::Numpad0;
        return -1;
    }

    int depthOf(TreeNode* node, int value, int d) {
        if (node == nullptr) return -1;
        if (node->value == value) return d;
        int l = depthOf(node->left, value, d + 1);
        if (l != -1) return l;
        return depthOf(node->right, value, d + 1);
    }

    int countInternal(TreeNode* node) {
        if (node == nullptr) return 0;
        if (node->left == nullptr && node->right == nullptr) return 0;
        return 1 + countInternal(node->left) + countInternal(node->right);
    }

    void fillHeights(TreeNode* node) {
        if (node == nullptr) return;
        expectedHeights[node->value] = act1Tree.height(node) ;
        fillHeights(node->left);
        fillHeights(node->right);
    }

    int findUnbalanced(TreeNode* node) {
        if (node == nullptr) return -1;
        int bf = act3Tree.balanceFactor(node);
        if (bf > 1 || bf < -1) return node->value;
        int l = findUnbalanced(node->left);
        if (l != -1) return l;
        return findUnbalanced(node->right);
    }

    TreeNode* findNodeBST(int key) {
        TreeNode* cur = act2Tree.root;
        while (cur != nullptr) {
            if (key == cur->value) return cur;
            if (key < cur->value) cur = cur->left;
            else cur = cur->right;
        }
        return nullptr;
    }

    void buildPlain(int a, int b, int c) {
        int keys[3] = { a, b, c };
        act3Tree.clear();
        for (int i = 0; i < 3; i++)
            act3Tree.root = act3Tree.BST::insertNode(act3Tree.root, nullptr, keys[i]);
        act3Tree.recomputeAll(act3Tree.root);
    }

    void makeRotOrder(int type, int s[], int order[]) {
        if (type == 0) { order[0] = s[2]; order[1] = s[1]; order[2] = s[0]; }
        else if (type == 1) { order[0] = s[0]; order[1] = s[1]; order[2] = s[2]; }
        else if (type == 2) { order[0] = s[2]; order[1] = s[0]; order[2] = s[1]; }
        else { order[0] = s[0]; order[1] = s[2]; order[2] = s[1]; }
    }

    void loadHeap(int arr[], int n) {
        for (int i = 0; i < n; i++) act4Heap.data[i] = arr[i];
        act4Heap.total = n;
        act4Heap.heapSize = n;
    }

    bool isMaxHeapArray(int arr[], int n) {
        for (int i = 1; i < n; i++) {
            if (arr[i] > arr[(i - 1) / 2]) return false;
        }
        return true;
    }

    // ================= WIN / ALARM / MODEL CHECKS =================
    void finishWin(string msg) {
        if (scoreSystem.isAlarmFull()) {
            levelLost = true;
            return;
        }
        levelWon = true;
        scoreSystem.calculateStars();
        if (usedHint && scoreSystem.stars > 2) {
            scoreSystem.stars = 2;
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
        return act1Tree.isSame(act1Ref.root);
    }

    // ================= LEVEL LOADING =================
    void loadLevel(int a, int sl) {
        seed = baseSeed + a * 1000 + sl * 100 + playCount * 7;
        playCount++;
        buildLevel(a, sl);
    }

    void loadRandomEndlessPuzzle() {
        int pick = rand() % 4;
        int a = 1, sl = 1;
        if (pick == 0) { a = 1; sl = 1; }      // Random Traversal
        else if (pick == 1) { a = 2; sl = 1; } // Random BST Search
        else if (pick == 2) { a = 3; sl = 2; } // Random AVL Single Rotation
        else { a = 4; sl = 1; }                // Random Build Max Heap

        loadLevel(a, sl);
        title = "ENDLESS #" + to_string(endlessSolved + 1) +
            " (Total: " + to_string(endlessTotalScore) + ") | " + title;
        instruction += " [ESC] Save & Exit.";
    }

    void buildLevel(int a, int sl) {
        seedRng(seed);
        act = a;
        subLevel = sl;
        levelWon = false;
        levelLost = false;
        campaignComplete = false;
        usedHint = false;
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
        rangeFound = 0;
        rangeTotal = 0;
        for (int i = 0; i < 100; i++) {
            enteredHeights[i] = -1;
            expectedHeights[i] = -1;
            harvested[i] = false;
        }
        logger.clear();

        // ================= ACT 1 =================
        if (act == 1) {
            if (subLevel == 2) {
                int v[7];
                makeDistinct(v, 7, 10, 99);
                int preIdx[7] = { 0, 1, 3, 4, 2, 5, 6 };
                int inIdx[7] = { 3, 1, 4, 0, 5, 2, 6 };
                int postIdx[7] = { 3, 4, 1, 5, 6, 2, 0 };
                int bank[7];
                for (int i = 0; i < 7; i++) {
                    targetSlots[i] = v[i];
                    slotValues[i] = -1;
                    bank[i] = v[i];
                    treePre[i] = v[preIdx[i]];
                    treeIn[i] = v[inIdx[i]];
                }
                sortArray(bank, 7);
                for (int i = 0; i < 7; i++) {
                    tokenBank[i] = bank[i];
                    tokenUsed[i] = false;
                }
                treeN = 7;
                act1Tree.rebuildTree(treePre, treeIn, 7);

                int post[7];
                for (int i = 0; i < 7; i++) post[i] = v[postIdx[i]];
                bool usePost = (rnd(2) == 1);

                scoreSystem.reset(7);
                title = "Level 1.2: Decode the Blueprint";
                if (usePost) {
                    objective = "Postorder: " + listToString(post, 7) + " | Inorder: " + listToString(treeIn, 7);
                    hintText = "HINT: The LAST element of Postorder is the root; Inorder splits Left and Right subtrees around it.";
                }
                else {
                    objective = "Preorder: " + listToString(treePre, 7) + " | Inorder: " + listToString(treeIn, 7);
                    hintText = "HINT: The FIRST element of Preorder is the root; Inorder splits Left and Right subtrees around it.";
                }
                instruction = "Click a token below, then click its [?] slot in the tree.";
            }
            else {
                treeN = 7 + rnd(3);
                makeDistinct(treeIn, treeN, 10, 99);
                int idx = 0;
                makePre(treeIn, 0, treeN - 1, treePre, idx);
                act1Tree.rebuildTree(treePre, treeIn, treeN);

                if (subLevel == 1) {
                    int type = rnd(4);
                    int cnt = 0;
                    string name = "";
                    if (type == 0) {
                        act1Tree.preorderIter(expectedSequence, cnt); name = "PREORDER";
                        hintText = "HINT: Preorder visits Root first, then the entire Left subtree, then the Right subtree.";
                    }
                    else if (type == 1) {
                        act1Tree.inorderIter(expectedSequence, cnt); name = "INORDER";
                        hintText = "HINT: Inorder visits the Left subtree, then the Root, then the Right subtree.";
                    }
                    else if (type == 2) {
                        act1Tree.postorderIter(expectedSequence, cnt); name = "POSTORDER";
                        hintText = "HINT: Postorder visits the Left subtree, then the Right subtree, and the Root LAST.";
                    }
                    else {
                        act1Tree.levelOrder(expectedSequence, cnt); name = "LEVEL ORDER";
                        hintText = "HINT: Level order visits rooms level by level, from left to right.";
                    }
                    seqLength = cnt;
                    scoreSystem.reset(cnt);
                    title = "Level 1.1: Guard Patrol Route";
                    objective = "Predict the patrol: Click all rooms in " + name + ".";
                    instruction = "Click rooms in traversal order.";
                }
                else if (subLevel == 3) {
                    fillHeights(act1Tree.root);
                    expectedDiameter = act1Tree.getDiameter();
                    scoreSystem.reset(treeN + 1);
                    title = "Level 1.3: Alarm Spread & Diameter";
                    objective = "Click each room and press a number key for its height, then enter the Diameter.";
                    instruction = "Select a room and enter its height.";
                    hintText = "HINT: Leaf height = 1. Parent height = 1 + max(left, right). Diameter = longest path in edges.";
                }
                else if (subLevel == 4) {
                    lcaA = treeIn[0];
                    lcaB = treeIn[1];
                    expectedLCA = act1Tree.getLCA(lcaA, lcaB);
                    for (int t = 0; t < 60; t++) {
                        int ia = rnd(treeN);
                        int ib = rnd(treeN);
                        if (ia == ib) continue;
                        lcaA = treeIn[ia];
                        lcaB = treeIn[ib];
                        expectedLCA = act1Tree.getLCA(lcaA, lcaB);
                        if (expectedLCA != lcaA && expectedLCA != lcaB) break;
                    }
                    expectedPathLen = depthOf(act1Tree.root, lcaA, 0) + depthOf(act1Tree.root, lcaB, 0)
                        - 2 * depthOf(act1Tree.root, expectedLCA, 0);
                    scoreSystem.reset(2);
                    title = "Level 1.4: Meeting Room (LCA)";
                    objective = "Find the LCA of Room " + to_string(lcaA) + " and Room " + to_string(lcaB) + ", then enter the path length.";
                    instruction = "Click the meeting room (LCA).";
                    hintText = "HINT: LCA is the deepest node that has both target rooms in its subtrees. Path length is counted in edges.";
                }
                else if (subLevel == 5) {
                    act1Ref.rebuildTree(treePre, treeIn, treeN);
                    act1Ref.mirror();
                    scoreSystem.reset(countInternal(act1Tree.root));
                    title = "Level 1.5: Mirror the Vault";
                    objective = "Invert the vault: Click nodes to swap their children (or press [A] for Auto).";
                    instruction = "Click parent rooms to swap left & right branches.";
                    hintText = "HINT: Mirroring a tree swaps the left and right child of every internal node.";
                }
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
                int target = vals[rnd(7)];
                TreeNode* cur = act2Tree.root;
                while (cur != nullptr && pathLen < 10) {
                    pathExpected[pathLen++] = cur->value;
                    if (target == cur->value) break;
                    cur = (target < cur->value) ? cur->left : cur->right;
                }
                scoreSystem.reset(pathLen);
                title = "Level 2.1: BST Search Heist";
                objective = "Search for Room " + to_string(target) + " starting from the Root.";
                instruction = "Click each room along the BST search path.";
                hintText = "HINT: In a BST, go Left if target < current key, and Right if target > current key.";
            }
            else if (subLevel == 2) {
                newKey = 10 + rnd(90);
                while (act2Tree.exists(newKey)) newKey = 10 + rnd(90);
                TreeNode* cur = act2Tree.root;
                while (cur != nullptr && pathLen < 10) {
                    pathExpected[pathLen++] = cur->value;
                    TreeNode* nxt = (newKey < cur->value) ? cur->left : cur->right;
                    if (nxt == nullptr) break;
                    cur = nxt;
                }
                scoreSystem.reset(pathLen);
                title = "Level 2.2: Stash Loot (BST Insert)";
                objective = "Insert key " + to_string(newKey) + ": Follow the BST path from Root to its parent room.";
                instruction = "Click each room along the insertion path.";
                hintText = "HINT: Compare " + to_string(newKey) + " at each node (smaller -> left, larger -> right) until the next slot is free.";
            }
            else if (subLevel == 3) {
                deleteTarget = vals[rnd(7)];
                TreeNode* node = findNodeBST(deleteTarget);
                if (node->left == nullptr && node->right == nullptr) {
                    deleteCase = 1;
                    replacementValue = -1;
                }
                else if (node->left == nullptr || node->right == nullptr) {
                    deleteCase = 2;
                    replacementValue = (node->left != nullptr) ? node->left->value : node->right->value;
                }
                else {
                    deleteCase = 3;
                    replacementValue = act2Tree.findMin(node->right)->value;
                }
                scoreSystem.reset(deleteCase == 1 ? 1 : 2);
                title = "Level 2.3: Steal and Collapse (BST Delete)";
                objective = "Delete room " + to_string(deleteTarget) + ": press [1] Leaf, [2] One-Child or [3] Two-Children.";
                instruction = "Select the deletion case using [1], [2], or [3].";
                hintText = "HINT: Leaf = just unlink. One child = the child takes its place. Two children = Inorder Successor (smallest key in right subtree) takes its place.";
            }
            else if (subLevel == 4) {
                rangeLo = vals[0] + 1 + rnd(vals[1] - vals[0]);
                int top = 3 + rnd(2);
                rangeHi = vals[top] + rnd(vals[top + 1] - vals[top]);
                for (int i = 0; i < 7; i++) {
                    if (vals[i] >= rangeLo && vals[i] <= rangeHi) rangeTotal++;
                }
                scoreSystem.reset(rangeTotal);
                title = "Level 2.4: Range Raid [" + to_string(rangeLo) + ", " + to_string(rangeHi) + "]";
                objective = "Harvest ALL rooms inside range [" + to_string(rangeLo) + ", " + to_string(rangeHi) + "].";
                instruction = "Click every room whose key is within the range.";
                hintText = "HINT: BST range queries prune branches outside [lo, hi] and collect only keys where lo <= key <= hi.";
            }
            else if (subLevel == 5) {
                int five[5];
                makeDistinct(five, 5, 10, 99);
                sortArray(five, 5);
                act2Tree.clear();
                for (int i = 0; i < 5; i++) act2Tree.insert(five[i]);
                for (int i = 0; i < 5; i++) pathExpected[i] = five[i];
                pathLen = 5;
                scoreSystem.reset(5);
                title = "Level 2.5: Boss - The Lazy Architect";
                objective = "Sorted inserts degenerated the BST! Search for Room " + to_string(five[4]) + " before the alarm fills.";
                instruction = "Step down the corridor from Root to the target room.";
                hintText = "HINT: Inserting sorted keys creates a skewed chain of height N, making search O(N).";
            }
        }
        // ================= ACT 3 =================
        else if (act == 3) {
            act3Tree.clear();
            int s[3];
            makeDistinct(s, 3, 10, 99);
            sortArray(s, 3);

            if (subLevel == 1) {
                int type = rnd(4);
                makeRotOrder(type, s, rotOrder);
                buildPlain(rotOrder[0], rotOrder[1], rotOrder[2]);
                pivotValue = findUnbalanced(act3Tree.root);
                scoreSystem.reset(1);
                title = "Level 3.1: Read the Badges";
                objective = "Green badge = balanced. Red badge = violation. Click the unbalanced room.";
                instruction = "Inspect the badges and click the violated room.";
                hintText = "HINT: Balance Factor = height(left) - height(right). A red badge means |BF| > 1.";
            }
            else if (subLevel == 2) {
                rotType = rnd(2);
                for (int i = 0; i < 3; i++) rotKeys[i] = s[i];
                makeRotOrder(rotType, s, rotOrder);
                buildPlain(rotOrder[0], rotOrder[1], rotOrder[2]);
                pivotValue = findUnbalanced(act3Tree.root);
                expectedKey1 = (rotType == 0) ? sf::Keyboard::R : sf::Keyboard::L;
                scoreSystem.reset(2);
                title = "Level 3.2: Manual Repair - Single Rotation";
                objective = "Repair the imbalance: Click the unbalanced pivot, then press [L] or [R] to rotate.";
                instruction = "Click the red-badged pivot, then press [L] or [R].";
                hintText = "HINT: A left-heavy (LL) tree is repaired with a Right rotation [R]; a right-heavy (RR) tree with a Left rotation [L].";
            }
            else if (subLevel == 3) {
                rotType = 2 + rnd(2);
                for (int i = 0; i < 3; i++) rotKeys[i] = s[i];
                makeRotOrder(rotType, s, rotOrder);
                buildPlain(rotOrder[0], rotOrder[1], rotOrder[2]);
                expectedKey1 = (rotType == 2) ? sf::Keyboard::L : sf::Keyboard::R;
                expectedKey2 = (rotType == 2) ? sf::Keyboard::R : sf::Keyboard::L;
                scoreSystem.reset(2);
                title = "Level 3.3: Manual Repair - Double Rotation";
                objective = "Repair the zig-zag imbalance using two rotations ([L] / [R]).";
                instruction = "Press [L] or [R] for the 1st rotation, then the 2nd.";
                hintText = "HINT: LR zig-zag: Left rotation [L] on the child first, then Right rotation [R] on the root. RL is the mirror image: [R] then [L].";
            }
            else if (subLevel == 4) {
                int type = rnd(4);
                makeRotOrder(type, s, rotOrder);
                act3Tree.insert(rotOrder[0]);
                act3Tree.insert(rotOrder[1]);
                newKey = rotOrder[2];
                AVL tmp;
                tmp.insert(rotOrder[0]);
                tmp.insert(rotOrder[1]);
                tmp.insert(rotOrder[2]);
                if (tmp.lastRotation == "LL") predictedCase = 1;
                else if (tmp.lastRotation == "RR") predictedCase = 2;
                else if (tmp.lastRotation == "LR") predictedCase = 3;
                else predictedCase = 4;
                scoreSystem.reset(1);
                title = "Level 3.4: Auto-Mode & Prediction";
                objective = "Key " + to_string(newKey) + " is about to be inserted. Predict: [1] LL, [2] RR, [3] LR, [4] RL.";
                instruction = "Press [1], [2], [3], or [4] to predict the rebalancing case.";
                hintText = "HINT: Trace where " + to_string(newKey) + " will land relative to the unbalanced ancestor (Left-Left, Right-Right, etc.).";
            }
            else if (subLevel == 5) {
                int v[7];
                makeDistinct(v, 7, 10, 99);
                for (int i = 0; i < 7; i++) act3Tree.insert(v[i]);
                int k = 2 + rnd(5);
                TreeNode* cur = act3Tree.root;
                int kk = k;
                while (cur != nullptr && pathLen < 10) {
                    pathExpected[pathLen++] = cur->value;
                    int leftSize = act3Tree.sizeOf(cur->left);
                    if (kk == leftSize + 1) break;
                    if (kk <= leftSize) cur = cur->left;
                    else {
                        kk = kk - leftSize - 1;
                        cur = cur->right;
                    }
                }
                scoreSystem.reset(pathLen);
                title = "Level 3.5: Boss - Order-Statistic Heist";
                objective = "Steal the " + to_string(k) + "th SMALLEST treasure in O(log N) using subtreeSize badges.";
                instruction = "Click rooms from Root using subtree sizes.";
                hintText = "HINT: Compare k against (left subtree size + 1): if smaller go left, if equal stop, if larger go right (k decreases).";
            }
        }
        // ================= ACT 4 =================
        else if (act == 4) {
            act4Heap.total = 0;
            act4Heap.heapSize = 0;
            act4Heap.swaps = 0;
            act4Heap.comparisons = 0;
            MaxHeap tmp;
            int arr[10];

            if (subLevel == 1) {
                int n = 7;
                makeDistinct(arr, n, 10, 99);
                if (rnd(2) == 1) sortArray(arr, n);
                int guard = 0;
                while (isMaxHeapArray(arr, n) && guard++ < 50) shuffleArray(arr, n);
                loadHeap(arr, n);
                tmp.total = n;
                for (int i = 0; i < n; i++) tmp.data[i] = arr[i];
                tmp.buildMaxHeap();
                scoreSystem.reset(tmp.swaps);
                title = "Level 4.1: Activate the Vault (Build Max Heap)";
                objective = "Click two nodes to SWAP them until the Max Heap property holds.";
                instruction = "Click two active nodes to swap them.";
                hintText = "HINT: In a Max Heap, every parent must be >= both of its children. Fix bottom-up.";
            }
            else if (subLevel == 2) {
                int n = 5 + rnd(3);
                makeDistinct(arr, n, 20, 99);
                tmp.total = n;
                for (int i = 0; i < n; i++) tmp.data[i] = arr[i];
                tmp.buildMaxHeap();
                for (int i = 0; i < n; i++) arr[i] = tmp.data[i];
                arr[0] = 1 + rnd(9);
                loadHeap(arr, n);
                tmp.swaps = 0;
                for (int i = 0; i < n; i++) tmp.data[i] = arr[i];
                tmp.heapSize = n;
                tmp.siftDown(0, n);
                scoreSystem.reset(tmp.swaps);
                title = "Level 4.2: Repair Security Locks (Sift-Down)";
                objective = "The root was disturbed. Restore the Max Heap by swapping nodes.";
                instruction = "Click two nodes to swap and repair the heap.";
                hintText = "HINT: Sift-down always swaps a violating parent with its LARGER child.";
            }
            else if (subLevel == 3) {
                int n = 7;
                makeDistinct(arr, n, 10, 99);
                tmp.total = n;
                for (int i = 0; i < n; i++) tmp.data[i] = arr[i];
                tmp.buildMaxHeap();
                for (int i = 0; i < n; i++) arr[i] = tmp.data[i];
                loadHeap(arr, n);
                tmp.swaps = 0;
                tmp.extractMax();
                extractsRemaining = 1;
                scoreSystem.reset(tmp.swaps);
                title = "Level 4.3: Extract the Greatest Treasure";
                objective = "Press [E] to extract the Max element, then restore the Max Heap.";
                instruction = "Press [E] to extract, then click nodes to heapify.";
                hintText = "HINT: Extract-Max swaps the root with the last active element, shrinks heapSize, then sifts down.";
            }
            else if (subLevel == 4) {
                int n = 5 + rnd(2);
                makeDistinct(arr, n, 10, 99);
                tmp.total = n;
                for (int i = 0; i < n; i++) tmp.data[i] = arr[i];
                tmp.buildMaxHeap();
                for (int i = 0; i < n; i++) arr[i] = tmp.data[i];
                loadHeap(arr, n);
                tmp.swaps = 0;
                tmp.extractMax();
                tmp.extractMax();
                extractsRemaining = 2;
                scoreSystem.reset(tmp.swaps);
                title = "Level 4.4: Race Against the Guards";
                objective = "Complete 2 Extract-Max [E] + Heapify cycles before the alarm fills.";
                instruction = "Press [E] to extract, then swap to restore the heap.";
                hintText = "HINT: Always restore the Max Heap property before pressing [E] for the next extraction.";
            }
            else if (subLevel == 5) {
                int n = 6;
                int kind = rnd(4);
                if (kind == 3) {
                    for (int i = 0; i < n; i++) arr[i] = 10 + rnd(5);
                }
                else {
                    makeDistinct(arr, n, 10, 99);
                    if (kind == 1) sortArray(arr, n);
                    if (kind == 2) {
                        sortArray(arr, n);
                        for (int i = 0; i < n / 2; i++) {
                            int t = arr[i];
                            arr[i] = arr[n - 1 - i];
                            arr[n - 1 - i] = t;
                        }
                    }
                }
                loadHeap(arr, n);
                tmp.total = n;
                for (int i = 0; i < n; i++) tmp.data[i] = arr[i];
                tmp.heapSort();
                scoreSystem.reset(tmp.swaps > 0 ? tmp.swaps : 1);
                title = "Level 4.5: Final Vault Boss - Heap Sort";
                objective = "Sort the entire array ascending using Build Max Heap + Extract [E] + Heapify.";
                instruction = "Swap to build Max Heap, then repeat [E] and sift-down.";
                hintText = "HINT: Heap Sort = 1) Build Max Heap, 2) Repeatedly Extract-Max [E] and sift-down until heapSize = 1.";
            }
        }

        title += "  [seed " + to_string(seed) + "]";
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
                        if (slotIdx < 0 || slotIdx > 6) return;
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
                            scoreSystem.addPenalty(1, 20);
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
            if (clickedId < 0 || clickedId >= act4Heap.total) return;
            if (clickedId >= act4Heap.heapSize) {
                scoreSystem.addPenalty(1, 20);
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
                bool isParentChild = (selectedHeapIdx == (clickedId - 1) / 2) || (clickedId == (selectedHeapIdx - 1) / 2);
                if (!isParentChild) {
                    scoreSystem.addPenalty(1, 20);
                    selectedHeapIdx = -1;
                    instruction = "Invalid swap! Only swap a parent with its child. Alarm +1.";
                    checkAlarm();
                    return;
                }

                scoreSystem.addStep();
                act4Heap.swapItems(selectedHeapIdx, clickedId, "player");
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
                if (clickedVal >= 0 && clickedVal < 100 && enteredHeights[clickedVal] != -1) return;
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

        if (act == 1 && subLevel == 4 && lcaFound) return;
        if (act == 3 && (subLevel == 3 || subLevel == 4)) return;

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
                    instruction = "Correct LCA! Now press the number key for the path length (edges) between the two rooms.";
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
                        if (subLevel == 2) act2Tree.insert(newKey);
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
                if (selectedDeleteCase == 0) {
                    instruction = "Pick the deletion case first ([1], [2], or [3]).";
                    return;
                }
                if (clickedVal == replacementValue) {
                    scoreSystem.addStep();
                    act2Tree.deleteKey(deleteTarget);
                    correct = true;
                }
            }
            else if (subLevel == 4) {
                if (clickedVal >= rangeLo && clickedVal <= rangeHi) {
                    if (clickedVal >= 0 && clickedVal < 100 && !harvested[clickedVal]) {
                        scoreSystem.addStep();
                        harvested[clickedVal] = true;
                        rangeFound++;
                        instruction = "Harvested! Find remaining in-range rooms (" + to_string(rangeFound) + "/" + to_string(rangeTotal) + ").";
                    }
                    if (rangeFound == rangeTotal) correct = true;
                    else { checkAlarm(); return; }
                }
            }
        }
        // Act 3 Checks
        else if (act == 3) {
            if (subLevel == 1 && clickedVal == pivotValue) { scoreSystem.addStep(); correct = true; }
            else if (subLevel == 2 && clickedVal == pivotValue) {
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
            scoreSystem.addPenalty(1, 20);
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

        int digit = keyToDigit(key);

        // Act 1.3: Heights & Diameter
        if (act == 1 && subLevel == 3) {
            if (digit == -1) return;

            if (!allHeightsDone && selectedHeightNode != -1) {
                if (digit == expectedHeights[selectedHeightNode]) {
                    scoreSystem.addStep();
                    enteredHeights[selectedHeightNode] = digit;
                    selectedHeightNode = -1;
                    allHeightsDone = true;
                    for (int k = 0; k < treeN; k++) {
                        if (enteredHeights[treeIn[k]] == -1) allHeightsDone = false;
                    }
                    if (allHeightsDone) instruction = "All heights set! Now press the tree DIAMETER (longest path, in edges).";
                    else instruction = "Correct height! Click the next room.";
                }
                else {
                    scoreSystem.addPenalty(1, 20);
                    instruction = "Wrong height! Alarm +1.";
                }
                checkAlarm();
            }
            else if (allHeightsDone) {
                if (digit == expectedDiameter) {
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
        if (act == 1 && subLevel == 4 && lcaFound && digit != -1) {
            if (digit == expectedPathLen) {
                scoreSystem.addStep();
                finishWin("LCA & PATH LENGTH VERIFIED! Press [ENTER].");
            }
            else {
                scoreSystem.addPenalty(1, 20);
                instruction = "Wrong path length! Alarm +1.";
                checkAlarm();
            }
        }

        // Act 1.5: Auto-Mirror
        if (act == 1 && subLevel == 5 && key == sf::Keyboard::A) {
            scoreSystem.addStep();
            act1Tree.rebuildTree(treePre, treeIn, treeN);
            act1Tree.mirror();
            finishWin("Auto-mirror complete! Press [ENTER].");
        }

        // Act 2.3: Delete case selection
        if (act == 2 && subLevel == 3 && selectedDeleteCase == 0 && digit >= 1 && digit <= 3) {
            if (digit == deleteCase) {
                scoreSystem.addStep();
                selectedDeleteCase = deleteCase;
                if (deleteCase == 1) {
                    act2Tree.deleteKey(deleteTarget);
                    finishWin("LEAF REMOVED! Press [ENTER].");
                }
                else if (deleteCase == 2) {
                    instruction = "Correct case! Now click the child that takes its place.";
                }
                else {
                    instruction = "Correct case! Now click the Inorder Successor.";
                }
                checkAlarm();
            }
            else {
                scoreSystem.addPenalty(1, 20);
                instruction = "Wrong deletion case! Alarm +1.";
                checkAlarm();
            }
        }

        // Act 3.2: Manual single rotation
        if (act == 3 && subLevel == 2 && pivotSelected) {
            if (key == expectedKey1) {
                scoreSystem.addStep();
                act3Tree.clear();
                for (int i = 0; i < 3; i++) act3Tree.insert(rotOrder[i]);
                finishWin("ROTATION COMPLETE! Press [ENTER].");
            }
            else if (key == sf::Keyboard::L || key == sf::Keyboard::R) {
                scoreSystem.addPenalty(2, 50);
                instruction = "Wrong rotation choice! Alarm +2, -50 pts.";
                checkAlarm();
            }
        }

        // Act 3.3: Manual double rotation (LR or RL)
        if (act == 3 && subLevel == 3) {
            if (rotationStep == 0 && key == expectedKey1) {
                scoreSystem.addStep();
                rotationStep = 1;
                if (rotType == 2) buildPlain(rotKeys[2], rotKeys[1], rotKeys[0]);
                else buildPlain(rotKeys[0], rotKeys[1], rotKeys[2]);
                instruction = "1st rotation complete! Now perform the 2nd rotation.";
                checkAlarm();
            }
            else if (rotationStep == 1 && key == expectedKey2) {
                scoreSystem.addStep();
                act3Tree.clear();
                for (int i = 0; i < 3; i++) act3Tree.insert(rotOrder[i]);
                finishWin("DOUBLE ROTATION COMPLETE! Press [ENTER].");
            }
            else if (key == sf::Keyboard::L || key == sf::Keyboard::R) {
                scoreSystem.addPenalty(2, 50);
                instruction = "Wrong rotation choice! Alarm +2, -50 pts.";
                checkAlarm();
            }
        }

        // Act 3.4: Predict rotation before auto-insert
        if (act == 3 && subLevel == 4 && digit >= 1 && digit <= 4) {
            if (digit == predictedCase) {
                scoreSystem.addStep();
                act3Tree.insert(newKey);
                finishWin("CORRECT PREDICTION! Auto-rebalanced. Press [ENTER].");
            }
            else {
                scoreSystem.addPenalty(2, 50);
                instruction = "Wrong prediction! Alarm +2, -50 pts.";
                checkAlarm();
            }
        }

        // Act 4.3, 4.4, 4.5: Extract Max [E]
        if (act == 4 && subLevel >= 3 && key == sf::Keyboard::E) {
            if (!isActiveMaxHeap()) {
                scoreSystem.addPenalty(1, 20);
                instruction = "Invalid extraction! Restore Max Heap first. Alarm +1.";
                checkAlarm();
                return;
            }
            if (act4Heap.heapSize > 1) {
                scoreSystem.addStep();
                int last = act4Heap.heapSize - 1;
                act4Heap.swapItems(0, last, "extract");
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
        buildLevel(act, subLevel);
    }
};