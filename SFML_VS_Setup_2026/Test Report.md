# Unit Test Report — Vault Heist: The Living Vault
**Authors:** Zubair Ashraf & Abdullah Saqib  
**Test File:** `tests.cpp` (Pure Model Testing — No GUI Dependencies)

## 1. Test Overview
All core data structures (`BinaryTree`, `BST`, `AVL`, `MaxHeap`) were tested independently of SFML using `tests.cpp`. Every test verifies structural invariants (`isValidBST`, `isValidAVL`, `checkHeights`, `checkSizes`, `checkParents`, `isMaxHeap`, `isSorted`) after operations.

## 2. Test Case Results

| Module | Test Case | Input / Scenario | Expected Result | Status |
| :--- | :--- | :--- | :--- | :--- |
| **BinaryTree** | Empty & Single Node | 0 nodes, 1 node (`42`) | Traversals return 0 and `[42]` | PASS |
| **BinaryTree** | Recursive vs. Iterative | 7-node full tree | Preorder, Inorder, Postorder match | PASS |
| **BinaryTree** | Reconstruction Round-Trip | Preorder + Inorder of 7-node tree | Rebuilt tree equals original (`isSame`) | PASS |
| **BinaryTree** | LCA, Diameter & Mirror | LCA(20,40), Diameter, double mirror | LCA = 30, Diameter = 4, restored tree | PASS |
| **BST** | Insert & Duplicates | 7 keys + duplicate `30` | Valid BST, duplicate rejected (`false`) | PASS |
| **BST** | Missing Key Delete | `deleteKey(999)` | Returns `false`, tree unchanged | PASS |
| **BST** | 3-Case & Root Deletion | Leaf (`20`), 1-child (`30`), 2-child (`70`), Root (`50`) | Keys removed, BST & parent links valid | PASS |
| **AVL** | All 4 Rotations | LL (`30,20,10`), RR (`10,20,30`), LR (`30,10,20`), RL (`10,30,20`) | Root becomes `20`, `isValidAVL()` true | PASS |
| **AVL** | Heavy-Child BF=0 Delete | Delete `80`, `70` from `{50,30,70,20,40,80}` | Rebalanced, heights & sizes valid | PASS |
| **AVL** | Order Statistics | 10 unsorted keys | `kth(i)` and `rank(x)` match sorted array | PASS |
| **MaxHeap** | Edge Cases | Empty & 1-element (`55`) arrays | Valid heap, sorted without crash | PASS |
| **MaxHeap** | Insert & ExtractMax | `{15,40,10,85,30,85}` | Max `85` extracted, remaining heap valid | PASS |
| **MaxHeap** | Heap Sort | Mixed/negative/duplicate & reverse-sorted arrays | Output matches reference sorted array | PASS |

## 3. Conclusion
All **29 unit tests passed (0 failed)**, confirming that the underlying data structures and algorithms are accurate, memory-safe, and ready for GUI integration.