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
#include <fstream>
#include <cstdlib>

using namespace std;

struct TargetHitbox { //har clickable node ki position aur size store karega
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

    //Endless Mode state
    bool isEndlessMode;
    int endlessTotalScore;
    int endlessSolved;

    ScoreSystem scoreSystem;

    TargetHitbox hitboxes[100];
    int hitboxCount;

    //Backend models
    BinaryTree act1Tree;
    BST act2Tree;
    AVL act3Tree;
    MaxHeap act4Heap;
    EventLog logger;

    //Seed variables
    int seed;
    int baseSeed;
    int playCount;

    //Act 1 common data random tree
    int treeN;
    int treePre[20];
    int treeIn[20];
    BinaryTree act1Ref;

    //Act 1.1 Traversal
    int expectedSequence[30];
    int seqLength;
    int currentSeqIdx;

    //Act 1.2 Reconstruction
    int slotValues[7];
    int targetSlots[7];
    int tokenBank[7];
    bool tokenUsed[7];
    int selectedTokenIdx;

    //Act 1.3 Heights and Diameter indexed by room VALUE 10..99
    int enteredHeights[100];
    int expectedHeights[100];
    int selectedHeightNode;
    bool allHeightsDone;
    int expectedDiameter;

    //Act 1.4 LCA and Path
    int lcaA, lcaB, expectedLCA;
    int expectedPathLen;
    bool lcaFound;

    //Act 2.1, 2.2, 2.5, 3.5 Path Walking
    int pathExpected[10];
    int pathLen;
    int pathIdx;
    int newKey;

    //Act 2.3 Deletion
    int selectedDeleteCase;
    int deleteTarget;
    int deleteCase;
    int replacementValue;

    //Act 2.4 Range Raid
    int rangeLo, rangeHi;
    bool harvested[100];
    int rangeTotal, rangeFound;

    //Act 3 Rotations
    int pivotValue;
    bool pivotSelected;
    int rotationStep;
    sf::Keyboard::Key expectedKey1;
    sf::Keyboard::Key expectedKey2;
    int rotOrder[3];
    int rotKeys[3];
    int rotType;
    int predictedCase;

    //Act 4 Interactive Swaps and Extract
    int selectedHeapIdx;
    int extractsRemaining;

    LevelManager() //starting values set karega aur level 1.1 load karega
    {
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

        act1Tree.events = &logger;
        act2Tree.events = &logger;
        act3Tree.events = &logger;
        act4Heap.events = &logger;

        loadLevel(1, 1);
    }

