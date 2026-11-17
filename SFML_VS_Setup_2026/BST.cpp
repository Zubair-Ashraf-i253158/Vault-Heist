#include "BST.h"

BST::BST() {
    root = nullptr;
    nextId = 1;
    events = nullptr;
    comparisons = 0;
}

BST::~BST() {
    destroy(root);
}

void BST::destroy(TreeNode* node) {
    if (node == nullptr) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

void BST::clear() {
    destroy(root);
    root = nullptr;
}

bool BST::isEmpty() {
    return (root == nullptr);
}

void BST::note(string type, int a, int b, int c, int oldVal, int newVal) {
    if (events != nullptr) {
        events->add(type, a, b, c, oldVal, newVal);
    }
}

bool BST::exists(int key) {
    TreeNode* cur = root;
    while (cur != nullptr) {
        if (cur->value == key) return true;
        if (key < cur->value) cur = cur->left;
        else cur = cur->right;
    }
    return false;
}

TreeNode* BST::insertNode(TreeNode* node, TreeNode* parent, int key) {
    // Base case: if tree is empty, create root
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

    // Iterative insertion to avoid stack overflow
    TreeNode* current = node;
    TreeNode* parentNode = nullptr;

    while (current != nullptr) {
        parentNode = current;
        comparisons++;

        if (key < current->value) {
            note("compare", current->id, key, -1, 0, 0);
            if (current->left == nullptr) {
                break;  // Found insertion point on left
            }
            current = current->left;
        }
        else if (key > current->value) {
            note("compare", current->id, key, 1, 0, 0);
            if (current->right == nullptr) {
                break;  // Found insertion point on right
            }
            current = current->right;
        }
        else {
            // Duplicate key
            return node;
        }
    }

    // Create the new node
    TreeNode* newNode = new TreeNode(nextId, key);
    nextId++;
    newNode->parent = parentNode;

    if (key < parentNode->value) {
        parentNode->left = newNode;
        note("insert", newNode->id, parentNode->id, 0, 0, key);
    }
    else if (key > parentNode->value) {
        parentNode->right = newNode;
        note("insert", newNode->id, parentNode->id, 1, 0, key);
    }

    return node;
}

bool BST::insert(int key) {
    if (exists(key)) return false;
    root = insertNode(root, nullptr, key);
    return true;
}

TreeNode* BST::search(int key) {
    TreeNode* cur = root;
    while (cur != nullptr) {
        comparisons++;
        if (key == cur->value) {
            note("compare", cur->id, key, 0, 0, 0);
            return cur;
        }
        if (key < cur->value) {
            note("compare", cur->id, key, -1, 0, 0);
            cur = cur->left;
        }
        else {
            note("compare", cur->id, key, 1, 0, 0);
            cur = cur->right;
        }
    }
    return nullptr;
}

TreeNode* BST::deleteNode(TreeNode* node, int key) {
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

        // Case 1: Leaf
        if (node->left == nullptr && node->right == nullptr) {
            note("unlink", node->id, parentId, side, 0, 0);
            delete node;
            return nullptr;
        }
        // Case 2: One child (right)
        else if (node->left == nullptr) {
            TreeNode* temp = node->right;
            temp->parent = node->parent;
            note("unlink", node->id, parentId, side, 0, 0);
            delete node;
            return temp;
        }
        // Case 2: One child (left)
        else if (node->right == nullptr) {
            TreeNode* temp = node->left;
            temp->parent = node->parent;
            note("unlink", node->id, parentId, side, 0, 0);
            delete node;
            return temp;
        }
        // Case 3: Two children
        else {
            TreeNode* temp = findMin(node->right);
            note("swap_value", node->id, temp->id, -1, node->value, temp->value);
            node->value = temp->value;
            node->right = deleteNode(node->right, temp->value);
        }
    }
    return node;
}

bool BST::deleteKey(int key) {
    if (!exists(key)) return false;
    root = deleteNode(root, key);
    return true;
}

TreeNode* BST::findMin(TreeNode* node) {
    if (node == nullptr) return nullptr;
    while (node->left != nullptr) node = node->left;
    return node;
}

