================================================================================
                    VAULT HEIST: ECHOES OF THE LIVING VAULT
                     Data Structures & Algorithms Project
================================================================================

Group Members:
- Zubair Ashraf  (Roll No: 25I-3158)
- Abdullah Saqib (Roll No: 25I-2020)
Institution: FAST National University of Computer & Emerging Sciences, Islamabad
Course: CS2001 - Data Structures & Algorithms

================================================================================
1. PROJECT OVERVIEW
================================================================================
Vault Heist: Echoes of the Living Vault is a desktop puzzle-strategy game built
in C++ using SFML 2.6. The game visualizes fundamental data structures through
interactive gameplay. Every chamber inside the vault corresponds to a node in a
tree or an element in a heap.

CRITICAL IMPLEMENTATION REQUIREMENT:
All core data structures and supporting containers have been built completely
FROM SCRATCH using raw pointers and arrays. Absolutely no built-in STL containers
(such as std::vector, std::queue, std::stack, std::priority_queue, or std::map)
are used in the model implementation.

Included Scratch Implementations:
- Stack.h       : Custom LIFO array-based stack (dynamic resizing)
- Queue.h       : Custom circular FIFO queue
- BinaryTree    : General binary tree (recursive & iterative traversals, 
                  reconstruction from traversals, LCA, diameter, mirror)
- BST           : Binary Search Tree (ordered insertion, 3-case deletion, 
                  inorder successor, range search, min/max, iterative cleanup)
- AVL           : Self-balancing AVL tree extending BST (LL, RR, LR, RL 
                  rotations, balance factor updates, O(log N) kth element 
                  and rank tracking via augmented subtreeSize)
- MaxHeap       : Zero-indexed array-based heap (O(N) bottom-up construction, 
                  sift-up, sift-down, extract-max, in-place Heap Sort)
- Event.h       : Decoupled event logging system connecting model to SFML GUI

================================================================================
2. DIRECTORY & FILE CONTENTS
================================================================================
- main.cpp              : Application entry point, state loop, SFML window
- BinaryTree.h / .cpp   : General binary tree algorithms
- BST.h / .cpp          : Binary search tree algorithms
- AVL.h / .cpp          : AVL self-balancing algorithms and order statistics
- MaxHeap.h / .cpp      : Max heap and Heap Sort algorithms
- TreeNode.h            : Node definition (id, key, left, right, parent, size, height)
- Stack.h / Queue.h     : Custom primitive linear containers
- Event.h               : Decoupled visual event logger
- TreeRenderer.h        : Layout engine and node/edge rendering for trees
- HeapRenderer.h        : Synchronized tree and array visualization for heaps
- LevelManager.h        : 20 campaign levels, endless puzzle generator, game rules
- ScoreSystem.h         : Alarm meter calculation, penalty tracking, stars
- SaveManager.h         : Preorder '#' tree and array heap serialization
- UIManager.h           : Menus, HUD, font rendering, and dialog boxes
- Benchmark.h           : Section 10 performance and height analysis suite
- tests.cpp             : Headless 29-test unit verification runner
- Level.json            : Seed configuration for 20 campaign levels
- sandbox_tree.txt      : Save file for Sandbox AVL/BST trees
- sandbox_heap.txt      : Save file for Sandbox heaps
- savegame.txt          : Campaign progress and star records
- leaderboard.txt       : Endless mode high scores
- assets/               : Required runtime assets (alarm.ogg, backk.wav, back.jpeg)

================================================================================
3. COMPILATION & EXECUTION INSTRUCTIONS
================================================================================
Requirements:
- Windows 10 or 11 (64-bit)
- Visual Studio 2022 (MSVC) or MinGW-w64 with C++17 support
- SFML 2.6.x (64-bit)
- Font: The game uses the standard Windows font C:\Windows\Fonts\arial.ttf

Option A: Visual Studio 2022 (Recommended)
1. Extract the submission ZIP folder.
2. Open the solution / project in Visual Studio 2022 and select 'x64' configuration.
3. In Project Properties -> C/C++ -> General -> Additional Include Directories:
   Add the path to your SFML 'include' folder.
4. In Project Properties -> Linker -> General -> Additional Library Directories:
   Add the path to your SFML 'lib' folder.
5. In Project Properties -> Linker -> Input -> Additional Dependencies:
   - For Debug:
     sfml-graphics-d.lib;sfml-window-d.lib;sfml-system-d.lib;sfml-audio-d.lib;
   - For Release:
     sfml-graphics.lib;sfml-window.lib;sfml-system.lib;sfml-audio.lib;
6. In Project Properties -> Linker -> System -> Stack Reserve Size:
   Set to 67108864 (64 MB) to prevent recursion exhaustion on 10,000-node benchmarks.
7. Ensure all SFML DLLs (sfml-graphics-2.dll, sfml-window-2.dll, sfml-system-2.dll,
   sfml-audio-2.dll, openal32.dll) and the 'assets' folder are placed in the same 
   directory as the generated .exe file.
8. Press F5 to build and launch the game.

