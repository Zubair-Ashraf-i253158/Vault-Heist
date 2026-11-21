#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include "LevelManager.h"
#include "UIManager.h"
#include "TreeRenderer.h"
#include "HeapRenderer.h"
#include "SaveManager.h"

using namespace std;

const int STATE_MENU = 0;
const int STATE_SELECT = 1;
const int STATE_PLAY = 2;
const int STATE_SANDBOX = 3;
const int STATE_ENDLESS = 4;
const int STATE_LEADERBOARD = 5;

int main() {
    int gameState = STATE_MENU;

    LevelManager levels;
    UIManager ui;
    TreeRenderer treeView;
    HeapRenderer heapView;

    // Campaign Save Data
    int unlockedCount = 1; // Level 1.1 unlocked by default
    int levelStars[4][5] = { 0 };
    int gameSeed = 42;
    SaveManager::loadProgress(unlockedCount, levelStars, gameSeed);
    srand(gameSeed);

    // Leaderboard Data
    int topScores[5] = { 0 };
    int scoreCount = SaveManager::loadLeaderboard(topScores);

    // Sandbox Data (0 = AVL, 1 = BST, 2 = MaxHeap)
    AVL sandboxAVL;
    BST sandboxBST;
    MaxHeap sandboxHeap;
    int sandboxMode = 0;
    string sandboxStatus = "Ready";

    // Endless Mode Data
    AVL endlessTree;
    sf::Clock endlessClock;
    sf::Clock opClock;
    int endlessOps = 0;
    string endlessStatus = "Surviving...";

    sf::RenderWindow window(sf::VideoMode(1000, 680), "Vault Heist: The Living Vault");
    window.setFramerateLimit(60);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                SaveManager::saveProgress(unlockedCount, levelStars, gameSeed);
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                if (gameState == STATE_MENU) {
                    if (event.key.code == sf::Keyboard::Num1) {
                        levels.loadLevel(1, 1);
                        gameState = STATE_PLAY;
                    }
                    else if (event.key.code == sf::Keyboard::Num2) {
                        gameState = STATE_SELECT;
                    }
                    else if (event.key.code == sf::Keyboard::Num3) {
                        endlessTree.clear();
                        endlessTree.insert(50);
                        endlessOps = 0;
                        endlessClock.restart();
                        opClock.restart();
                        endlessStatus = "Stream started!";
                        gameState = STATE_ENDLESS;
                    }
                    else if (event.key.code == sf::Keyboard::Num4) {
                        sandboxAVL.clear(); sandboxAVL.insert(50); sandboxAVL.insert(30); sandboxAVL.insert(70);
                        sandboxBST.clear(); sandboxBST.insert(50); sandboxBST.insert(30); sandboxBST.insert(70);
                        sandboxHeap.total = 0; sandboxHeap.heapSize = 0; sandboxHeap.insert(50); sandboxHeap.insert(30); sandboxHeap.insert(70);
                        gameState = STATE_SANDBOX;
                    }
                    else if (event.key.code == sf::Keyboard::Num5) {
                        scoreCount = SaveManager::loadLeaderboard(topScores);
                        gameState = STATE_LEADERBOARD;
                    }
                    else if (event.key.code == sf::Keyboard::Num6) {
                        SaveManager::saveProgress(unlockedCount, levelStars, gameSeed);
                        window.close();
                    }
                }
                else if (gameState == STATE_SELECT) {
                    if (event.key.code == sf::Keyboard::Escape) gameState = STATE_MENU;
                    else if (event.key.code == sf::Keyboard::U) {
                        unlockedCount = 20; // Helpful for viva demo!
                        SaveManager::saveProgress(unlockedCount, levelStars, gameSeed);
                    }
                }
                else if (gameState == STATE_LEADERBOARD) {
                    if (event.key.code == sf::Keyboard::Escape) gameState = STATE_MENU;
                }
                else if (gameState == STATE_ENDLESS) {
                    if (event.key.code == sf::Keyboard::Escape) {
                        int finalPts = (int)endlessClock.getElapsedTime().asSeconds() * 10 + endlessOps * 5;
                        SaveManager::saveLeaderboardScore(finalPts);
                        scoreCount = SaveManager::loadLeaderboard(topScores);
                        gameState = STATE_LEADERBOARD;
                    }
                }
                else if (gameState == STATE_PLAY) {
                    if (event.key.code == sf::Keyboard::Escape) {
                        if (levels.campaignComplete) {
                            levels.campaignComplete = false;
                            gameState = STATE_MENU;
                        }
                        else {
                            gameState = STATE_SELECT;
                        }
                    }
                    else if (event.key.code == sf::Keyboard::Enter) {
                        if (levels.levelWon) {
                            // Save stars & unlock next level
                            int aIdx = levels.act - 1;
                            int lIdx = levels.subLevel - 1;
                            if (levels.scoreSystem.stars > levelStars[aIdx][lIdx]) {
                                levelStars[aIdx][lIdx] = levels.scoreSystem.stars;
                            }
                            int nextIdx = aIdx * 5 + levels.subLevel + 1;
                            if (nextIdx > unlockedCount && nextIdx <= 20) unlockedCount = nextIdx;
                            SaveManager::saveProgress(unlockedCount, levelStars, gameSeed);
                        }
                        levels.nextLevel();
                    }
                    else if (event.key.code == sf::Keyboard::Backspace) {
                        levels.retry();
                    }
                    else {
                        levels.handleKeyPress(event.key.code);
                    }
                }
                else if (gameState == STATE_SANDBOX) {
                    if (event.key.code == sf::Keyboard::Escape) gameState = STATE_MENU;
                    else if (event.key.code == sf::Keyboard::T) {
                        sandboxMode = (sandboxMode + 1) % 3;
                        sandboxStatus = "Switched structure";
                    }
                    else if (event.key.code == sf::Keyboard::I) {
                        int val = (rand() % 90) + 10;
                        if (sandboxMode == 0) sandboxAVL.insert(val);
                        else if (sandboxMode == 1) sandboxBST.insert(val);
                        else sandboxHeap.insert(val);
                        sandboxStatus = "Inserted " + to_string(val);
                    }
                    else if (event.key.code == sf::Keyboard::D) {
                        if (sandboxMode == 0 && sandboxAVL.root) {
                            int r = sandboxAVL.root->value; sandboxAVL.deleteKey(r);
                            sandboxStatus = "Deleted AVL Root " + to_string(r);
                        }
                        else if (sandboxMode == 1 && sandboxBST.root) {
                            int r = sandboxBST.root->value; sandboxBST.deleteKey(r);
                            sandboxStatus = "Deleted BST Root " + to_string(r);
                        }
                        else if (sandboxMode == 2 && sandboxHeap.heapSize > 0) {
                            int m = sandboxHeap.extractMax();
                            sandboxStatus = "Extracted Max " + to_string(m);
                        }
                    }
                    else if (event.key.code == sf::Keyboard::C) {
                        if (sandboxMode == 0) sandboxAVL.clear();
                        else if (sandboxMode == 1) sandboxBST.clear();
                        else { sandboxHeap.total = 0; sandboxHeap.heapSize = 0; }
                        sandboxStatus = "Cleared";
                    }
                    else if (event.key.code == sf::Keyboard::S) {
                        if (sandboxMode == 0) SaveManager::saveTreeToFile(sandboxAVL.root, "sandbox_tree.txt");
                        else if (sandboxMode == 1) SaveManager::saveTreeToFile(sandboxBST.root, "sandbox_tree.txt");
                        else SaveManager::saveHeapToFile(sandboxHeap, "sandbox_heap.txt");
                        sandboxStatus = "Saved to file!";
                    }
                    else if (event.key.code == sf::Keyboard::L) {
                        int nextId = 1;
                        if (sandboxMode == 0) {
                            sandboxAVL.clear();
                            sandboxAVL.root = SaveManager::loadTreeFromFile("sandbox_tree.txt", nextId);
                        }
                        else if (sandboxMode == 1) {
                            sandboxBST.clear();
                            sandboxBST.root = SaveManager::loadTreeFromFile("sandbox_tree.txt", nextId);
                        }
                        else {
                            SaveManager::loadHeapFromFile(sandboxHeap, "sandbox_heap.txt");
                        }
                        sandboxStatus = "Loaded from file!";
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                float mx = (float)event.mouseButton.x;
                float my = (float)event.mouseButton.y;

                if (gameState == STATE_MENU) {
                    if (mx >= 280.0f && mx <= 720.0f) {
                        for (int i = 0; i < 6; i++) {
                            float cy = 165.0f + i * 62.0f;
                            if (my >= cy - 22.0f && my <= cy + 22.0f) {
                                if (i == 0) { levels.loadLevel(1, 1); gameState = STATE_PLAY; }
                                else if (i == 1) gameState = STATE_SELECT;
                                else if (i == 2) {
                                    endlessTree.clear(); endlessTree.insert(50); endlessOps = 0;
                                    endlessClock.restart(); opClock.restart(); gameState = STATE_ENDLESS;
                                }
                                else if (i == 3) {
                                    sandboxAVL.clear(); sandboxAVL.insert(50); sandboxAVL.insert(30); sandboxAVL.insert(70);
                                    gameState = STATE_SANDBOX;
                                }
                                else if (i == 4) {
                                    scoreCount = SaveManager::loadLeaderboard(topScores);
                                    gameState = STATE_LEADERBOARD;
                                }
                                else if (i == 5) {
                                    SaveManager::saveProgress(unlockedCount, levelStars, gameSeed);
                                    window.close();
                                }
                            }
                        }
                    }
                }
                else if (gameState == STATE_SELECT) {
                    for (int a = 0; a < 4; a++) {
                        float y = 85.0f + (a * 135.0f) + 25.0f;
                        for (int l = 1; l <= 5; l++) {
                            int levelIndex = a * 5 + l;
                            if (levelIndex > unlockedCount) continue;
                            float x = 150.0f + ((l - 1) * 145.0f);
                            if (mx >= x && mx <= x + 115.0f && my >= y && my <= y + 52.0f) {
                                levels.loadLevel(a + 1, l);
                                gameState = STATE_PLAY;
                            }
                        }
                    }
                }
                else if (gameState == STATE_PLAY) {
                    levels.handleNodeClick(mx, my);
                }
            }
        }

        // Endless Mode live operation stream (ramps up speed)
        if (gameState == STATE_ENDLESS) {
            float interval = 1.8f - (endlessOps * 0.03f);
            if (interval < 0.45f) interval = 0.45f;
            if (opClock.getElapsedTime().asSeconds() >= interval) {
                opClock.restart();
                endlessOps++;
                if (rand() % 3 == 0 && endlessTree.root != nullptr) {
                    int r = endlessTree.root->value;
                    endlessTree.deleteKey(r);
                    endlessStatus = "Auto-Deleted " + to_string(r);
                }
                else {
                    int v = (rand() % 90) + 10;
                    endlessTree.insert(v);
                    endlessStatus = "Auto-Inserted " + to_string(v);
                }
            }
        }

        window.clear(sf::Color(16, 18, 24));

        if (gameState == STATE_MENU) {
            ui.drawMainMenu(window);
        }
        else if (gameState == STATE_SELECT) {
            ui.drawLevelSelect(window, levelStars, unlockedCount);
        }
        else if (gameState == STATE_LEADERBOARD) {
            ui.drawLeaderboard(window, topScores, scoreCount);
        }
        else if (gameState == STATE_ENDLESS) {
            levels.hitboxCount = 0;
            int prevAct = levels.act;
            levels.act = 3; // Show AVL balance badges
            levels.subLevel = 1;
            treeView.drawNode(window, endlessTree.root, 500.0f, 130.0f, 220.0f, levels);
            levels.act = prevAct;
            ui.drawEndlessHUD(window, (int)endlessClock.getElapsedTime().asSeconds(), endlessOps, endlessStatus);
        }
        else if (gameState == STATE_PLAY) {
            levels.hitboxCount = 0;

            if (levels.act == 1) {
                if (levels.subLevel == 2) treeView.drawReconstructionSlots(window, levels);
                else treeView.drawNode(window, levels.act1Tree.root, 500.0f, 130.0f, 220.0f, levels);
            }
            else if (levels.act == 2) {
                treeView.drawNode(window, levels.act2Tree.root, 500.0f, 130.0f, 220.0f, levels);
            }
            else if (levels.act == 3) {
                treeView.drawNode(window, levels.act3Tree.root, 500.0f, 130.0f, 220.0f, levels);
            }
            else if (levels.act == 4) {
                heapView.highlightIdxA = levels.selectedHeapIdx;
                heapView.highlightIdxB = -1;
                for (int i = 0; i < levels.act4Heap.total; i++) {
                    sf::Vector2f pos = heapView.getNodePos(i);
                    levels.registerHitbox(i, levels.act4Heap.data[i], pos.x, pos.y, 20.0f);
                }
                heapView.drawHeapTree(window, levels.act4Heap);
                heapView.drawHeapArray(window, levels.act4Heap);
            }

            ui.drawHUD(window, levels.act, levels.subLevel, levels.title, levels.objective, levels.instruction, levels.scoreSystem);

            if (levels.campaignComplete) {
                ui.drawCampaignCompleteDialog(window, levels.scoreSystem.score, levels.act4Heap.data, levels.act4Heap.total);
            }
            else if (levels.levelWon) {
                ui.drawResultDialog(window, true, levels.scoreSystem.stars, levels.scoreSystem.score);
            }
            else if (levels.levelLost) {
                ui.drawResultDialog(window, false, 0, 0);
            }
        }
        else if (gameState == STATE_SANDBOX) {
            levels.hitboxCount = 0;
            if (sandboxMode == 0) {
                int prevAct = levels.act; levels.act = 3; levels.subLevel = 1;
                treeView.drawNode(window, sandboxAVL.root, 500.0f, 140.0f, 220.0f, levels);
                levels.act = prevAct;
                ui.drawSandboxHUD(window, "AVL Tree", sandboxStatus);
            }
            else if (sandboxMode == 1) {
                int prevAct = levels.act; levels.act = 2; levels.subLevel = 1;
                treeView.drawNode(window, sandboxBST.root, 500.0f, 140.0f, 220.0f, levels);
                levels.act = prevAct;
                ui.drawSandboxHUD(window, "BST", sandboxStatus);
            }
            else {
                heapView.highlightIdxA = -1; heapView.highlightIdxB = -1;
                heapView.drawHeapTree(window, sandboxHeap);
                heapView.drawHeapArray(window, sandboxHeap);
                ui.drawSandboxHUD(window, "Max Heap", sandboxStatus);
            }
        }

        window.display();
    }

    return 0;
}