    void registerHitbox(int id, int val, float x, float y, float r) //har node ka click area register karega
    {
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

    int rnd(int n) { //0 se n-1 tak random number dega
        return rand() % n;
    }

    void makeDistinct(int arr[], int n, int lo, int hi) //unique random numbers ka array banayega
    {
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

    void sortArray(int arr[], int n) //array ko ascending order mein sort karega
    {
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

    void shuffleArray(int arr[], int n) //array ke elements random order mein mix karega
    {
        for (int i = n - 1; i > 0; i--) {
            int j = rnd(i + 1);
            int t = arr[i];
            arr[i] = arr[j];
            arr[j] = t;
        }
    }

    void makePre(int in[], int lo, int hi, int pre[], int& idx) //inorder array se valid preorder banayega
    {
        if (lo > hi) return;
        int r = (lo + hi) / 2 + rnd(3) - 1;
        if (r < lo) r = lo;
        if (r > hi) r = hi;
        pre[idx] = in[r];
        idx++;
        makePre(in, lo, r - 1, pre, idx);
        makePre(in, r + 1, hi, pre, idx);
    }

    string listToString(int arr[], int n) //array ko readable string mein convert karega
    {
        string s = "(";
        for (int i = 0; i < n; i++) {
            s += to_string(arr[i]);
            if (i < n - 1) s += ", ";
        }
        return s + ")";
    }

    int keyToDigit(sf::Keyboard::Key key) //keyboard number key ko 0-9 int mein badlega
    {
        if (key >= sf::Keyboard::Num0 && key <= sf::Keyboard::Num9) return (int)key - (int)sf::Keyboard::Num0;
        if (key >= sf::Keyboard::Numpad0 && key <= sf::Keyboard::Numpad9) return (int)key - (int)sf::Keyboard::Numpad0;
        return -1;
    }

    int depthOf(TreeNode* node, int value, int d) //root se kisi node ki depth nikalega
    {
        if (node == nullptr) return -1;
        if (node->value == value) return d;
        int l = depthOf(node->left, value, d + 1);
        if (l != -1) return l;
        return depthOf(node->right, value, d + 1);
    }

    int countInternal(TreeNode* node) //jin nodes ke children hain unki ginti karega
    {
        if (node == nullptr) return 0;
        if (node->left == nullptr && node->right == nullptr) return 0;
        return 1 + countInternal(node->left) + countInternal(node->right);
    }

    void fillHeights(TreeNode* node) //har node ki sahi height (leaf = 1) store karega
    {
        if (node == nullptr) return;
        expectedHeights[node->value] = act1Tree.height(node);
        fillHeights(node->left);
        fillHeights(node->right);
    }

    int findUnbalanced(TreeNode* node) //jis node ka balance factor kharab hai uski value dega
    {
        if (node == nullptr) return -1;
        int bf = act3Tree.balanceFactor(node);
        if (bf > 1 || bf < -1) return node->value;
        int l = findUnbalanced(node->left);
        if (l != -1) return l;
        return findUnbalanced(node->right);
    }

    TreeNode* findNodeBST(int key) //BST mein key dhoond kar node pointer dega
    {
        TreeNode* cur = act2Tree.root;
        while (cur != nullptr) {
            if (key == cur->value) return cur;
            if (key < cur->value) cur = cur->left;
            else cur = cur->right;
        }
        return nullptr;
    }

    void buildPlain(int a, int b, int c) //bina auto-balance kiye 3 nodes ka unbalanced tree banayega
    {
        int keys[3] = { a, b, c };
        act3Tree.clear();
        for (int i = 0; i < 3; i++)
            act3Tree.root = act3Tree.BST::insertNode(act3Tree.root, nullptr, keys[i]);
        act3Tree.recomputeAll(act3Tree.root);
    }

    void makeRotOrder(int type, int s[], int order[]) //LL, RR, LR ya RL case ke liye insert order banayega
    {
        if (type == 0) { order[0] = s[2]; order[1] = s[1]; order[2] = s[0]; }
        else if (type == 1) { order[0] = s[0]; order[1] = s[1]; order[2] = s[2]; }
        else if (type == 2) { order[0] = s[2]; order[1] = s[0]; order[2] = s[1]; }
        else { order[0] = s[0]; order[1] = s[2]; order[2] = s[1]; }
    }

    void loadHeap(int arr[], int n) //array ko act4Heap mein copy karega
    {
        for (int i = 0; i < n; i++) act4Heap.data[i] = arr[i];
        act4Heap.total = n;
        act4Heap.heapSize = n;
    }

    bool isMaxHeapArray(int arr[], int n) //check karega ke array pehle se max heap to nahi
    {
        for (int i = 1; i < n; i++) {
            if (arr[i] > arr[(i - 1) / 2]) return false;
        }
        return true;
    }

    // ================= WIN / ALARM / MODEL CHECKS =================
    void finishWin(string msg) //level win mark karega aur stars calculate karega
    {
        if (scoreSystem.isAlarmFull()) {
            levelLost = true;
            return;
        }
        levelWon = true;
        scoreSystem.calculateStars();
        if (usedHint && scoreSystem.stars > 2) {
            scoreSystem.stars = 2; //hint use karne par max 2 stars milenge
        }
        instruction = msg;
    }

    void checkAlarm() //alarm full hone par level lose karega
    {
        if (!levelWon && scoreSystem.isAlarmFull()) {
            levelLost = true;
        }
    }

    bool isActiveMaxHeap() //check karega ke active heap valid Max Heap hai ya nahi
    {
        for (int i = 0; i < act4Heap.heapSize; i++) {
            int l = 2 * i + 1;
            int r = 2 * i + 2;
            if (l < act4Heap.heapSize && act4Heap.data[i] < act4Heap.data[l]) return false;
            if (r < act4Heap.heapSize && act4Heap.data[i] < act4Heap.data[r]) return false;
        }
        return true;
    }

    bool isArraySorted() //check karega ke poora array ascending sort ho gaya ya nahi
    {
        for (int i = 0; i < act4Heap.total - 1; i++) {
            if (act4Heap.data[i] > act4Heap.data[i + 1]) return false;
        }
        return true;
    }

    bool swapChildrenByVal(TreeNode* node, int val) //clicked node ke left aur right child swap karega
    {
        if (node == nullptr) return false;
        if (node->value == val) {
            TreeNode* temp = node->left;
            node->left = node->right;
            node->right = temp;
            return true;
        }
        return swapChildrenByVal(node->left, val) || swapChildrenByVal(node->right, val);
    }

    bool isAct1Mirrored() //player ke tree ko hidden mirrored answer se compare karega
    {
        return act1Tree.isSame(act1Ref.root);
    }

    // ================= LEVEL LOADING =================
    int getSeedFromJSON(int targetAct, int targetSub) //Level.json file se current level ka seed parhega
    {
        ifstream file("Level.json");
        if (!file.is_open()) {
            return baseSeed + targetAct * 1000 + targetSub * 100; //file na mile to fallback seed
        }

        string line;
        while (getline(file, line)) {
            int posAct = (int)line.find("\"act\"");
            int posLvl = (int)line.find("\"level\"");
            int posSeed = (int)line.find("\"seed\"");

            if (posAct != -1 && posLvl != -1 && posSeed != -1) {
                int fAct = stoi(line.substr(line.find(":", posAct) + 1));
                int fSub = stoi(line.substr(line.find(":", posLvl) + 1));
                int fSeed = stoi(line.substr(line.find(":", posSeed) + 1));

                if (fAct == targetAct && fSub == targetSub) {
                    file.close();
                    return fSeed;
                }
            }
        }

        file.close();
        return baseSeed + targetAct * 1000 + targetSub * 100;
    }

    void loadLevel(int a, int sl) //seed choose karke level build karega
    {
        if (isEndlessMode) {
            seed = baseSeed + playCount * 7; //endless mode mein har baar naya seed
            playCount++;
        }
        else {
            seed = getSeedFromJSON(a, sl); //campaign mein Level.json se fixed seed
        }
        buildLevel(a, sl);
    }

    void loadRandomEndlessPuzzle() //endless mode ke liye random act ka puzzle load karega
    {
        int pick = rand() % 4;
        int a = 1, sl = 1;
        if (pick == 0) { a = 1; sl = 1; }      // Random Traversal
        else if (pick == 1) { a = 2; sl = 1; } // Random BST Search
        else if (pick == 2) { a = 3; sl = 2; } // Random AVL Single Rotation
        else { a = 4; sl = 1; }                // Random Build Max Heap

        loadLevel(a, sl);
        title = "ENDLESS " + to_string(endlessSolved + 1) +
            " (Total: " + to_string(endlessTotalScore) + ") | " + title;
        instruction += " ESC Save and Exit ";
    }

    void buildLevel(int a, int sl) //seed ke mutabiq poora level aur uske answers generate karega
    {
        srand(seed);
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
        if (act == 1) //Binary Tree ke levels setup karega
        {
            if (subLevel == 2) //tree reconstruction level
            {
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
                title = "Level 1.2 Decode the Blueprint ";
                if (usePost) {
                    objective = "Postorder: " + listToString(post, 7) + " | Inorder: " + listToString(treeIn, 7) + " ";
                    hintText = "HINT: Postorder = Left, Right, Root | Inorder = Left, Root, Right ";
                }
                else {
                    objective = "Preorder: " + listToString(treePre, 7) + " | Inorder: " + listToString(treeIn, 7) + " ";
                    hintText = "HINT: Preorder = Root, Left, Right | Inorder = Left, Root, Right ";
                }
                instruction = "Click a token below, then click its ? slot in the tree ";
            }
            else //baqi act 1 levels ke liye 7 se 9 nodes ka random tree banayega
            {
                treeN = 7 + rnd(3);
                makeDistinct(treeIn, treeN, 10, 99);
                int idx = 0;
                makePre(treeIn, 0, treeN - 1, treePre, idx);
                act1Tree.rebuildTree(treePre, treeIn, treeN);

                if (subLevel == 1) //random traversal order select karega
                {
                    int type = rnd(4);
                    int cnt = 0;
                    string name = "";
                    if (type == 0) {
                        act1Tree.preorderIter(expectedSequence, cnt); name = "PREORDER";
                        hintText = "HINT: Preorder = Root, Left, Right ";
                    }
                    else if (type == 1) {
                        act1Tree.inorderIter(expectedSequence, cnt); name = "INORDER";
                        hintText = "HINT: Inorder = Left, Root, Right ";
                    }
                    else if (type == 2) {
                        act1Tree.postorderIter(expectedSequence, cnt); name = "POSTORDER";
                        hintText = "HINT: Postorder = Left, Right, Root ";
                    }
                    else {
                        act1Tree.levelOrder(expectedSequence, cnt); name = "LEVEL ORDER";
                        hintText = "HINT: Level order visits left to right ";
                    }
                    seqLength = cnt;
                    scoreSystem.reset(cnt);
                    title = "Level 1.1: Guard Patrol Route ";
                    objective = "Predict the patrol: Click all rooms in " + name + " ";
                    instruction = " ";
                }
                else if (subLevel == 3) //node heights aur diameter setup karega
                {
                    fillHeights(act1Tree.root);
                    expectedDiameter = act1Tree.getDiameter();
                    scoreSystem.reset(treeN + 1);
                    title = "Level 1.3: Alarm Spread and Diameter ";
                    objective = "Click each room and press a number key for its height, then enter the Diameter ";
                    instruction = "Select a room and enter its height ";
                    hintText = "HINT: Leaf height = 1 | Parent height = 1 + max(left, right) | Diameter = longest path in edges ";
                }
                else if (subLevel == 4) //LCA aur path length setup karega
                {
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
                    title = "Level 1.4: Meeting Room (LCA) ";
                    objective = "Find the LCA of Room " + to_string(lcaA) + " and Room " + to_string(lcaB) + ", then enter the path length ";
                    instruction = "Click the meeting room (LCA) ";
                    hintText = "HINT: LCA = Lowest Common Ancestor | Path Length = Edges ";
                }
                else if (subLevel == 5) //mirror tree answer ready karega
                {
                    act1Ref.rebuildTree(treePre, treeIn, treeN);
                    act1Ref.mirror();
                    scoreSystem.reset(countInternal(act1Tree.root));
                    title = "Level 1.5: Mirror the Vault ";
                    objective = "Invert the vault: Click nodes to swap their children (or press A for Auto) ";
                    instruction = "Click parent rooms to swap left and right branches ";
                    hintText = "HINT: Mirroring swaps the left and right child of every internal node ";
                }
            }
        }
        // ================= ACT 2 =================
        else if (act == 2) //BST ke levels setup karega
        {
            int vals[7];
            int pre[7];
            makeDistinct(vals, 7, 10, 99);
            sortArray(vals, 7);
            int idx = 0;
            makePre(vals, 0, 6, pre, idx);
            act2Tree.clear();
            for (int i = 0; i < 7; i++) act2Tree.insert(pre[i]);

            if (subLevel == 1) //BST search path nikalega
            {
                int target = vals[rnd(7)];
                TreeNode* cur = act2Tree.root;
                while (cur != nullptr && pathLen < 10) {
                    pathExpected[pathLen++] = cur->value;
                    if (target == cur->value) break;
                    cur = (target < cur->value) ? cur->left : cur->right;
                }
                scoreSystem.reset(pathLen);
                title = "Level 2.1: BST Search Heist ";
                objective = "Search for Room " + to_string(target) + " starting from the Root ";
                instruction = "Click each room along the search path ";
                hintText = "HINT: Smaller = Left | Larger = Right ";
            }
            else if (subLevel == 2) //BST insert path nikalega
            {
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
                title = "Level 2.2: Stash Loot (BST Insert) ";
                objective = "Insert Room " + to_string(newKey) + ": Follow the path from Root to its parent room ";
                instruction = "Click each room along the insert path ";
                hintText = "HINT: Smaller = Left | Larger = Right until empty spot ";
            }
            else if (subLevel == 3) //BST delete case aur replacement nikalega
            {
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
                title = "Level 2.3: Steal and Collapse (BST Delete) ";
                objective = "Delete Room " + to_string(deleteTarget) + ": Press 1 for Leaf, 2 for One-Child, or 3 for Two-Children ";
                instruction = "Pick the delete case using 1, 2, or 3 ";
                hintText = "HINT: Leaf = Remove | One Child = Child replaces | Two Children = Inorder Successor ";
            }
            else if (subLevel == 4) //range query ke liye lo aur hi set karega
            {
                rangeLo = vals[0] + 1 + rnd(vals[1] - vals[0]);
                int top = 3 + rnd(2);
                rangeHi = vals[top] + rnd(vals[top + 1] - vals[top]);
                for (int i = 0; i < 7; i++) {
                    if (vals[i] >= rangeLo && vals[i] <= rangeHi) rangeTotal++;
                }
                scoreSystem.reset(rangeTotal);
                title = "Level 2.4: Range Raid (" + to_string(rangeLo) + " to " + to_string(rangeHi) + ") ";
                objective = "Collect all rooms between " + to_string(rangeLo) + " and " + to_string(rangeHi) + " ";
                instruction = "Click every room inside the range ";
                hintText = "HINT: Only click rooms from " + to_string(rangeLo) + " to " + to_string(rangeHi) + " ";
            }
            else if (subLevel == 5) //sorted inserts se skewed BST boss banayega
            {
                int five[5];
                makeDistinct(five, 5, 10, 99);
                sortArray(five, 5);
                act2Tree.clear();
                for (int i = 0; i < 5; i++) act2Tree.insert(five[i]);
                for (int i = 0; i < 5; i++) pathExpected[i] = five[i];
                pathLen = 5;
                scoreSystem.reset(5);
                title = "Level 2.5: Boss - The Lazy Architect ";
                objective = "Sorted inserts broke the BST! Search for Room " + to_string(five[4]) + " before the alarm fills ";
                instruction = "Follow the path from Root to Room " + to_string(five[4]) + " ";
                hintText = "HINT: Sorted inserts make a straight chain, so search takes O(N) steps ";
            }
        }        // ================= ACT 3 =================
        else if (act == 3) //AVL tree ke levels setup karega
        {
            act3Tree.clear();
            int s[3];
            makeDistinct(s, 3, 10, 99); //3 alag random numbers banayega
            sortArray(s, 3);

            if (subLevel == 1) //unbalanced node dhoondna
            {
                int type = rnd(4);
                makeRotOrder(type, s, rotOrder);
                buildPlain(rotOrder[0], rotOrder[1], rotOrder[2]); //bina balance kiye tree banayega
                pivotValue = findUnbalanced(act3Tree.root);
                scoreSystem.reset(1);
                title = "Level 3.1: Read the Badges ";
                objective = "Green badge = balanced | Red badge = violation | Click the unbalanced room ";
                instruction = "Check the badges and click the red room ";
                hintText = "HINT: Balance Factor = left height - right height | Red badge means |BF| > 1 ";
            }
            else if (subLevel == 2) //single rotation LL ya RR
            {
                rotType = rnd(2);
                for (int i = 0; i < 3; i++) rotKeys[i] = s[i];
                makeRotOrder(rotType, s, rotOrder);
                buildPlain(rotOrder[0], rotOrder[1], rotOrder[2]);
                pivotValue = findUnbalanced(act3Tree.root);
                expectedKey1 = (rotType == 0) ? sf::Keyboard::R : sf::Keyboard::L;
                scoreSystem.reset(2);
                title = "Level 3.2: Manual Repair - Single Rotation ";
                objective = "Fix the tree: Click the red pivot, then press L or R to rotate ";
                instruction = "Click the red pivot, then press L or R ";
                hintText = "HINT: LL needs Right rotation (R) | RR needs Left rotation (L) ";
            }
            else if (subLevel == 3) //double rotation LR ya RL
            {
                rotType = 2 + rnd(2);
                for (int i = 0; i < 3; i++) rotKeys[i] = s[i];
                makeRotOrder(rotType, s, rotOrder);
                buildPlain(rotOrder[0], rotOrder[1], rotOrder[2]);
                expectedKey1 = (rotType == 2) ? sf::Keyboard::L : sf::Keyboard::R;
                expectedKey2 = (rotType == 2) ? sf::Keyboard::R : sf::Keyboard::L;
                scoreSystem.reset(2);
                title = "Level 3.3: Manual Repair - Double Rotation ";
                objective = "Fix the zig-zag tree using two rotations (L or R) ";
                instruction = "Press L or R for 1st rotation, then 2nd ";
                hintText = "HINT: LR = Left (L) then Right (R) | RL = Right (R) then Left (L) ";
            }
            else if (subLevel == 4) //rotation case predict karna
            {
                int type = rnd(4);
                makeRotOrder(type, s, rotOrder);
                act3Tree.insert(rotOrder[0]);
                act3Tree.insert(rotOrder[1]);
                newKey = rotOrder[2];
                AVL tmp; //temp AVL se check karega konsi rotation hogi
                tmp.insert(rotOrder[0]);
                tmp.insert(rotOrder[1]);
                tmp.insert(rotOrder[2]);
                if (tmp.lastRotation == "LL") predictedCase = 1;
                else if (tmp.lastRotation == "RR") predictedCase = 2;
                else if (tmp.lastRotation == "LR") predictedCase = 3;
                else predictedCase = 4;
                scoreSystem.reset(1);
                title = "Level 3.4: Auto-Mode and Prediction ";
                objective = "Room " + to_string(newKey) + " will be inserted | Predict: 1 LL, 2 RR, 3 LR, 4 RL ";
                instruction = "Press 1, 2, 3, or 4 to predict the case ";
                hintText = "HINT: Check where Room " + to_string(newKey) + " lands to see if it is LL, RR, LR, or RL ";
            }
            else if (subLevel == 5) //k th smallest room find karna
            {
                int v[7];
                makeDistinct(v, 7, 10, 99);
                for (int i = 0; i < 7; i++) act3Tree.insert(v[i]); //7 nodes ka balanced AVL banayega
                int k = 2 + rnd(5);
                TreeNode* cur = act3Tree.root;
                int kk = k;
                while (cur != nullptr && pathLen < 10) //subtree size use karke path nikalega
                {
                    pathExpected[pathLen++] = cur->value;
                    int leftSize = act3Tree.sizeOf(cur->left);
                    if (kk == leftSize + 1) break;
                    if (kk <= leftSize) cur = cur->left;
                    else
                    {
                        kk = kk - leftSize - 1;
                        cur = cur->right;
                    }
                }
                scoreSystem.reset(pathLen);
                title = "Level 3.5: Boss - Order-Statistic Heist ";
                objective = "Find the " + to_string(k) + "th SMALLEST room using subtree size badges ";
                instruction = "Click rooms from Root using subtree sizes ";
                hintText = "HINT: Compare k with (left size + 1): smaller = Left | equal = Stop | larger = Right ";
            }
            }
            // ================= ACT 4 =================
        else if (act == 4) //Max Heap aur Heap Sort ke levels setup karega
        {
            act4Heap.total = 0;
            act4Heap.heapSize = 0;
            act4Heap.swaps = 0;
            act4Heap.comparisons = 0;
            MaxHeap tmp; //optimal swaps count karne ke liye temp heap
            int arr[10];

            if (subLevel == 1) //build max heap
            {
                int n = 7;
                makeDistinct(arr, n, 10, 99);
                if (rnd(2) == 1) sortArray(arr, n);
                int guard = 0;
                while (isMaxHeapArray(arr, n) && guard++ < 50) shuffleArray(arr, n); //ensure karega ke pehle se heap na ho
                loadHeap(arr, n);
                tmp.total = n;
                for (int i = 0; i < n; i++) tmp.data[i] = arr[i];
                tmp.buildMaxHeap();
                scoreSystem.reset(tmp.swaps);
                title = "Level 4.1: Activate the Vault (Build Max Heap) ";
                objective = "Click two nodes to swap them until it becomes a valid Max Heap ";
                instruction = "Click two active nodes to swap ";
                hintText = "HINT: Every parent must be >= both children | Fix from bottom to top ";
            }
            else if (subLevel == 2) //sift down repair
            {
                int n = 5 + rnd(3);
                makeDistinct(arr, n, 20, 99);
                tmp.total = n;
                for (int i = 0; i < n; i++) tmp.data[i] = arr[i];
                tmp.buildMaxHeap();
                for (int i = 0; i < n; i++) arr[i] = tmp.data[i];
                arr[0] = 1 + rnd(9); //root ko chota number dekar heap kharab karega
                loadHeap(arr, n);
                tmp.swaps = 0;
                for (int i = 0; i < n; i++) tmp.data[i] = arr[i];
                tmp.heapSize = n;
                tmp.siftDown(0, n);
                scoreSystem.reset(tmp.swaps);
                title = "Level 4.2: Repair Security Locks (Sift-Down) ";
                objective = "The root was disturbed | Fix the Max Heap by swapping nodes ";
                instruction = "Click two nodes to swap and fix the heap ";
                hintText = "HINT: Always swap a smaller parent with its LARGER child ";
            }
            else if (subLevel == 3) //single extract max
            {
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
                title = "Level 4.3: Extract the Greatest Treasure ";
                objective = "Press E to extract the Max element, then fix the Max Heap ";
                instruction = "Press E to extract, then click nodes to heapify ";
                hintText = "HINT: Extract-Max swaps root with last element, shrinks heap, then sifts down ";
            }
            else if (subLevel == 4) //double extract max
            {
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
                title = "Level 4.4: Race Against the Guards ";
                objective = "Complete 2 Extract Max (E) and Heapify cycles before alarm fills ";
                instruction = "Press E to extract, then swap to fix the heap ";
                hintText = "HINT: Fix the Max Heap completely before pressing E again ";
            }
            else if (subLevel == 5) //final boss heap sort
            {
                int n = 6;
                int kind = rnd(4); //random, ascending, descending ya duplicate array banayega
                if (kind == 3)
                {
                    for (int i = 0; i < n; i++) arr[i] = 10 + rnd(5);
                }
                else
                {
                    makeDistinct(arr, n, 10, 99);
                    if (kind == 1) sortArray(arr, n);
                    if (kind == 2)
                    {
                        sortArray(arr, n);
                        for (int i = 0; i < n / 2; i++)
                        {
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
                title = "Level 4.5: Final Vault Boss - Heap Sort ";
                objective = "Sort the whole array ascending using Build Max Heap, Extract (E), and Heapify ";
                instruction = "Swap to build Max Heap, then repeat E and sift down ";
                hintText = "HINT: Build Max Heap first, then keep pressing E and shift down until sorted ";
            }
            }

            title += " [seed " + to_string(seed) + "]"; //title ke sath level ka seed show karega
    }
    void handleNodeClick(float mouseX, float mouseY) //tree ya heap node par click handle karega
    {
        if (levelWon || levelLost) return; //level win ya lose ho chuka ho to click ignore karega

        //Act 1.2 Token Bank and Empty Slots
        if (act == 1 && subLevel == 2)
        {
            for (int i = 0; i < 7; i++) //neeche token bank mein check karega
            {
                if (tokenUsed[i]) continue;
                float tx = 200.0f + i * 100.0f;
                float ty = 580.0f;
                if (sqrt((mouseX - tx) * (mouseX - tx) + (mouseY - ty) * (mouseY - ty)) <= 24.0f)
                {
                    selectedTokenIdx = i; //clicked token select karega
                    instruction = "Selected token " + to_string(tokenBank[i]) + " Now click a ? slot ";
                    return;
                }
            }
            if (selectedTokenIdx != -1) //agar token select hai to tree slot check karega
            {
                for (int i = 0; i < hitboxCount; i++)
                {
                    float dx = mouseX - hitboxes[i].x;
                    float dy = mouseY - hitboxes[i].y;
                    if (sqrt(dx * dx + dy * dy) <= hitboxes[i].radius)
                    {
                        int slotIdx = hitboxes[i].id;
                        if (slotIdx < 0 || slotIdx > 6) return;
                        if (slotValues[slotIdx] != -1) return; //pehle se bhara slot skip karega
                        if (tokenBank[selectedTokenIdx] == targetSlots[slotIdx]) //sahi token sahi slot mein laga
                        {
                            scoreSystem.addStep();
                            slotValues[slotIdx] = tokenBank[selectedTokenIdx];
                            tokenUsed[selectedTokenIdx] = true;
                            selectedTokenIdx = -1;
                            instruction = "Token placed Select the next token ";
                            bool allDone = true;
                            for (int k = 0; k < 7; k++) if (slotValues[k] == -1) allDone = false;
                            if (allDone) finishWin("BLUEPRINT DECODED Press ENTER "); //saare slots fill hone par win
                        }
                        else //galat slot par alarm barhega
                        {
                            scoreSystem.addPenalty(1, 20);
                            instruction = "Wrong slot Alarm +1 ";
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

        for (int i = 0; i < hitboxCount; i++) //check karega ke kis node par click hua
        {
            float dx = mouseX - hitboxes[i].x;
            float dy = mouseY - hitboxes[i].y;
            if (sqrt(dx * dx + dy * dy) <= hitboxes[i].radius)
            {
                clickedId = hitboxes[i].id;
                clickedVal = hitboxes[i].value;
                hit = true;
                break;
            }
        }

        if (!hit) return; //khali jagah click ho to kuch nahi karega

        //Act 4 Interactive Swapping
        if (act == 4)
        {
            if (clickedId < 0 || clickedId >= act4Heap.total) return;
            if (clickedId >= act4Heap.heapSize) //sorted region lock hai
            {
                scoreSystem.addPenalty(1, 20);
                instruction = "Sorted region is locked Alarm +1 ";
                checkAlarm();
                return;
            }
            if (selectedHeapIdx == -1) //pehla node select karega
            {
                selectedHeapIdx = clickedId;
                instruction = "Selected " + to_string(act4Heap.data[clickedId]) + " Click another active node to swap ";
                return;
            }
            else if (selectedHeapIdx == clickedId) //dobara same node click par cancel
            {
                selectedHeapIdx = -1;
                instruction = "Selection cancelled ";
                return;
            }
            else //doosra node click hone par swap check karega
            {
                bool isParentChild = (selectedHeapIdx == (clickedId - 1) / 2) || (clickedId == (selectedHeapIdx - 1) / 2);
                if (!isParentChild) //sirf parent aur child swap ho sakte hain
                {
                    scoreSystem.addPenalty(1, 20);
                    selectedHeapIdx = -1;
                    instruction = "Invalid swap Only swap parent with child Alarm +1 ";
                    checkAlarm();
                    return;
                }

                scoreSystem.addStep();
                act4Heap.swapItems(selectedHeapIdx, clickedId, "player"); //dono nodes swap karega
                selectedHeapIdx = -1;

                if (subLevel == 1 || subLevel == 2) //build heap ya sift down levels
                {
                    if (isActiveMaxHeap()) finishWin("MAX HEAP VALID Press ENTER ");
                    else instruction = "Swapped Keep fixing the Max Heap ";
                }
                else if (subLevel == 3 || subLevel == 4) //extract max levels
                {
                    if (isActiveMaxHeap())
                    {
                        if (extractsRemaining <= 0) finishWin("EXTRACTION COMPLETE Press ENTER ");
                        else instruction = "Heap restored Press E for next Extract Max (" + to_string(extractsRemaining) + " left) ";
                    }
                    else instruction = "Swapped Keep sifting down ";
                }
                else if (subLevel == 5) //final boss heap sort
                {
                    if (act4Heap.heapSize <= 1 && isArraySorted()) finishWin("FINAL VAULT UNLOCKED Press ENTER ");
                    else if (isActiveMaxHeap()) instruction = "Max Heap valid Press E to Extract Max ";
                    else instruction = "Swapped Keep fixing the Max Heap ";
                }
                checkAlarm();
                return;
            }
        }

        //Act 1.3 Select node to enter height
        if (act == 1 && subLevel == 3)
        {
            if (!allHeightsDone)
            {
                if (clickedVal >= 0 && clickedVal < 100 && enteredHeights[clickedVal] != -1) return; //pehle se height enter ho to skip
                selectedHeightNode = clickedVal; //room select karega
                instruction = "Room " + to_string(clickedVal) + " selected Press a number key for its height ";
            }
            return;
        }

        // Act 1.5 Manual child swap
        if (act == 1 && subLevel == 5)
        {
            scoreSystem.addStep();
            swapChildrenByVal(act1Tree.root, clickedVal); //left aur right child swap karega
            if (isAct1Mirrored()) finishWin("VAULT MIRRORED Press ENTER ");
            else instruction = "Swapped children of Room " + to_string(clickedVal) + " ";
            checkAlarm();
            return;
        }

        if (act == 1 && subLevel == 4 && lcaFound) return; //LCA milne ke baad click ignore
        if (act == 3 && (subLevel == 3 || subLevel == 4)) return; //keyboard levels mein click ignore

        bool correct = false;

        //Act 1.1 1.4
        if (act == 1)
        {
            if (subLevel == 1) //traversal sequence check karega
            {
                if (clickedVal == expectedSequence[currentSeqIdx])
                {
                    scoreSystem.addStep();
                    currentSeqIdx++;
                    instruction = "Correct Progress " + to_string(currentSeqIdx) + "/" + to_string(seqLength) + " ";
                    if (currentSeqIdx == seqLength) correct = true;
                    else { checkAlarm(); return; }
                }
            }
            else if (subLevel == 4) //LCA node check karega
            {
                if (!lcaFound && clickedVal == expectedLCA)
                {
                    scoreSystem.addStep();
                    lcaFound = true;
                    instruction = "Correct LCA Now press the number key for path length ";
                    checkAlarm();
                    return;
                }
            }
        }
        //Act 2 Checks
        else if (act == 2)
        {
            if (subLevel == 1 || subLevel == 2 || subLevel == 5) //BST search ya insert path check karega
            {
                if (clickedVal == pathExpected[pathIdx])
                {
                    scoreSystem.addStep();
                    pathIdx++;
                    if (pathIdx == pathLen)
                    {
                        if (subLevel == 2) act2Tree.insert(newKey); //leaf milne par new key insert karega
                        correct = true;
                    }
                    else
                    {
                        instruction = "Good step Choose the next room ";
                        checkAlarm();
                        return;
                    }
                }
            }
            else if (subLevel == 3) //BST delete replacement node check karega
            {
                if (selectedDeleteCase == 0)
                {
                    instruction = "Pick the delete case first (1, 2, or 3) ";
                    return;
                }
                if (clickedVal == replacementValue)
                {
                    scoreSystem.addStep();
                    act2Tree.deleteKey(deleteTarget); //target delete karega
                    correct = true;
                }
            }
            else if (subLevel == 4) //range ke andar rooms collect karega
            {
                if (clickedVal >= rangeLo && clickedVal <= rangeHi)
                {
                    if (clickedVal >= 0 && clickedVal < 100 && !harvested[clickedVal])
                    {
                        scoreSystem.addStep();
                        harvested[clickedVal] = true;
                        rangeFound++;
                        instruction = "Collected Find remaining rooms (" + to_string(rangeFound) + "/" + to_string(rangeTotal) + ") ";
                    }
                    if (rangeFound == rangeTotal) correct = true;
                    else { checkAlarm(); return; }
                }
            }
        }
        // Act 3 Checks
        else if (act == 3)
        {
            if (subLevel == 1 && clickedVal == pivotValue) //unbalanced node check
            {
                scoreSystem.addStep();
                correct = true;
            }
            else if (subLevel == 2 && clickedVal == pivotValue) //rotation ke liye pivot select karega
            {
                scoreSystem.addStep();
                pivotSelected = true;
                instruction = "Pivot selected Press L or R to rotate ";
                checkAlarm();
                return;
            }
            else if (subLevel == 5) //k-th smallest path check karega
            {
                if (clickedVal == pathExpected[pathIdx])
                {
                    scoreSystem.addStep();
                    pathIdx++;
                    if (pathIdx == pathLen) correct = true;
                    else
                    {
                        instruction = "Correct branch Keep going down ";
                        checkAlarm();
                        return;
                    }
                }
            }
        }

        if (correct) //level complete hone par win
        {
            finishWin("SUCCESS Sector secured Press ENTER ");
        }
        else //galat node click karne par alarm barhega
        {
            scoreSystem.addPenalty(1, 20);
            instruction = "Wrong room (" + to_string(clickedVal) + ") Alarm +1 ";
            checkAlarm();
        }
    }

    void handleKeyPress(sf::Keyboard::Key key) //keyboard input handle karega
    {
        if (levelWon || levelLost) return; //level win ya lose ho chuka ho to key ignore karega

        // Global Hint Key H -> hint show karega aur star cap 2 karega
        if (key == sf::Keyboard::H)
        {
            usedHint = true;
            instruction = hintText + " (Star cap: 2) ";
            return;
        }

        int digit = keyToDigit(key); //pressed key ko number 0 sa 9 mein convert karega

        //Act 1.3 Heights & Diameter
        if (act == 1 && subLevel == 3)
        {
            if (digit == -1) return; //agar number key nahi to return

            if (!allHeightsDone && selectedHeightNode != -1) //selected room ki height check karega
            {
                if (digit == expectedHeights[selectedHeightNode])
                {
                    scoreSystem.addStep();
                    enteredHeights[selectedHeightNode] = digit;
                    selectedHeightNode = -1;
                    allHeightsDone = true;
                    for (int k = 0; k < treeN; k++) //check karega ke saare rooms ki height enter ho gayi
                    {
                        if (enteredHeights[treeIn[k]] == -1) allHeightsDone = false;
                    }
                    if (allHeightsDone) instruction = "All heights set Now enter the tree DIAMETER (edges) ";
                    else instruction = "Correct height Click the next room ";
                }
                else
                {
                    scoreSystem.addPenalty(1, 20);
                    instruction = "Wrong height Alarm +1 ";
                }
                checkAlarm();
            }
            else if (allHeightsDone) //saari heights ke baad diameter check karega
            {
                if (digit == expectedDiameter)
                {
                    scoreSystem.addStep();
                    finishWin("DIAMETER VERIFIED Press ENTER ");
                }
                else
                {
                    scoreSystem.addPenalty(1, 20);
                    instruction = "Wrong diameter Alarm +1 ";
                    checkAlarm();
                }
            }
        }

        //Act 1.4 Path length after LCA
        if (act == 1 && subLevel == 4 && lcaFound && digit != -1) //LCA milne ke baad path length check karega
        {
            if (digit == expectedPathLen)
            {
                scoreSystem.addStep();
                finishWin("LCA AND PATH VERIFIED Press ENTER ");
            }
            else
            {
                scoreSystem.addPenalty(1, 20);
                instruction = "Wrong path length Alarm +1 ";
                checkAlarm();
            }
        }

        //Act 1.5 Auto Mirror
        if (act == 1 && subLevel == 5 && key == sf::Keyboard::A) //A press karne par tree auto mirror karega
        {
            scoreSystem.addStep();
            act1Tree.rebuildTree(treePre, treeIn, treeN);
            act1Tree.mirror();
            finishWin("Auto mirror complete Press ENTER ");
        }

        //Act 2.3 Delete case selection
        if (act == 2 && subLevel == 3 && selectedDeleteCase == 0 && digit >= 1 && digit <= 3) //delete case 1, 2 ya 3 check karega
        {
            if (digit == deleteCase)
            {
                scoreSystem.addStep();
                selectedDeleteCase = deleteCase;
                if (deleteCase == 1) //leaf node seedha delete hoga
                {
                    act2Tree.deleteKey(deleteTarget);
                    finishWin("LEAF REMOVED Press ENTER ");
                }
                else if (deleteCase == 2) //one child case
                {
                    instruction = "Correct case Now click the child that replaces it ";
                }
                else //two children case
                {
                    instruction = "Correct case Now click the Inorder Successor ";
                }
                checkAlarm();
            }
            else
            {
                scoreSystem.addPenalty(1, 20);
                instruction = "Wrong delete case Alarm +1 ";
                checkAlarm();
            }
        }

        //Act 3.2 Manual single rotation
        if (act == 3 && subLevel == 2 && pivotSelected) //pivot select hone ke baad L ya R check karega
        {
            if (key == expectedKey1)
            {
                scoreSystem.addStep();
                act3Tree.clear();
                for (int i = 0; i < 3; i++) act3Tree.insert(rotOrder[i]); //AVL tree balance karega
                finishWin("ROTATION COMPLETE Press ENTER ");
            }
            else if (key == sf::Keyboard::L || key == sf::Keyboard::R)
            {
                scoreSystem.addPenalty(2, 50);
                instruction = "Wrong rotation Alarm +2 and -50 pts ";
                checkAlarm();
            }
        }

		//Act 3.3 Manual double rotation LR ya RL
        if (act == 3 && subLevel == 3) //double rotation ke 2 steps check karega
        {
            if (rotationStep == 0 && key == expectedKey1) //pehli rotation
            {
                scoreSystem.addStep();
                rotationStep = 1;
                if (rotType == 2) buildPlain(rotKeys[2], rotKeys[1], rotKeys[0]);
                else buildPlain(rotKeys[0], rotKeys[1], rotKeys[2]);
                instruction = "1st rotation done Now do the 2nd rotation ";
                checkAlarm();
            }
            else if (rotationStep == 1 && key == expectedKey2) //doosri rotation
            {
                scoreSystem.addStep();
                act3Tree.clear();
                for (int i = 0; i < 3; i++) act3Tree.insert(rotOrder[i]);
                finishWin("DOUBLE ROTATION COMPLETE Press ENTER ");
            }
            else if (key == sf::Keyboard::L || key == sf::Keyboard::R)
            {
                scoreSystem.addPenalty(2, 50);
                instruction = "Wrong rotation Alarm +2 and -50 pts ";
                checkAlarm();
            }
        }

        //Act 3.4 Predict rotation before auto insert
        if (act == 3 && subLevel == 4 && digit >= 1 && digit <= 4) //1 LL, 2 RR, 3 LR, 4 RL prediction check
        {
            if (digit == predictedCase)
            {
                scoreSystem.addStep();
                act3Tree.insert(newKey); //new key insert karke auto balance karega
                finishWin("CORRECT PREDICTION Auto balanced Press ENTER ");
            }
            else
            {
                scoreSystem.addPenalty(2, 50);
                instruction = "Wrong prediction Alarm +2 and -50 pts ";
                checkAlarm();
            }
        }

        //Act 4.3,4.4,4.5 Extract Max E
        if (act == 4 && subLevel >= 3 && key == sf::Keyboard::E) //E press karne par max element extract karega
        {
            if (!isActiveMaxHeap()) //extract se pehle max heap valid hona zaroori hai
            {
                scoreSystem.addPenalty(1, 20);
                instruction = "Invalid extract Fix Max Heap first Alarm +1 ";
                checkAlarm();
                return;
            }
            if (act4Heap.heapSize > 1) //root ko last active element se swap karke heapSize kam karega
            {
                scoreSystem.addStep();
                int last = act4Heap.heapSize - 1;
                act4Heap.swapItems(0, last, "extract");
                act4Heap.heapSize--;
                selectedHeapIdx = -1;

                if (subLevel == 3 || subLevel == 4) //extract max levels
                {
                    extractsRemaining--;
                    if (isActiveMaxHeap() && extractsRemaining <= 0)
                    {
                        finishWin("EXTRACTION COMPLETE Press ENTER ");
                    }
                    else
                    {
                        instruction = "Extracted Max Now sift down to fix Max Heap ";
                    }
                }
                else if (subLevel == 5) //final boss heap sort
                {
                    if (act4Heap.heapSize <= 1 && isArraySorted())
                    {
                        finishWin("FINAL VAULT UNLOCKED Heap Sort Complete Press ENTER ");
                    }
                    else if (isActiveMaxHeap())
                    {
                        instruction = "Extracted Press E to extract next Max ";
                    }
                    else
                    {
                        instruction = "Extracted Swap parent and child to fix Max Heap ";
                    }
                }
                checkAlarm();
            }
        }
    }

    void nextLevel() //next level ya next act load karega
    {
        if (!levelWon) return;
        if (subLevel < 5) loadLevel(act, subLevel + 1);
        else if (act < 4) loadLevel(act + 1, 1);
        else
        {
            campaignComplete = true; //saare 20 levels complete
        }
    }

    void retry() //same seed ke sath current level restart karega
    {
        buildLevel(act, subLevel);
    }
};