Option B: MinGW Command Line (g++)
Compile the main executable with:
g++ -std=c++17 main.cpp BinaryTree.cpp BST.cpp AVL.cpp MaxHeap.cpp -o VaultHeist.exe -I"<SFML_PATH>/include" -L"<SFML_PATH>/lib" -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

Compile and run the headless unit test suite with:
g++ -std=c++17 tests.cpp BinaryTree.cpp BST.cpp AVL.cpp MaxHeap.cpp -o tests.exe
./tests.exe

================================================================================
4. COMPLETE GAME CONTROLS
================================================================================

--- MAIN MENU ---
- '1' or Left Click   : Launch Campaign from Level 1.1
- '2' or Left Click   : Open Campaign Map (Level Selection & Star Ratings)
- '3' or Left Click   : Launch Endless Survival Mode
- '4' or Left Click   : Open Sandbox Mode
- '5' or Left Click   : View Local Leaderboard
- '6' or Left Click   : Save Progress and Exit Game

--- CAMPAIGN MAP (LEVEL SELECT) ---
- Left Click          : Select and start any unlocked level
- 'U'                 : Unlock all 20 levels (Evaluation / Viva shortcut)
- ESC                 : Return to Main Menu

--- GENERAL IN-GAME CONTROLS ---
- 'H'                 : Display Concept Hint (caps star rating to 2 stars)
- ENTER               : Advance to next level after victory
- BACKSPACE           : Restart current level (re-uses seed in Campaign)
- ESC                 : Return to Campaign Map (or save & exit Endless mode)

--- ACT 1 CONTROLS: THE BLUEPRINT WING (BINARY TREE) ---
- Level 1.1 (Traversals)      : Left Click rooms in the requested sequence 
                                (Preorder, Inorder, Postorder, or Level-Order)
- Level 1.2 (Reconstruction)  : Left Click bottom number token, then Left Click 
                                its matching '?' slot in the tree
- Level 1.3 (Heights/Diameter): Left Click a room, press '0'-'9' for height. 
                                Once all heights are set, press '0'-'9' for Diameter
- Level 1.4 (LCA & Path)      : Left Click the LCA room, press '0'-'9' for path length
- Level 1.5 (Tree Mirror)     : Left Click parent rooms to swap children, or 
                                press 'A' for Auto-Mirror

--- ACT 2 CONTROLS: THE SORTED WING (BST) ---
- Levels 2.1, 2.2, 2.5        : Left Click rooms sequentially along search path from root
- Level 2.3 (BST Deletion)    : Press '1' (Leaf), '2' (One Child), or '3' (Two Children).
                                For cases 2 and 3, Left Click the replacement node 
                                (child or Inorder Successor)
- Level 2.4 (Range Raid)      : Left Click every room whose value falls inside [Low, High]

--- ACT 3 CONTROLS: THE ARCHITECT'S WING (AVL TREE) ---
- Level 3.1 (Balance Badges)  : Left Click the room showing an invalid balance factor (|BF| > 1)
- Level 3.2 (Single Rotation) : Left Click the unbalanced pivot, then press 'L' (Left) 
                                or 'R' (Right) to rebalance
- Level 3.3 (Double Rotation) : Press 'L' or 'R' for first rotation, then 'L' or 'R' 
                                for second rotation
- Level 3.4 (Predict Rotation): Press '1' (LL), '2' (RR), '3' (LR), or '4' (RL) 
                                to predict the rebalancing maneuver
- Level 3.5 (Order Statistics): Left Click nodes using subtree size badges to reach 
                                the k-th smallest element in O(log N) steps

--- ACT 4 CONTROLS: THE HEAP FORTRESS (MAX HEAP & HEAP SORT) ---
- Levels 4.1 & 4.2 (Heapify)  : Left Click an active node, then Left Click its parent 
                                or child to execute a manual swap
- Levels 4.3, 4.4, 4.5        : Press 'E' to extract root maximum into sorted zone, 
                                then swap parent-child nodes to restore Max Heap property

--- SANDBOX MODE CONTROLS ---
- 'T'                         : Switch active structure (AVL Tree -> BST -> Max Heap)
- '0'-'9' / BACKSPACE         : Type or edit custom integer key (up to 4 digits)
- 'I'                         : Insert typed value (or random 10-99 if empty)
- 'D'                         : Delete typed value in AVL/BST; Extract Max in Max Heap
- 'H'                         : Run complete Heap Sort (Max Heap mode only)
- 'C'                         : Clear current data structure
- 'S'                         : Save to sandbox_tree.txt or sandbox_heap.txt
- 'L'                         : Load from file
- ESC                         : Return to Main Menu

================================================================================
5. RUNNING THE BENCHMARKS (SECTION 10)
================================================================================
The project includes a dedicated benchmarking class (Benchmark.h). Benchmarks
are run with GUI and event logging disabled to capture raw algorithmic performance.

To run benchmarks directly inside your code, call:
  Benchmark::runTreeComparison(N);
  Benchmark::runHeapBenchmark(N);
for N = 100, 1000, and 10000.
================================================================================