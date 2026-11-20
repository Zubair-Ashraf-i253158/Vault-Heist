#include <SFML/Graphics.hpp>
#include <iostream>
#include "LevelManager.h"
#include "UIManager.h"
#include "TreeRenderer.h"
#include "HeapRenderer.h"

using namespace std;

const int STATE_MENU = 0;
const int STATE_SELECT = 1;
const int STATE_PLAY = 2;

int main()
{
    int gameState = STATE_MENU;

    LevelManager levels;
    UIManager ui;
    TreeRenderer treeView;
    HeapRenderer heapView;

    sf::RenderWindow window(sf::VideoMode(1000, 680), "Vault Heist with ABDULLAH AND ZUBAIR");

    while (window.isOpen())//window open hone tak loop chalega
    {
        sf::Event event;
        while (window.pollEvent(event))//events check karega
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed)//keyboard press check karega
            {
                if (gameState == STATE_MENU)
                {
                    if (event.key.code == sf::Keyboard::Num1)
                    {
                        levels.loadLevel(1, 1);
                        gameState = STATE_PLAY;
                    }
                    else if (event.key.code == sf::Keyboard::Num2)
                    {
                        gameState = STATE_SELECT;
                    }
                    else if (event.key.code == sf::Keyboard::Num3)
                    {
                        window.close();
                    }
                }
                else if (gameState == STATE_SELECT)
                {
                    if (event.key.code == sf::Keyboard::Escape)
                    {
                        gameState = STATE_MENU;
                    }
                }
                else if (gameState == STATE_PLAY)
                {
                    if (event.key.code == sf::Keyboard::Escape)
                    {
                        gameState = STATE_SELECT;
                    }
                    if (event.key.code == sf::Keyboard::Enter)
                    {
                        levels.nextLevel();
                    }
                    if (event.key.code == sf::Keyboard::R)
                    {
                        levels.retry();
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)//left click check karega
            {
                float mx = (float)event.mouseButton.x;
                float my = (float)event.mouseButton.y;

                if (gameState == STATE_MENU)
                {
                    if (mx >= 290.0f && mx <= 710.0f)
                    {
                        if (my >= 235.0f && my <= 285.0f)
                        {
                            levels.loadLevel(1, 1);
                            gameState = STATE_PLAY;
                        }
                        else if (my >= 315.0f && my <= 365.0f)
                        {
                            gameState = STATE_SELECT;
                        }
                        else if (my >= 395.0f && my <= 445.0f)
                        {
                            window.close();
                        }
                    }
                }
                else if (gameState == STATE_SELECT)
                {
                    for (int a = 0; a < 4; a++)//4 acts ki rows check karega
                    {
                        float y = 110.0f + (a * 125.0f) + 28.0f;
                        for (int l = 1; l <= 5; l++)//har row ke 5 levels check karega
                        {
                            float x = 180.0f + ((l - 1) * 140.0f);
                            if (mx >= x && mx <= x + 100.0f && my >= y && my <= y + 45.0f)//click ki position check karega
                            {
                                levels.loadLevel(a + 1, l);
                                gameState = STATE_PLAY;
                            }
                        }
                    }
                }
                else if (gameState == STATE_PLAY)
                {
                    levels.handleNodeClick(mx, my);
                }
            }
        }

        window.clear(sf::Color(16, 18, 24));

        if (gameState == STATE_MENU)
        {
            ui.drawMainMenu(window);
        }
        else if (gameState == STATE_SELECT)
        {
            ui.drawLevelSelect(window);
        }
        else if (gameState == STATE_PLAY)
        {
            levels.hitboxCount = 0;

            if (levels.act == 1)
            {
                treeView.drawNode(window, levels.act1Tree.root, 500.0f, 130.0f, 220.0f, levels);
            }
            else if (levels.act == 2)
            {
                treeView.drawNode(window, levels.act2Tree.root, 500.0f, 130.0f, 220.0f, levels);
            }
            else if (levels.act == 3)
            {
                treeView.drawNode(window, levels.act3Tree.root, 500.0f, 130.0f, 220.0f, levels);
            }
            else if (levels.act == 4)
            {
                for (int i = 0; i < levels.act4Heap.total; i++)//heap ke sare nodes ki position nikalta hai
                {
                    sf::Vector2f pos = heapView.getNodePos(i);
                    levels.registerHitbox(i, levels.act4Heap.data[i], pos.x, pos.y, 20.0f);
                }
                heapView.drawHeapTree(window, levels.act4Heap);
                heapView.drawHeapArray(window, levels.act4Heap);
            }

            ui.drawHUD(window, levels.act, levels.subLevel, levels.title, levels.objective, levels.instruction, levels.scoreSystem);

            if (levels.levelWon)
            {
                ui.drawResultDialog(window, true, levels.scoreSystem.stars, levels.scoreSystem.score);
            }
            else if (levels.levelLost)
            {
                ui.drawResultDialog(window, false, 0, 0);
            }
        }

        window.display();
    }

    return 0;
}