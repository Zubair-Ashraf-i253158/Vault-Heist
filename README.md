# 🗝️ Vault Heist: Echoes of the Living Vault

<div align="center">

![Vault Heist Banner](SFML_VS_Setup_2026/assets/menu1.jpg)

**An interactive C++ puzzle-strategy game powered by custom Trees, AVL Rotations, and Max Heaps**

[![Made with SFML](https://img.shields.io/badge/Made%20with-SFML-00ADD8?style=for-the-badge&logo=cplusplus&logoColor=white)](https://www.sfml-dev.org/)
[![Language](https://img.shields.io/badge/Language-C%2B%2B17-blue?style=for-the-badge&logo=cplusplus)](https://en.cppreference.com/)
[![Platform](https://img.shields.io/badge/Platform-Windows-0078D6?style=for-the-badge&logo=windows)](https://www.microsoft.com/windows)
[![DSA Project](https://img.shields.io/badge/DSA-Semester%20Project-crimson?style=for-the-badge)](https://www.nu.edu.pk/)

---

### 👨‍💻 Developed by

| Developer | Roll No | Institution |
| :--- | :--- | :--- |
| **Zubair Ashraf** | 25I-3158 | FAST-NU Islamabad |
| **Abdullah Saqib** | 25I-2020 | FAST-NU Islamabad |

**Course:** Data Structures & Algorithms (CS2001)  
**University:** FAST National University of Computer and Emerging Sciences, Islamabad  
          **Video added in assets **
</div>

---

## 🚪 About the Game

**Vault Heist: Echoes of the Living Vault** is a puzzle-strategy game where data structures are not just lines of code—they are the dynamic chambers of a high-security vault. You play as a master thief navigating a vault that shifts, rebalances, and sounds the alarm as you move.

Every step, key comparison, and tree navigation advances the **Alarm Meter**. If a tree is skewed or unbalanced, the optimal escape path lengthens, triggering lockdown. To bypass the security barriers, the player must master traversals, binary search pathways, self-balancing AVL rotations, and complete Max Heap priority extractions.

> *"Zero STL containers. Built entirely from scratch with raw pointers, memory safety, and pure algorithmic logic."*

---

## 🎮 Core Features

### 🏆 4 Campaign Acts (20 Handcrafted Levels)
- **Act 1: The Blueprint Wing (General Binary Tree):** Master recursive & iterative traversals, reconstruct tree topologies from Preorder + Inorder blueprints, calculate node heights & diameter, locate Lowest Common Ancestors (LCA), and mirror the vault.
- **Act 2: The Sorted Wing (Binary Search Tree):** Search and insert keys in logarithmic time, execute 3-case node deletions (Leaf, Single-Child, Two-Children with Inorder Successor), perform range queries with branch pruning, and defeat *The Lazy Architect* boss.
- **Act 3: The Architect's Wing (AVL Tree):** Monitor balance factors, execute all 4 single & double rotations (**LL**, **RR**, **LR**, **RL**), predict balancing pivots, and steal the $k$-th smallest treasure in $\mathcal{O}(\log N)$ time using augmented subtree sizes.
- **Act 4: The Heap Fortress (Max Heap & Heap Sort):** Reorganize the vault using bottom-up $\mathcal{O}(N)$ heap construction, repair security locks via sift-down heapify, extract maximums, and execute a full synchronized **Heap Sort**.

### ♾️ Endless Survival Mode & Leaderboard
- Continuous stream of randomized data structure puzzles with ramping speed and tight alarm windows.
- Persistent local high scores stored in `leaderboard.txt`.

### 🧪 Interactive Sandbox Mode
- Free-form testing environment allowing instant switching between **AVL Tree**, **BST**, and **Max Heap**.
- Type custom keys, insert, delete, trigger rebalancing, run full Heap Sort, and serialize the structure to disk.

### ⏱️ The Living Alarm Mechanic
- **Step Cost:** Visiting a node or comparing keys adds 1 alarm step.
- **Capacity Formula:** Bounded by $\lceil 1.5 \times \text{optimalSteps} \rceil + 2$.
- **Star Scoring:**
  - ⭐⭐⭐ : Steps $\le$ Optimal, 0 penalties
  - ⭐⭐ : Steps $\le 1.25 \times$ Optimal, $\le 1$ penalty (or hint used)
  - ⭐: Completed before lockdown
  - ❌ : Alarm triggered (lockdown/retry)

---

## 🏛️ System Architecture

The engine adheres to a **strictly decoupled event-driven architecture** where core data structures remain completely independent of the graphical user interface:

```
[ Model Layer ]           Pure C++ Data Structures (Zero GUI headers)
       │                  BinaryTree, BST, AVL, MaxHeap, Stack, Queue
       ▼
[ Event Log Engine ]      Records mutations & inverses
       │                  VISIT, COMPARE, ROTATE, HEAP_SWAP, EXTRACT_MAX
       ▼
[ Layout Engine ]         Calculates collision-free (x, y) coordinates
       │                  and synchronized array cell slots
       ▼
[ SFML Renderer & HUD ]   Interpolates positions, renders badges,
                          handles audio, and draws UI frames
```

### Scratch-Built Implementations (No STL)
- **Custom `Stack` & `Queue`:** Dynamic array-backed LIFO and circular FIFO structures powering iterative traversals and level-order traversals.
- **`BinaryTree`:** General pointer-based tree supporting reconstruction, diameter, and LCA.
- **`BST`:** Search tree featuring iterative stack-safe routines (`height`, `destroy`) to prevent stack overflow on skewed inputs.
- **`AVL`:** Height-balanced search tree augmented with `subtreeSize`, maintaining balance factor $\in \{-1, 0, +1\}$.
- **`MaxHeap`:** Zero-indexed contiguous array heap featuring bottom-up $\mathcal{O}(N)$ construction and in-place $\mathcal{O}(N \log N)$ Heap Sort.

---

## 📊 Algorithmic Complexity

| Structure / Algorithm | Operation | Average Case | Worst Case | Space Complexity |
| :--- | :--- | :--- | :--- | :--- |
| **General Binary Tree** | Traversal (Recursive/Iterative) | $\mathcal{O}(N)$ | $\mathcal{O}(N)$ | $\mathcal{O}(H)$ |
| **Binary Tree** | Tree Reconstruction | $\mathcal{O}(N)$ | $\mathcal{O}(N^2)$ | $\mathcal{O}(N)$ |
| **Binary Search Tree** | Search / Insert / Delete | $\mathcal{O}(\log N)$ | $\mathcal{O}(N)$ *(Degenerate)* | $\mathcal{O}(1)$ iter / $\mathcal{O}(H)$ rec |
| **AVL Tree** | Search / Insert / Delete | $\mathcal{O}(\log N)$ | $\mathcal{O}(\log N)$ | $\mathcal{O}(\log N)$ |
| **AVL Tree** | $k$-th Smallest (Order Statistics) | $\mathcal{O}(\log N)$ | $\mathcal{O}(\log N)$ | $\mathcal{O}(\log N)$ |
| **Max Heap** | Bottom-Up `buildMaxHeap` | $\mathcal{O}(N)$ | $\mathcal{O}(N)$ | $\mathcal{O}(1)$ |
| **Max Heap** | Insert / Extract-Max | $\mathcal{O}(\log N)$ | $\mathcal{O}(\log N)$ | $\mathcal{O}(1)$ |
| **Heap Sort** | Full In-Place Array Sort | $\mathcal{O}(N \log N)$ | $\mathcal{O}(N \log N)$ | $\mathcal{O}(1)$ |

---

## ⚡ Empirical Benchmarks (Section 10)

Benchmarked on Windows 11 without GUI overhead (`events = nullptr`) across $N = 100, 1{,}000, 10{,}000$:

| Test Variable | $N = 100$ | $N = 1{,}000$ | $N = 10{,}000$ | Theoretical Observation |
| :--- | :--- | :--- | :--- | :--- |
| **BST Height (Sorted Keys)** | 100 | 1,000 | **10,000** | Degenerates to linear chain: $H = N$ |
| **AVL Height (Sorted Keys)** | 7 | 10 | **14** | Logarithmic balance: $H \le 1.44 \log_2(N)$ |
| **AVL Rotations** | 93 | 990 | **9,986** | Self-balances on each sequential insert |
| **BST Search Comparisons** | 100 | 1,000 | **10,000** | Full chain traversal required |
| **AVL Search Comparisons** | 7 | 10 | **14** | Traverses tree height only |
| **BST Search Runtime** | 0.005 s | 0.719 s | **39.192 s** | Quadratic time blowup on repeated searches |
| **AVL Search Runtime** | 0.001 s | 0.013 s | **0.093 s** | **421× faster** than degenerate BST |
| **Heap Build $\mathcal{O}(N)$** | 0.000 s | 0.002 s | **0.018 s** | Confirms linear time bottom-up build |
| **Heap Sort $\mathcal{O}(N \log N)$** | 0.001 s | 0.026 s | **0.227 s** | Predictable scaling with zero memory overhead |

---

## 🗂️ Project Directory Structure

```
VaultHeist/
├── assets/
│   ├── back.jpeg          # Crimson vault / Vecna-inspired background
│   ├── backk.wav          # Atmospheric background audio
│   └── alarm.ogg          # Lockdown alarm SFX
├── AVL.h / .cpp           # Self-balancing AVL tree & rotation engine
├── Benchmark.h            # Section 10 benchmarking suite
├── BinaryTree.h / .cpp    # General tree traversals & reconstruction
├── BST.h / .cpp           # Binary Search Tree & 3-case delete
├── Event.h                # Decoupled GUI animation event logger
├── HeapRenderer.h         # Synchronized Heap Tree + Array visualization
├── Level.json             # Deterministic seed configuration
├── LevelManager.h         # 20 campaign levels, endless logic, win/loss rules
├── main.cpp               # State machine, SFML window, and input router
├── MaxHeap.h / .cpp       # Array-based Max Heap & Heap Sort
├── Queue.h / Stack.h      # Custom primitive linear containers
├── SaveManager.h          # Preorder '#' tree & array heap serialization
├── ScoreSystem.h          # Alarm meter and star calculator
├── tests.cpp              # 29-test headless unit verification suite
├── TreeNode.h             # Core node definition
├── TreeRenderer.h         # Hierarchical tree layout and SFML drawing
└── UIManager.h            # Main menu, HUD, and dialog rendering
```

---

## 🛠️ Getting Started

### Prerequisites
- **OS:** Windows 10/11 (Uses standard system font `C:\Windows\Fonts\arial.ttf`)
- **Compiler:** Visual Studio 2022 (MSVC) or MinGW-w64 with C++17 support
- **Graphics Library:** [SFML 2.6.x (64-bit)](https://www.sfml-dev.org/download/sfml/2.6.2/)

### Setup in Visual Studio
1. Clone the repository:
   ```bash
   git clone https://github.com/Zubair-Ashraf-i253158/Vault-Heist.git
   ```
2. Open the solution in **Visual Studio 2022** and set configuration to **x64**.
3. Configure Project Properties:
   - **C/C++ $\rightarrow$ General $\rightarrow$ Additional Include Directories:** Add path to `SFML-2.6.x/include`
   - **Linker $\rightarrow$ General $\rightarrow$ Additional Library Directories:** Add path to `SFML-2.6.x/lib`
   - **Linker $\rightarrow$ Input $\rightarrow$ Additional Dependencies:**
     ```
     sfml-graphics-d.lib
     sfml-window-d.lib
     sfml-system-d.lib
     sfml-audio-d.lib
     ```
   - **Linker $\rightarrow$ System $\rightarrow$ Stack Reserve Size:** Set to `67108864` (64 MB) for large-scale benchmarks.
4. Copy SFML DLLs (`sfml-graphics-2.dll`, `sfml-window-2.dll`, `sfml-system-2.dll`, `sfml-audio-2.dll`, `openal32.dll`) into the executable folder.
5. Ensure `Level.json` and the `assets/` directory are in the working directory.
6. Build and press **F5** to run.

---

## 🎮 Game Controls

### Main Menu & Navigation
| Input | Action |
| :--- | :--- |
| `1` / Click | Launch Campaign (Level 1.1) |
| `2` / Click | Open Campaign Map (Level Select & Stars) |
| `3` / Click | Start Endless Survival Mode |
| `4` / Click | Open Sandbox Mode |
| `5` / Click | View Leaderboard |
| `6` / Click | Save & Exit |
| `U` | Dev Command: Unlock all 20 levels (on Campaign Map) |
| `ESC` | Return to previous menu / Exit to map |

### Campaign & Puzzle Controls
| Key / Input | Context | Action |
| :--- | :--- | :--- |
| `Left Click` | General | Select/visit room, place token, or pick heap swap nodes |
| `0`–`9` / Numpad | Act 1.3 / 1.4 | Input node heights, diameter, or LCA path length |
| `A` | Act 1.5 | Auto-mirror tree |
| `1`, `2`, `3` | Act 2.3 | Select BST Delete case (Leaf / One-Child / Two-Children) |
| `L` / `R` | Act 3.2 & 3.3 | Execute Left or Right AVL rotation |
| `1`–`4` | Act 3.4 | Predict rotation type (`1: LL`, `2: RR`, `3: LR`, `4: RL`) |
| `E` | Act 4 | Extract root maximum from Max Heap |
| `H` | Gameplay | Show concept hint (caps score at 2 stars) |
| `BACKSPACE` | Gameplay | Retry level with identical seed |
| `ENTER` | Victory | Advance to next level |

### Sandbox Mode Controls
| Key | Action |
| :--- | :--- |
| `T` | Switch active structure (`AVL Tree` $\rightarrow$ `BST` $\rightarrow$ `Max Heap`) |
| `0`–`9` / `BACKSPACE` | Type or edit integer key value (up to 4 digits) |
| `I` | Insert typed key (or random `10–99` if empty) |
| `D` | Delete typed key / Extract Max |
| `H` | Run full Heap Sort (Max Heap mode only) |
| `C` | Clear data structure |
| `S` / `L` | Save to or Load from `sandbox_tree.txt` or `sandbox_heap.txt` |

---

## 🧪 Unit Testing Suite

The repository includes a headless automated test runner (`tests.cpp`) covering 29 distinct test scenarios across all data structures:

```bash
g++ -std=c++17 tests.cpp BinaryTree.cpp BST.cpp AVL.cpp MaxHeap.cpp -o tests.exe
./tests.exe
```

```
========================================
  VAULT HEIST - UNIT TEST SUITE
========================================
--- BinaryTree Tests ---
[PASS] Empty tree traversals
[PASS] Single node traversal
[PASS] Preorder recursive == iterative
[PASS] Inorder recursive == iterative
[PASS] Postorder recursive == iterative
[PASS] Reconstruction round-trip matches original
[PASS] BinaryTree LCA(20, 40) == 30
[PASS] BinaryTree Diameter == 4
[PASS] Double mirror restores original tree

--- BST Tests ---
[PASS] BST valid after inserts
[PASS] BST rejects duplicate insert
[PASS] BST delete missing key returns false
[PASS] BST delete leaf
[PASS] BST delete one-child node
[PASS] BST delete two-children node
[PASS] BST delete root

--- AVL Tests ---
[PASS] AVL LL rotation
[PASS] AVL RR rotation
[PASS] AVL LR rotation
[PASS] AVL RL rotation
[PASS] AVL rejects duplicate insert
[PASS] AVL delete with BF 0 on heavy child
[PASS] AVL kth() and rank() match sorted array

--- MaxHeap & Heap Sort Tests ---
[PASS] Heap empty array
[PASS] Heap single element
[PASS] Heap property holds after inserts with duplicates
[PASS] ExtractMax returns max and keeps heap valid
[PASS] Heap Sort matches reference on mixed/negative/duplicate array
[PASS] Heap Sort on reverse-sorted array

========================================
  SUMMARY: 29 PASSED, 0 FAILED
========================================
```

---

## 📜 Academic Integrity & License

This project was developed strictly for academic purposes as part of the CS2001 Data Structures & Algorithms coursework at FAST-NU Islamabad. All core data structures and gameplay routines were developed from scratch by the authors and images by AI.

---

<div align="center">

**Made with 🗝️, C++, and SFML by Zubair Ashraf & Abdullah Saqib**  
*FAST-NU Islamabad — CS2001 DSA Semester Project*

⭐ If you found this implementation helpful, please consider starring the repository!

</div>
