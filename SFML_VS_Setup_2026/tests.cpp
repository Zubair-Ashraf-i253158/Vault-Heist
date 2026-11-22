#include <iostream>
#include <cstdlib>
#include "BinaryTree.h"
#include "BST.h"
#include "AVL.h"
#include "MaxHeap.h"

using namespace std;

int passed = 0;
int failed = 0;

void check(bool condition, string testName) {
    if (condition) {
        cout << "[PASS] " << testName << endl;
        passed++;
    }
    else {
        cout << "[FAIL] " << testName << endl;
        failed++;
    }
}

bool arraysEqual(int a[], int b[], int n) {
    for (int i = 0; i < n; i++) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

void simpleSort(int arr[], int n) {
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

// ================= 1. BINARY TREE TESTS =================
void testBinaryTree() {
    cout << "\n--- BinaryTree Tests ---" << endl;

    // Empty tree
    BinaryTree emptyTree;
    int a[20], b[20], ca = 0, cb = 0;
    emptyTree.preorderRec(emptyTree.root, a, ca);
    emptyTree.preorderIter(b, cb);
    check(ca == 0 && cb == 0, "Empty tree traversals");

    // Single node
    BinaryTree singleTree;
    int sp[1] = { 42 }, si[1] = { 42 };
    singleTree.rebuildTree(sp, si, 1);
    ca = cb = 0;
    singleTree.inorderRec(singleTree.root, a, ca);
    singleTree.inorderIter(b, cb);
    check(ca == 1 && cb == 1 && a[0] == 42 && b[0] == 42, "Single node traversal");

    // Full tree: recursive == iterative
    BinaryTree bt;
    int pre[7] = { 50, 30, 20, 40, 70, 60, 80 };
    int in[7] = { 20, 30, 40, 50, 60, 70, 80 };
    bt.rebuildTree(pre, in, 7);

    ca = cb = 0;
    bt.preorderRec(bt.root, a, ca);
    bt.preorderIter(b, cb);
    check(ca == 7 && arraysEqual(a, b, 7), "Preorder recursive == iterative");

    ca = cb = 0;
    bt.inorderRec(bt.root, a, ca);
    bt.inorderIter(b, cb);
    check(ca == 7 && arraysEqual(a, b, 7), "Inorder recursive == iterative");

    ca = cb = 0;
    bt.postorderRec(bt.root, a, ca);
    bt.postorderIter(b, cb);
    check(ca == 7 && arraysEqual(a, b, 7), "Postorder recursive == iterative");

    // Reconstruction round trip
    BinaryTree rebuilt;
    int rPre[20], rIn[20], cPre = 0, cIn = 0;
    bt.preorderIter(rPre, cPre);
    bt.inorderIter(rIn, cIn);
    rebuilt.rebuildTree(rPre, rIn, cPre);
    check(bt.isSame(rebuilt.root), "Reconstruction round-trip matches original");

    // LCA, Diameter, Mirror
    check(bt.getLCA(20, 40) == 30, "BinaryTree LCA(20, 40) == 30");
    check(bt.getDiameter() == 4, "BinaryTree Diameter == 4");
    bt.mirror();
    bt.mirror();
    check(bt.isSame(rebuilt.root), "Double mirror restores original tree");
}

// ================= 2. BST TESTS =================
void testBST() {
    cout << "\n--- BST Tests ---" << endl;

    BST bst;
    int keys[7] = { 50, 30, 70, 20, 40, 60, 80 };
    for (int i = 0; i < 7; i++) bst.insert(keys[i]);

    check(bst.isValidBST() && bst.checkParents(), "BST valid after inserts");
    check(!bst.insert(30), "BST rejects duplicate insert");

    // Delete missing key
    check(!bst.deleteKey(999), "BST delete missing key returns false");

    // Delete leaf (20)
    check(bst.deleteKey(20) && bst.search(20) == nullptr && bst.isValidBST(), "BST delete leaf");

    // Delete one child (30 now has only right child 40)
    check(bst.deleteKey(30) && bst.search(30) == nullptr && bst.isValidBST(), "BST delete one-child node");

    // Delete two children (70 has 60 and 80)
    check(bst.deleteKey(70) && bst.search(70) == nullptr && bst.isValidBST(), "BST delete two-children node");

    // Delete root (50)
    check(bst.deleteKey(50) && bst.search(50) == nullptr && bst.isValidBST(), "BST delete root");
}

// ================= 3. AVL TESTS =================
void testAVL() {
    cout << "\n--- AVL Tests ---" << endl;

    // LL Rotation
    AVL avlLL;
    avlLL.insert(30); avlLL.insert(20); avlLL.insert(10);
    check(avlLL.lastRotation == "LL" && avlLL.root->value == 20 && avlLL.isValidAVL(), "AVL LL rotation");

    // RR Rotation
    AVL avlRR;
    avlRR.insert(10); avlRR.insert(20); avlRR.insert(30);
    check(avlRR.lastRotation == "RR" && avlRR.root->value == 20 && avlRR.isValidAVL(), "AVL RR rotation");

    // LR Rotation
    AVL avlLR;
    avlLR.insert(30); avlLR.insert(10); avlLR.insert(20);
    check(avlLR.lastRotation == "LR" && avlLR.root->value == 20 && avlLR.isValidAVL(), "AVL LR rotation");

    // RL Rotation
    AVL avlRL;
    avlRL.insert(10); avlRL.insert(30); avlRL.insert(20);
    check(avlRL.lastRotation == "RL" && avlRL.root->value == 20 && avlRL.isValidAVL(), "AVL RL rotation");

    // Duplicate rejection
    check(!avlLL.insert(20), "AVL rejects duplicate insert");

    // Deletion with BF == 0 on heavy child
    AVL avlDel;
    int vals[6] = { 50, 30, 70, 20, 40, 80 };
    for (int i = 0; i < 6; i++) avlDel.insert(vals[i]);
    avlDel.deleteKey(80);
    avlDel.deleteKey(70); // forces rotation where left child (30) has BF == 0
    check(avlDel.isValidAVL() && avlDel.checkHeights() && avlDel.checkSizes(), "AVL delete with BF 0 on heavy child");

    // Order statistics (kth and rank) vs sorted array
    AVL avlOS;
    int nums[10] = { 45, 12, 88, 33, 7, 91, 24, 60, 15, 72 };
    int sorted[10];
    for (int i = 0; i < 10; i++) {
        avlOS.insert(nums[i]);
        sorted[i] = nums[i];
    }
    simpleSort(sorted, 10);

    bool osOk = true;
    for (int i = 0; i < 10; i++) {
        TreeNode* node = avlOS.kth(i + 1);
        if (node == nullptr || node->value != sorted[i]) osOk = false;
        if (avlOS.rank(sorted[i]) != i + 1) osOk = false;
    }
    check(osOk && avlOS.isValidAVL(), "AVL kth() and rank() match sorted array");
}

// ================= 4. MAX HEAP & HEAP SORT TESTS =================
void testMaxHeap() {
    cout << "\n--- MaxHeap & Heap Sort Tests ---" << endl;

    // Empty & Single element
    MaxHeap hEmpty;
    hEmpty.total = 0; hEmpty.heapSize = 0;
    hEmpty.buildMaxHeap();
    hEmpty.heapSort();
    check(hEmpty.isMaxHeap() && hEmpty.isSorted(), "Heap empty array");

    MaxHeap hSingle;
    hSingle.data[0] = 55; hSingle.total = 1; hSingle.heapSize = 1;
    hSingle.buildMaxHeap();
    hSingle.heapSort();
    check(hSingle.data[0] == 55 && hSingle.isSorted(), "Heap single element");

    // Insert & ExtractMax
    MaxHeap hOps;
    int ins[6] = { 15, 40, 10, 85, 30, 85 }; // includes duplicate
    for (int i = 0; i < 6; i++) hOps.insert(ins[i]);
    check(hOps.isMaxHeap(), "Heap property holds after inserts with duplicates");
    int mx = hOps.extractMax();
    check(mx == 85 && hOps.isMaxHeap(), "ExtractMax returns max and keeps heap valid");

    // Heap Sort on negative, reverse-sorted, duplicate, and random inputs
    int testArr[10] = { 12, -5, 40, 40, 0, -20, 99, 7, 7, 3 };
    int expected[10];
    MaxHeap hSort;
    hSort.total = 10;
    for (int i = 0; i < 10; i++) {
        hSort.data[i] = testArr[i];
        expected[i] = testArr[i];
    }
    simpleSort(expected, 10);
    hSort.heapSort();
    check(hSort.isSorted() && arraysEqual(hSort.data, expected, 10), "Heap Sort matches reference on mixed/negative/duplicate array");

    // Reverse-sorted array
    MaxHeap hRev;
    hRev.total = 8;
    int revExp[8];
    for (int i = 0; i < 8; i++) {
        hRev.data[i] = 80 - i * 10;
        revExp[i] = 10 + i * 10;
    }
    hRev.heapSort();
    check(arraysEqual(hRev.data, revExp, 8), "Heap Sort on reverse-sorted array");
}

int main() {
    cout << "========================================" << endl;
    cout << "  VAULT HEIST - UNIT TEST SUITE" << endl;
    cout << "========================================" << endl;

    testBinaryTree();
    testBST();
    testAVL();
    testMaxHeap();

    cout << "\n========================================" << endl;
    cout << "  SUMMARY: " << passed << " PASSED, " << failed << " FAILED" << endl;
    cout << "========================================" << endl;
    return (failed == 0) ? 0 : 1;
}