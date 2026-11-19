#include "AVL.h"

AVL::AVL() 
{
    rotationCount = 0;
    lastRotation = "none";
}

bool AVL::insert(int key) 
{
    if (exists(key)) return false;
    lastRotation = "none";
    root = insertNode(root, nullptr, key);
    return true;
}

TreeNode* AVL::insertNode(TreeNode* node, TreeNode* parent, int key) {
    if (node == nullptr) {
        TreeNode* n = new TreeNode(nextId, key);
        nextId++;
        n->parent = parent;

        if (parent == nullptr)
            note("insert", n->id, -1, -1, 0, key);
        else if (key < parent->value)
            note("insert", n->id, parent->id, 0, 0, key);
        else
            note("insert", n->id, parent->id, 1, 0, key);

        return n;
    }

    comparisons++;
    if (key < node->value) {
        note("compare", node->id, key, -1, 0, 0);
        node->left = insertNode(node->left, node, key);
    }
    else if (key > node->value) {
        note("compare", node->id, key, 1, 0, 0);
        node->right = insertNode(node->right, node, key);
    }
    else {
        return node;
    }

    return rebalance(node);
}

bool AVL::deleteKey(int key) 
{
    if (!exists(key)) return false;
    lastRotation = "none";
    root = deleteNode(root, key);
    return true;
}

TreeNode* AVL::deleteNode(TreeNode* node, int key) {
    if (node == nullptr) return nullptr;

    if (key < node->value) {
        note("compare", node->id, key, -1, 0, 0);
        node->left = deleteNode(node->left, key);
    }
    else if (key > node->value) {
        note("compare", node->id, key, 1, 0, 0);
        node->right = deleteNode(node->right, key);
    }
    else {
        int parentId = -1;
        int side = -1;
        if (node->parent != nullptr) {
            parentId = node->parent->id;
            if (node->parent->left == node) side = 0;
            else side = 1;
        }

        if (node->left == nullptr && node->right == nullptr) {
            note("unlink", node->id, parentId, side, 0, 0);
            delete node;
            return nullptr;
        }
        else if (node->left == nullptr) {
            TreeNode* temp = node->right;
            temp->parent = node->parent;
            note("unlink", node->id, parentId, side, 0, 0);
            delete node;
            return temp;
        }
        else if (node->right == nullptr) {
            TreeNode* temp = node->left;
            temp->parent = node->parent;
            note("unlink", node->id, parentId, side, 0, 0);
            delete node;
            return temp;
        }
        else {
            TreeNode* temp = findMin(node->right);
            note("swap_value", node->id, temp->id, -1, node->value, temp->value);
            node->value = temp->value;
            node->right = deleteNode(node->right, temp->value);
        }
    }

    return rebalance(node);
}

int AVL::height(TreeNode* node) \\ ya tree ki height ret kre ga
{
    if (node == nullptr) return 0;
    return node->height;
}

int AVL::sizeOf(TreeNode* node) {
    if (node == nullptr) return 0;
    return node->size;
}

int AVL::bigger(int a, int b) {
    if (a > b) return a;
    return b;
}

int AVL::balanceFactor(TreeNode* node) {
    if (node == nullptr) return 0;
    return height(node->left) - height(node->right);
}

void AVL::update(TreeNode* node) {
    if (node == nullptr) return;

    int oldH = node->height;
    int oldS = node->size;

    node->height = 1 + bigger(height(node->left), height(node->right));
    node->size = 1 + sizeOf(node->left) + sizeOf(node->right);

    if (oldH != node->height)
        note("update_height", node->id, -1, -1, oldH, node->height);

    if (oldS != node->size)
        note("update_size", node->id, -1, -1, oldS, node->size);
}

TreeNode* AVL::rotateRight(TreeNode* y) {
    TreeNode* x = y->left;
    TreeNode* T2 = x->right;

    x->parent = y->parent;
    x->right = y;
    y->parent = x;
    y->left = T2;

    if (T2 != nullptr) T2->parent = y;

    update(y);
    update(x);
    rotationCount++;
    return x;
}

