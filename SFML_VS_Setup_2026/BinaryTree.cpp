#include "BinaryTree.h"

BinaryTree::BinaryTree() {
    root = nullptr;
    nextId = 1;
    events = nullptr;
}

BinaryTree::~BinaryTree() {
    destroy(root);
}

void BinaryTree::destroy(TreeNode* node) {
    if (node == nullptr) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

void BinaryTree::note(string type, int a, int b, int c, int oldVal, int newVal) {
    if (events != nullptr) {
        events->add(type, a, b, c, oldVal, newVal);
    }
}

// 1. Recursive Traversals
void BinaryTree::preorderRec(TreeNode* node, int outArr[], int& count) {
    if (node == nullptr) return;
    note("visit", node->id, -1, -1, 0, node->value);
    outArr[count++] = node->value;
    preorderRec(node->left, outArr, count);
    preorderRec(node->right, outArr, count);
}

void BinaryTree::inorderRec(TreeNode* node, int outArr[], int& count) {
    if (node == nullptr) return;
    inorderRec(node->left, outArr, count);
    note("visit", node->id, -1, -1, 0, node->value);
    outArr[count++] = node->value;
    inorderRec(node->right, outArr, count);
}

void BinaryTree::postorderRec(TreeNode* node, int outArr[], int& count) {
    if (node == nullptr) return;
    postorderRec(node->left, outArr, count);
    postorderRec(node->right, outArr, count);
    note("visit", node->id, -1, -1, 0, node->value);
    outArr[count++] = node->value;
}

// 2. Iterative Traversals (Using Custom Stack)
void BinaryTree::preorderIter(int outArr[], int& count) {
    count = 0;
    if (root == nullptr) return;

    Stack st;
    st.push(root);

    while (!st.isEmpty()) {
        TreeNode* curr = st.pop();
        outArr[count++] = curr->value;
        if (curr->right != nullptr) st.push(curr->right);
        if (curr->left != nullptr) st.push(curr->left);
    }
}

void BinaryTree::inorderIter(int outArr[], int& count) {
    count = 0;
    Stack st;
    TreeNode* curr = root;

    while (curr != nullptr || !st.isEmpty()) {
        while (curr != nullptr) {
            st.push(curr);
            curr = curr->left;
        }
        curr = st.pop();
        outArr[count++] = curr->value;
        curr = curr->right;
    }
}

void BinaryTree::postorderIter(int outArr[], int& count) {
    count = 0;
    if (root == nullptr) return;

    Stack s1;
    Stack s2;
    s1.push(root);

    while (!s1.isEmpty()) {
        TreeNode* curr = s1.pop();
        s2.push(curr);
        if (curr->left != nullptr) s1.push(curr->left);
        if (curr->right != nullptr) s1.push(curr->right);
    }

    while (!s2.isEmpty()) {
        outArr[count++] = s2.pop()->value;
    }
}

// 3. Level Order (Using Custom Queue)
void BinaryTree::levelOrder(int outArr[], int& count) {
    count = 0;
    if (root == nullptr) return;

    Queue q;
    q.enqueue(root);

    while (!q.isEmpty()) {
        TreeNode* curr = q.dequeue();
        outArr[count++] = curr->value;
        if (curr->left != nullptr) q.enqueue(curr->left);
        if (curr->right != nullptr) q.enqueue(curr->right);
    }
}

// 4. Height and Diameter
int BinaryTree::height(TreeNode* node) {
    if (node == nullptr) return 0;
    int lh = height(node->left);
    int rh = height(node->right);
    if (lh > rh) return 1 + lh;
    return 1 + rh;
}

int BinaryTree::treeHeight() {
    return height(root);
}

int BinaryTree::calcDiameter(TreeNode* node, int& maxDia) {
    if (node == nullptr) return 0;
    int lh = calcDiameter(node->left, maxDia);
    int rh = calcDiameter(node->right, maxDia);
    if (lh + rh > maxDia) {
        maxDia = lh + rh;
    }
    if (lh > rh) return 1 + lh;
    return 1 + rh;
}

int BinaryTree::getDiameter() {
    int maxDia = 0;
    calcDiameter(root, maxDia);
    return maxDia;
}

// 5. Lowest Common Ancestor (LCA)
TreeNode* BinaryTree::findLCA(TreeNode* node, int a, int b) {
    if (node == nullptr) return nullptr;
    if (node->value == a || node->value == b) return node;

    TreeNode* leftRes = findLCA(node->left, a, b);
    TreeNode* rightRes = findLCA(node->right, a, b);

    if (leftRes != nullptr && rightRes != nullptr) return node;
    if (leftRes != nullptr) return leftRes;
    return rightRes;
}

int BinaryTree::getLCA(int a, int b) {
    TreeNode* res = findLCA(root, a, b);
    if (res != nullptr) return res->value;
    return -1;
}

// 6. Mirror Vault
TreeNode* BinaryTree::mirrorSubtree(TreeNode* node) {
    if (node == nullptr) return nullptr;
    TreeNode* tLeft = mirrorSubtree(node->left);
    TreeNode* tRight = mirrorSubtree(node->right);

    node->left = tRight;
    node->right = tLeft;
    note("swap_value", node->id, -1, -1, 0, 0);
    return node;
}

void BinaryTree::mirror() {
    root = mirrorSubtree(root);
}

// 7. Reconstruction from Preorder + Inorder
TreeNode* BinaryTree::buildPreIn(int pre[], int in[], int preStart, int preEnd, int inStart, int inEnd) {
    if (preStart > preEnd || inStart > inEnd) return nullptr;

    int rootVal = pre[preStart];
    TreeNode* node = new TreeNode(nextId, rootVal);
    nextId++;

    int inIndex = -1;
    for (int i = inStart; i <= inEnd; i++) {
        if (in[i] == rootVal) {
            inIndex = i;
            break;
        }
    }

    int leftLen = inIndex - inStart;
    node->left = buildPreIn(pre, in, preStart + 1, preStart + leftLen, inStart, inIndex - 1);
    if (node->left != nullptr) node->left->parent = node;

    node->right = buildPreIn(pre, in, preStart + leftLen + 1, preEnd, inIndex + 1, inEnd);
    if (node->right != nullptr) node->right->parent = node;

    return node;
}

void BinaryTree::rebuildTree(int pre[], int in[], int size) {
    destroy(root);
    root = nullptr;
    nextId = 1;
    if (size > 0) {
        root = buildPreIn(pre, in, 0, size - 1, 0, size - 1);
    }
}

bool BinaryTree::checkSame(TreeNode* p, TreeNode* q) {
    if (p == nullptr && q == nullptr) return true;
    if (p == nullptr || q == nullptr) return false;
    if (p->value != q->value) return false;
    return checkSame(p->left, q->left) && checkSame(p->right, q->right);
}

bool BinaryTree::isSame(TreeNode* otherRoot) {
    return checkSame(root, otherRoot);
}