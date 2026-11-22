# Vault Heist: The Living Vault
### By Zubair Ashraf & Abdullah Saqib

**Vault Heist** is a C++ and SFML puzzle game made for our Data Structures & Algorithms project. You play as a thief breaking into a vault where every room is a node in a tree or heap. Every step raises the alarm meter, so you have to use the right data structure rules to finish each level before the alarm gets full.

All data structures (**Binary Tree**, **BST**, **AVL Tree**, **Max Heap**, **Stack**, and **Queue**) are written from scratch without using built-in STL tree or heap libraries.

---

## 1. What is Included

- **4 Campaign Acts (20 Levels):**
  - **Act 1 (Binary Tree):** Traversals, Tree Reconstruction, Heights & Diameter, LCA, and Mirroring
  - **Act 2 (BST):** Search, Insert, 3-Case Delete, Range Query, and Skewed BST Boss
  - **Act 3 (AVL Tree):** Balance Factors, Single & Double Rotations (LL, RR, LR, RL), Prediction, and Order Statistics (k-th smallest)
  - **Act 4 (Max Heap & Heap Sort):** Build Max Heap, Sift-Down, Extract Max, and full Heap Sort with synced Tree + Array view
- **Endless Survival Mode:** Random puzzles from all acts that save your total score to the local leaderboard
- **Sandbox Mode:** Free mode where you can insert, delete, sort, save, and load your own AVL, BST, or Max Heap
- **Seeded Levels:** All 20 campaign levels read their fixed seed from `Level.json` so every run is fair and repeatable

---

## 2. Project Files

- `main.cpp` — Main game loop, menus, audio, and screen switching
- `BinaryTree.h` / `BinaryTree.cpp` — General binary tree logic
- `BST.h` / `BST.cpp` — Binary Search Tree logic
- `AVL.h` / `AVL.cpp` — Self-balancing AVL tree logic
- `MaxHeap.h` / `MaxHeap.cpp` — Array-based Max Heap and Heap Sort logic
- `TreeNode.h`, `Stack.h`, `Queue.h`, `Event.h` — Node structure, custom stack/queue, and event log
- `LevelManager.h` — All 20 levels, Endless mode puzzles, and win/lose checks
- `ScoreSystem.h` — Alarm meter, penalties, score, and 3-star rating
- `SaveManager.h` — Saves and loads progress (`savegame.txt`), leaderboard (`leaderboard.txt`), and sandbox files
- `TreeRenderer.h`, `HeapRenderer.h`, `UIManager.h` — Draws the trees, heaps, menus, and HUD
- `Benchmark.h` — Tests and compares BST vs. AVL and Heap Sort speed
- `Level.json` — Stores the seed for each campaign level
- `assets/` — Background image (`back.jpeg`) and sound files (`backk.wav`, `alarm.ogg`)

---

## 3. How to Run the Game

### What You Need
- **Windows PC** (uses `C:\Windows\Fonts\arial.ttf` for text)
- **Visual Studio 2022** (or MinGW g++ with C++17)
- **SFML 2.6.x** installed on your computer

### Option A: Running in Visual Studio (Recommended)
1. Open the project in Visual Studio and select **x64** (or the platform matching your SFML).
2. Go to **Project Properties -> C/C++ -> General -> Additional Include Directories** and add your SFML `include` folder.
3. Go to **Linker -> General -> Additional Library Directories** and add your SFML `lib` folder.
4. Go to **Linker -> Input -> Additional Dependencies** and add:
   - **Debug:** `sfml-graphics-d.lib; sfml-window-d.lib; sfml-system-d.lib; sfml-audio-d.lib;`
   - **Release:** `sfml-graphics.lib; sfml-window.lib; sfml-system.lib; sfml-audio.lib;`
5. Put the SFML `.dll` files (`sfml-graphics-2.dll`, `sfml-window-2.dll`, `sfml-system-2.dll`, `sfml-audio-2.dll`, `openal32.dll`) in the same folder as the `.exe`.
6. Make sure `Level.json` and the `assets/` folder are in the working folder.
7. Press **F5** to build and run.

---