TreeNode* AVL::rotateLeft(TreeNode* x) {
    TreeNode* y = x->right;
    TreeNode* T2 = y->left;

    y->parent = x->parent;
    y->left = x;
    x->parent = y;
    x->right = T2;

    if (T2 != nullptr) T2->parent = x;

    update(x);
    update(y);
    rotationCount++;
    return y;
}

TreeNode* AVL::rebalance(TreeNode* node) {
    if (node == nullptr) return nullptr;

    update(node);
    int balance = balanceFactor(node);

    if (balance > 1 || balance < -1) {
        note("balance_alert", node->id, balance, -1, 0, 0);
    }

    // LL Case
    if (balance > 1 && balanceFactor(node->left) >= 0) {
        TreeNode* top = rotateRight(node);
        lastRotation = "LL";
        note("rotate", node->id, top->id, 0, 0, 0);
        return top;
    }

    // LR Case
    if (balance > 1 && balanceFactor(node->left) < 0) {
        node->left = rotateLeft(node->left);
        TreeNode* top = rotateRight(node);
        lastRotation = "LR";
        note("rotate", node->id, top->id, 0, 0, 0);
        return top;
    }

    // RR Case
    if (balance < -1 && balanceFactor(node->right) <= 0) {
        TreeNode* top = rotateLeft(node);
        lastRotation = "RR";
        note("rotate", node->id, top->id, 0, 0, 0);
        return top;
    }

    // RL Case
    if (balance < -1 && balanceFactor(node->right) > 0) {
        node->right = rotateRight(node->right);
        TreeNode* top = rotateLeft(node);
        lastRotation = "RL";
        note("rotate", node->id, top->id, 0, 0, 0);
        return top;
    }

    return node;
}

TreeNode* AVL::kth(int k) {
    TreeNode* cur = root;
    while (cur != nullptr) {
        int leftSize = sizeOf(cur->left);
        note("visit", cur->id, -1, -1, 0, 0);

        if (k == leftSize + 1) return cur;

        if (k <= leftSize) cur = cur->left;
        else {
            k = k - leftSize - 1;
            cur = cur->right;
        }
    }
    return nullptr;
}

int AVL::rank(int key) {
    TreeNode* cur = root;
    int count = 0;
    while (cur != nullptr) {
        if (key >= cur->value) {
            count = count + sizeOf(cur->left) + 1;
            cur = cur->right;
        }
        else {
            cur = cur->left;
        }
    }
    return count;
}

int AVL::countInRange(int lo, int hi) {
    return rank(hi) - rank(lo - 1);
}

bool AVL::balancedCheck(TreeNode* node) {
    if (node == nullptr) return true;
    int balance = balanceFactor(node);
    if (balance > 1 || balance < -1) return false;
    return balancedCheck(node->left) && balancedCheck(node->right);
}

bool AVL::isBalanced() {
    return balancedCheck(root);
}

bool AVL::heightsOk(TreeNode* node) {
    if (node == nullptr) return true;
    if (node->height != BST::height(node)) return false;
    return heightsOk(node->left) && heightsOk(node->right);
}

bool AVL::checkHeights() {
    return heightsOk(root);
}

bool AVL::sizesOk(TreeNode* node) {
    if (node == nullptr) return true;
    if (node->size != countNodes(node)) return false;
    return sizesOk(node->left) && sizesOk(node->right);
}

bool AVL::checkSizes() {
    return sizesOk(root);
}

bool AVL::isValidAVL() {
    return isValidBST() && checkParents() && isBalanced() &&
        checkHeights() && checkSizes();
}

void AVL::recomputeAll(TreeNode* node) {
    if (node == nullptr) return;
    recomputeAll(node->left);
    recomputeAll(node->right);
    update(node);
}