TreeNode* BST::findMax(TreeNode* node) {
    if (node == nullptr) return nullptr;
    while (node->right != nullptr) node = node->right;
    return node;
}

TreeNode* BST::successor(int key) {
    TreeNode* cur = root;
    TreeNode* ans = nullptr;
    while (cur != nullptr) {
        if (key < cur->value) {
            ans = cur;
            cur = cur->left;
        }
        else {
            cur = cur->right;
        }
    }
    return ans;
}

TreeNode* BST::predecessor(int key) {
    TreeNode* cur = root;
    TreeNode* ans = nullptr;
    while (cur != nullptr) {
        if (key > cur->value) {
            ans = cur;
            cur = cur->right;
        }
        else {
            cur = cur->left;
        }
    }
    return ans;
}

void BST::rangeHelper(TreeNode* node, int lo, int hi, int outArr[], int& count) {
    if (node == nullptr) return;

    note("visit", node->id, -1, -1, 0, 0);

    if (node->value > lo) {
        rangeHelper(node->left, lo, hi, outArr, count);
    }
    else if (node->left != nullptr) {
        note("highlight_subtree", node->left->id, -1, 1, 0, 0);
    }

    if (node->value >= lo && node->value <= hi) {
        outArr[count] = node->value;
        count++;
        note("highlight_subtree", node->id, -1, 2, 0, 0);
    }

    if (node->value < hi) {
        rangeHelper(node->right, lo, hi, outArr, count);
    }
    else if (node->right != nullptr) {
        note("highlight_subtree", node->right->id, -1, 1, 0, 0);
    }
}

void BST::rangeQuery(int lo, int hi, int outArr[], int& count) {
    count = 0;
    rangeHelper(root, lo, hi, outArr, count);
}

int BST::countNodes(TreeNode* node) {
    if (node == nullptr) return 0;
    return 1 + countNodes(node->left) + countNodes(node->right);
}

TreeNode* BST::kthHelper(TreeNode* node, int k, int& counter) {
    if (node == nullptr) return nullptr;
    TreeNode* leftRes = kthHelper(node->left, k, counter);
    if (leftRes != nullptr) return leftRes;

    counter++;
    if (counter == k) return node;

    return kthHelper(node->right, k, counter);
}

TreeNode* BST::kthSmallest(int k) {
    int counter = 0;
    return kthHelper(root, k, counter);
}

TreeNode* BST::lcaBST(int keyA, int keyB) {
    TreeNode* cur = root;
    while (cur != nullptr) {
        if (keyA < cur->value && keyB < cur->value) cur = cur->left;
        else if (keyA > cur->value && keyB > cur->value) cur = cur->right;
        else return cur;
    }
    return nullptr;
}

int BST::height(TreeNode* node) {
    if (node == nullptr) return 0;
    int lh = height(node->left);
    int rh = height(node->right);
    if (lh > rh) return 1 + lh;
    return 1 + rh;
}

int BST::treeHeight() {
    return height(root);
}

void BST::inorderHelper(TreeNode* node, int outArr[], int& count) {
    if (node == nullptr) return;
    inorderHelper(node->left, outArr, count);
    outArr[count] = node->value;
    count++;
    inorderHelper(node->right, outArr, count);
}

void BST::inorderKeys(int outArr[], int& count) {
    count = 0;
    inorderHelper(root, outArr, count);
}

bool BST::isBST(TreeNode* node, long long minVal, long long maxVal) {
    if (node == nullptr) return true;
    if (node->value <= minVal || node->value >= maxVal) return false;
    return isBST(node->left, minVal, node->value) &&
        isBST(node->right, node->value, maxVal);
}

bool BST::isValidBST() {
    long long inf = 9223372036854775807LL;
    return isBST(root, -inf, inf);
}

bool BST::parentsOk(TreeNode* node) {
    if (node == nullptr) return true;
    if (node->left != nullptr && node->left->parent != node) return false;
    if (node->right != nullptr && node->right->parent != node) return false;
    return parentsOk(node->left) && parentsOk(node->right);
}

bool BST::checkParents() {
    if (root != nullptr && root->parent != nullptr) return false;
    return parentsOk(root);
}