## 4. Complete Game Controls

### Main Menu
- **1 (or Left Click):** Play Campaign from Level 1.1
- **2 (or Left Click):** Open Campaign Map (Level Select & Stars)
- **3 (or Left Click):** Start Endless Survival Mode
- **4 (or Left Click):** Open Sandbox Mode (AVL / BST / Max Heap)
- **5 (or Left Click):** Open Leaderboard
- **6 (or Left Click):** Save Progress and Exit

### Campaign Map (Level Select)
- **Left Click:** Start any unlocked level
- **U:** Unlock all 20 levels (for testing or viva demo)
- **ESC:** Go back to Main Menu

### Leaderboard
- **ESC:** Go back to Main Menu

### General Gameplay Controls (Campaign & Endless)
- **H:** Show concept hint (caps level rating at 2 stars)
- **ENTER:** Move to the next level (or next Endless puzzle) after winning
- **BACKSPACE:** Restart current level (uses the same seed in Campaign; loads a new puzzle in Endless)
- **ESC:** Go back to Level Select (in Endless Mode, saves your total score to Leaderboard and goes to Main Menu)

### Act 1 Controls (Binary Tree)
- **Level 1.1 (Traversal):** Left Click rooms in the asked order (Preorder, Inorder, Postorder, or Level Order)
- **Level 1.2 (Reconstruction):** Left Click a number token at the bottom, then Left Click its `?` slot in the tree
- **Level 1.3 (Heights & Diameter):** Left Click a room and press **0–9** (or Numpad **0–9**) for its height (`leaf = 1`). Once all heights are entered, press **0–9** for the Diameter
- **Level 1.4 (LCA & Path):** Left Click the LCA room, then press **0–9** (or Numpad) for the path length in edges
- **Level 1.5 (Mirror):** Left Click parent rooms to swap their left and right children, or press **A** for Auto-Mirror

### Act 2 Controls (BST)
- **Levels 2.1, 2.2, 2.5 (Search / Insert / Boss):** Left Click each room along the BST path from Root
- **Level 2.3 (Delete):** Press **1** (Leaf), **2** (One-Child), or **3** (Two-Children). For One-Child or Two-Children, then Left Click the replacement room (child or Inorder Successor)
- **Level 2.4 (Range Raid):** Left Click every room whose number is inside the given range

### Act 3 Controls (AVL Tree)
- **Level 3.1 (Badges):** Left Click the unbalanced (red-badged) room
- **Level 3.2 (Single Rotation):** Left Click the unbalanced (red-badged) pivot, then press **L** (Left rotation) or **R** (Right rotation)
- **Level 3.3 (Double Rotation):** Press **L** or **R** for the 1st rotation, then **L** or **R** for the 2nd rotation
- **Level 3.4 (Prediction):** Press **1** (LL), **2** (RR), **3** (LR), or **4** (RL) to predict the rotation
- **Level 3.5 (Order Statistics):** Left Click rooms from Root using subtree size badges to find the k-th smallest room

### Act 4 Controls (Max Heap & Heap Sort)
- **Levels 4.1 & 4.2 (Build Heap / Sift-Down):** Left Click one active node, then Left Click its parent or child to swap them (Left Click the same node again to cancel selection)
- **Levels 4.3, 4.4, 4.5 (Extract Max & Heap Sort):** Press **E** to extract the Max root (when the Max Heap is valid), then Left Click parent-child nodes to swap and restore the Max Heap

### Sandbox Mode Controls
- **T:** Switch structure (`AVL Tree` -> `BST` -> `Max Heap`)
- **0–9:** Type a custom number (up to 4 digits)
- **BACKSPACE:** Delete the last typed digit
- **I:** Insert your typed number (or a random number `10–99` if nothing is typed)
- **D:** Delete your typed number (or delete Root if nothing is typed) in AVL/BST; Extract Max in Max Heap
- **H:** Run Heap Sort (Max Heap mode only)
- **C:** Clear the whole tree or heap
- **S:** Save to file (`sandbox_tree.txt` or `sandbox_heap.txt`)
- **L:** Load from file
- **ESC:** Go back to Main Menu
