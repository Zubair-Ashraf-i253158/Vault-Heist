# Level Data & Guide to Adding New Levels

## Vault Heist: The Living Vault
**Developers:** Zubair Ashraf & Abdullah Saqib

---

## 1. How `Level.json` Works

All 20 campaign levels are stored in the `Level.json` file located in the main project folder.

Each JSON entry represents one level and contains three fields:

```json
{ "act": 1, "level": 1, "seed": 12345 }
```

### Field Descriptions

| Field | Description |
|---|---|
| `act` | The Act number (1 = Binary Tree, 2 = BST, 3 = AVL Tree, 4 = Max Heap). |
| `level` | The sub-level number within that Act (1 to 5). |
| `seed` | A fixed integer passed to `srand(seed)` inside `LevelManager::buildLevel()`. |


## 1. How to Change an Existing Level (No Code Changes)

You can generate a brand-new tree, target, or heap array for any existing level without recompiling the C++ code.

### Steps

1. Open `Level.json` in Notepad or Visual Studio.
2. Find the entry for the level you want to modify (for example, Act 2, Level 3).
3. Change its seed value to any other integer.

**Example:**

Original entry:

```json
{ "act": 2, "level": 3, "seed": 12345 }
```

Updated entry:

```json
{ "act": 2, "level": 3, "seed": 99999 }
```

4. Save `Level.json`.
5. Run the game.

The level will now generate a different puzzle using the new seed. Its optimal steps will automatically be recalculated by the model.

**Note:** No C++ code changes or recompilation are required when modifying the seed of an existing level.

---

## 2. How to Add a Brand-New Level

To add a new sub-level (for example, Level 1.6), you need to update both the level data and the relevant C++ code.

### Step 1: Add the Level to `Level.json`

Open `Level.json` and add a new entry for your level.

For example, to add Act 1, Level 6:

```json
{ "act": 1, "level": 6, "seed": 67890 }
```

Make sure to add a comma after the previous entry when inserting the new entry into the JSON array.

### Step 2: Define the Mission in `LevelManager.h`

Open `LevelManager.h` and locate the following function:

```cpp
buildLevel(int a, int sl)
```

Inside this function, find the matching Act and add a new `else if` block for the new sub-level.

For example:

```cpp
else if (subLevel == 6)
{
    // Configure the new level
}
```

Configure the following properties for the new mission:

- **title:** The name of the level.
- **objective:** The goal the player must achieve.
- **instruction:** Instructions explaining how to complete the mission.
- **hintText:** A helpful hint for the player.
- **scoreSystem.reset(...):** Initialize the scoring system for the new level.

Make sure the new level is added under the correct Act.

### Step 3: Update the Sub-Level Limit in `LevelManager::nextLevel()`

Locate `LevelManager::nextLevel()` and update the sub-level limit from 5 to 6.

This ensures that the campaign progression system recognizes the newly added level.

### Step 4: Update the Campaign Map

Update the Campaign Map grid in the relevant UI files:

- `main.cpp`
- `UIManager.h`

Find the existing sub-level limit of 5 and change it to 6 wherever it controls the campaign map or level unlocking.

This ensures that:

- Level 1.6 appears on the Campaign Map.
- The new level can be selected when unlocked.
- Level 1.6 unlocks after successfully completing Level 1.5.
- Campaign progression supports the additional level.

---

## 3. Important Reference: Act Numbers

The game contains four Acts, each representing a different data structure.

| Act | Data Structure |
|---|---|
| Act 1 | Binary Tree |
| Act 2 | Binary Search Tree (BST) |
| Act 3 | AVL Tree |
| Act 4 | Max Heap |

---


**Project:** Vault Heist: The Living Vault  
**Documentation:** Level Data & Guide to Adding New Levels  
**Developers:** Zubair Ashraf & Abdullah Saqib
