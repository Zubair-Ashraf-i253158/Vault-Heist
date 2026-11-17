#include <SFML/Graphics.hpp>
#include <iostream>
#include "LevelManager.h"
#include "UIManager.h"
#include "TreeRenderer.h"
#include "HeapRenderer.h"

using namespace std;

// Application States (Section 2.3 & 6.1)
const int STATE_MENU = 0;
const int STATE_SELECT = 1;
const int STATE_PLAY = 2;

int main() {
    int gameState = STATE_MENU;

    LevelManager levels;
    UIManager ui;
    TreeRenderer treeView;
    HeapRenderer heapView;

    sf::RenderWindow window(sf::VideoMode(1000, 680), "Vault Heist - Complete Campaign Engine");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Keyboard navigation
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
                        window.close();
                    }
                }
                else if (gameState == STATE_SELECT) {
                    if (event.key.code == sf::Keyboard::Escape) {
                        gameState = STATE_MENU;
                    }
                }
                else if (gameState == STATE_PLAY) {
                    if (event.key.code == sf::Keyboard::Escape) {
                        gameState = STATE_SELECT;
                    }
                    if (event.key.code == sf::Keyboard::Enter) {
                        levels.nextLevel();
                    }
                    if (event.key.code == sf::Keyboard::R) {
                        levels.retry();
                    }
                }
            }

            // Mouse click handling
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                float mx = (float)event.mouseButton.x;
                float my = (float)event.mouseButton.y;

                if (gameState == STATE_MENU) {
                    // Check button clicks
                    if (mx >= 290.0f && mx <= 710.0f) {
                        if (my >= 235.0f && my <= 285.0f) {
                            levels.loadLevel(1, 1);
                            gameState = STATE_PLAY;
                        }
                        else if (my >= 315.0f && my <= 365.0f) {
                            gameState = STATE_SELECT;
                        }
                        else if (my >= 395.0f && my <= 445.0f) {
                            window.close();
                        }
                    }
                }
                else if (gameState == STATE_SELECT) {
                    // Detect which level cell was clicked
                    for (int a = 0; a < 4; a++) {
                        float y = 110.0f + (a * 125.0f) + 28.0f;
                        for (int l = 1; l <= 5; l++) {
                            float x = 180.0f + ((l - 1) * 140.0f);
                            if (mx >= x && mx <= x + 100.0f && my >= y && my <= y + 45.0f) {
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

        window.clear(sf::Color(16, 18, 24));

        // RENDER SCREEN ACCORDING TO STATE
        if (gameState == STATE_MENU) {
            ui.drawMainMenu(window);
        }
        else if (gameState == STATE_SELECT) {
            ui.drawLevelSelect(window);
        }
        else if (gameState == STATE_PLAY) {
            levels.hitboxCount = 0;

            if (levels.act == 1) {
                treeView.drawNode(window, levels.act1Tree.root, 500.0f, 130.0f, 220.0f, levels);
            }
            else if (levels.act == 2) {
                treeView.drawNode(window, levels.act2Tree.root, 500.0f, 130.0f, 220.0f, levels);
            }
            else if (levels.act == 3) {
                treeView.drawNode(window, levels.act3Tree.root, 500.0f, 130.0f, 220.0f, levels);
            }
            else if (levels.act == 4) {
                for (int i = 0; i < levels.act4Heap.total; i++) {
                    sf::Vector2f pos = heapView.getNodePos(i);
                    levels.registerHitbox(i, levels.act4Heap.data[i], pos.x, pos.y, 20.0f);
                }
                heapView.drawHeapTree(window, levels.act4Heap);
                heapView.drawHeapArray(window, levels.act4Heap);
            }

            ui.drawHUD(window, levels.act, levels.subLevel, levels.title, levels.objective, levels.instruction, levels.scoreSystem);

            if (levels.levelWon) {
                ui.drawResultDialog(window, true, levels.scoreSystem.stars, levels.scoreSystem.score);
            }
            else if (levels.levelLost) {
                ui.drawResultDialog(window, false, 0, 0);
            }
        }

        window.display();
    }

    return 0;
}