#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include "LevelManager.h"
#include "UIManager.h"
#include "TreeRenderer.h"
#include "HeapRenderer.h"
#include "SaveManager.h"
#include <SFML/Audio.hpp>

using namespace std;

const int STATE_MENU = 0;
const int STATE_SELECT = 1;
const int STATE_PLAY = 2;
const int STATE_SANDBOX = 3;
const int STATE_ENDLESS = 4;
const int STATE_LEADERBOARD = 5;

int main()//game start karega
{
    int gameState = STATE_MENU;

    LevelManager levels;
    UIManager ui;
    TreeRenderer treeView;
    HeapRenderer heapView;

    int unlockedCount = 1;//starting mein level 1 unlock hoga
    int levelStars[4][5] = { 0 };
    int gameSeed = 42;
    SaveManager::loadProgress(unlockedCount, levelStars, gameSeed);//saved progress load karega
    srand(gameSeed);//random numbers ke liye seed set karega

    int topScores[5] = { 0 };
    int scoreCount = SaveManager::loadLeaderboard(topScores);//leaderboard load karega

    AVL sandboxAVL;
    BST sandboxBST;
    MaxHeap sandboxHeap;
    int sandboxMode = 0;//0 AVL 1 BST 2 MaxHeap
    string sandboxStatus = "Ready";
    string sandboxInput = "";

    AVL endlessTree;
    sf::Clock endlessClock;
    sf::Clock opClock;
    int endlessOps = 0;
    string endlessStatus = "Surviving...";

    sf::RenderWindow window(sf::VideoMode(1000, 680), "VAULT HEIST: ECHOES OF THE LIVING VAULT");//game window create karega
    window.setFramerateLimit(60);
    // 1. Overall Background Music (loops forever)
    sf::Music bgMusic;
    if (bgMusic.openFromFile("assets/backk.wav")) {
        bgMusic.setLoop(true);
        bgMusic.setVolume(35.0f);
        bgMusic.play();
    }

    // 2. Alarm Full Siren (plays when level is lost)
    sf::SoundBuffer fullAlarmBuf;
    sf::Sound fullAlarmSound;
    if (fullAlarmBuf.loadFromFile("assets/alarm.ogg")) {
        fullAlarmSound.setBuffer(fullAlarmBuf);
        fullAlarmSound.setVolume(90.0f);
    }

    bool playedFullSiren = false;
    while (window.isOpen())//jab tak window open hai game chalta rahega
    {
        sf::Event event;
        while (window.pollEvent(event))//window ke events check karega
        {
            if (event.type == sf::Event::Closed)//window close hone par
            {
                SaveManager::saveProgress(unlockedCount, levelStars, gameSeed);
                window.close();
            }
            // Typing custom numbers in Sandbox
            if (event.type == sf::Event::TextEntered && gameState == STATE_SANDBOX) {
                char ch = (char)event.text.unicode;
                if (ch >= '0' && ch <= '9' && sandboxInput.length() < 4) {
                    sandboxInput += ch;
                }
                else if (ch == 8 && !sandboxInput.empty()) { // 8 = Backspace
                    sandboxInput.pop_back();
                }
            }

            if (event.type == sf::Event::KeyPressed)//keyboard input handle karega
            {
                if (gameState == STATE_MENU)//main menu ke options handle karega
                {
                    if (event.key.code == sf::Keyboard::Num1)
                    {
                        levels.isEndlessMode = false;
                        levels.loadLevel(1, 1);
                        gameState = STATE_PLAY;
                    }
                    else if (event.key.code == sf::Keyboard::Num2)
                    {
                        gameState = STATE_SELECT;
                    }
                    else if (event.key.code == sf::Keyboard::Num3)//endless mode start karega
                    {
                        levels.isEndlessMode = true;
                        levels.endlessTotalScore = 0;
                        levels.endlessSolved = 0;
                        levels.loadRandomEndlessPuzzle();
                        gameState = STATE_PLAY;
                    }
                    else if (event.key.code == sf::Keyboard::Num4)//sandbox ke data structures initialize karega
                    {
                        sandboxAVL.clear(); sandboxAVL.insert(50); sandboxAVL.insert(30); sandboxAVL.insert(70);
                        sandboxBST.clear(); sandboxBST.insert(50); sandboxBST.insert(30); sandboxBST.insert(70);
                        sandboxHeap.total = 0; sandboxHeap.heapSize = 0; sandboxHeap.insert(50); sandboxHeap.insert(30); sandboxHeap.insert(70);
                        gameState = STATE_SANDBOX;
                    }
                    else if (event.key.code == sf::Keyboard::Num5)
                    {
                        scoreCount = SaveManager::loadLeaderboard(topScores);
                        gameState = STATE_LEADERBOARD;
                    }
                    else if (event.key.code == sf::Keyboard::Num6)//progress save karke game close karega
                    {
                        SaveManager::saveProgress(unlockedCount, levelStars, gameSeed);
                        window.close();
                    }
                }
                else if (gameState == STATE_SELECT)//level selection screen ke controls
                {
                    if (event.key.code == sf::Keyboard::Escape)
                        gameState = STATE_MENU;
                    else if (event.key.code == sf::Keyboard::U)//viva ke liye tamam levels unlock karega
                    {
                        unlockedCount = 20;
                        SaveManager::saveProgress(unlockedCount, levelStars, gameSeed);
                    }
                }
                else if (gameState == STATE_LEADERBOARD)//leaderboard ke controls
                {
                    if (event.key.code == sf::Keyboard::Escape)
                        gameState = STATE_MENU;
                }
                else if (gameState == STATE_ENDLESS)//endless mode ke controls
                {
                    if (event.key.code == sf::Keyboard::Escape)//endless mode end karega
                    {
                        int finalPts = (int)endlessClock.getElapsedTime().asSeconds() * 10 + endlessOps * 5;
                        SaveManager::saveLeaderboardScore(finalPts);
                        scoreCount = SaveManager::loadLeaderboard(topScores);
                        gameState = STATE_LEADERBOARD;
                    }
                }
                else if (gameState == STATE_PLAY)//campaign gameplay ke controls
                {
                    if (event.key.code == sf::Keyboard::Escape)
                    {
                        if (levels.isEndlessMode)
                        {
                            if (levels.endlessTotalScore > 0)
                            {
                                SaveManager::saveLeaderboardScore(levels.endlessTotalScore);
                                scoreCount = SaveManager::loadLeaderboard(topScores);
                            }
                            levels.isEndlessMode = false;
                            gameState = STATE_MENU;
                        }
                        else if (levels.campaignComplete)//campaign complete hone par menu mein jayega
                        {
                            levels.campaignComplete = false;
                            gameState = STATE_MENU;
                        }
                        else
                        {
                            gameState = STATE_SELECT;
                        }
                    }
                    else if (event.key.code == sf::Keyboard::Enter)//level complete hone par next level par jayega
                    {
                        if (levels.levelWon)
                        {
                            if (levels.isEndlessMode)
                            {
                                levels.endlessTotalScore += levels.scoreSystem.score;
                                levels.endlessSolved++;
                                levels.loadRandomEndlessPuzzle();
                            }
                            else
                            {
                                int aIdx = levels.act - 1;
                                int lIdx = levels.subLevel - 1;
                                if (levels.scoreSystem.stars > levelStars[aIdx][lIdx])//best stars update karega
                                {
                                    levelStars[aIdx][lIdx] = levels.scoreSystem.stars;
                                }
                                int nextIdx = aIdx * 5 + levels.subLevel + 1;
                                if (nextIdx > unlockedCount && nextIdx <= 20)//next level unlock karega
                                    unlockedCount = nextIdx;
                                SaveManager::saveProgress(unlockedCount, levelStars, gameSeed);
                                levels.nextLevel();
                            }
                        }
                    }
                    else if (event.key.code == sf::Keyboard::Backspace)//current level restart karega
                    {
                        if (levels.isEndlessMode) levels.loadRandomEndlessPuzzle();
                        else levels.retry();
                    }
                    else//baqi keyboard input level manager ko dega
                    {
                        levels.handleKeyPress(event.key.code);
                    }
                }
                else if (gameState == STATE_SANDBOX) {
                    if (event.key.code == sf::Keyboard::Escape) {
                        sandboxInput = "";
                        gameState = STATE_MENU;
                    }
                    else if (event.key.code == sf::Keyboard::T) {
                        sandboxMode = (sandboxMode + 1) % 3;
                        sandboxInput = "";
                        sandboxStatus = "Switched structure";
                    }
                    else if (event.key.code == sf::Keyboard::I) {
                        int val = 0;
                        if (!sandboxInput.empty()) {
                            val = stoi(sandboxInput); // Use typed number
                            sandboxInput = "";
                        }
                        else {
                            val = (rand() % 90) + 10; // Fallback: random
                        }

                        if (sandboxMode == 0) sandboxAVL.insert(val);
                        else if (sandboxMode == 1) sandboxBST.insert(val);
                        else sandboxHeap.insert(val);

                        sandboxStatus = "Inserted " + to_string(val);
                    }
                    else if (event.key.code == sf::Keyboard::D) {
                        if (sandboxMode == 2) {
                            if (sandboxHeap.heapSize > 0) {
                                int m = sandboxHeap.extractMax();
                                sandboxStatus = "Extracted Max " + to_string(m);
                            }
                        }
                        else if (!sandboxInput.empty()) {
                            int val = stoi(sandboxInput); // Delete typed number
                            sandboxInput = "";
                            bool ok = false;
                            if (sandboxMode == 0) ok = sandboxAVL.deleteKey(val);
                            else if (sandboxMode == 1) ok = sandboxBST.deleteKey(val);

                            if (ok) sandboxStatus = "Deleted " + to_string(val);
                            else sandboxStatus = to_string(val) + " not found!";
                        }
                        else {
                            // Fallback: delete root if nothing typed
                            if (sandboxMode == 0 && sandboxAVL.root) {
                                int r = sandboxAVL.root->value;
                                sandboxAVL.deleteKey(r);
                                sandboxStatus = "Deleted Root " + to_string(r);
                            }
                            else if (sandboxMode == 1 && sandboxBST.root) {
                                int r = sandboxBST.root->value;
                                sandboxBST.deleteKey(r);
                                sandboxStatus = "Deleted Root " + to_string(r);
                            }
                        }
                    }
                    else if (event.key.code == sf::Keyboard::H && sandboxMode == 2) {
                        sandboxHeap.heapSort();
                        sandboxStatus = "Heap Sort Complete!";
                    }
                    else if (event.key.code == sf::Keyboard::C) {
                        if (sandboxMode == 0) sandboxAVL.clear();
                        else if (sandboxMode == 1) sandboxBST.clear();
                        else { sandboxHeap.total = 0; sandboxHeap.heapSize = 0; }
                        sandboxInput = "";
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
                float mx = (float)event.mouseButton.x;//mouse ki x position
                float my = (float)event.mouseButton.y;//mouse ki y position

                if (gameState == STATE_MENU) {//menu screen par click
                    if (mx >= 280.0f && mx <= 720.0f) {
                        for (int i = 0; i < 6; i++) {
                            float cy = 165.0f + i * 62.0f;
                            if (my >= cy - 22.0f && my <= cy + 22.0f) {
                                if (i == 0) { levels.isEndlessMode = false; levels.loadLevel(1, 1); gameState = STATE_PLAY; }//campaign start
                                else if (i == 1) gameState = STATE_SELECT;//level select kholega
                                else if (i == 2) {
                                    levels.isEndlessMode = true;
                                    levels.endlessTotalScore = 0;
                                    levels.endlessSolved = 0;
                                    levels.loadRandomEndlessPuzzle();
                                    gameState = STATE_PLAY;//endless mode start
                                }
                                else if (i == 3) {
                                    sandboxAVL.clear(); sandboxAVL.insert(50); sandboxAVL.insert(30); sandboxAVL.insert(70);
                                    gameState = STATE_SANDBOX;//sandbox start
                                }
                                else if (i == 4) {
                                    scoreCount = SaveManager::loadLeaderboard(topScores);
                                    gameState = STATE_LEADERBOARD;//leaderboard kholega
                                }
                                else if (i == 5) {
                                    SaveManager::saveProgress(unlockedCount, levelStars, gameSeed);
                                    window.close();//game band karega
                                }
                            }
                        }
                    }
                }
                else if (gameState == STATE_SELECT) {//level select screen par click
                    for (int a = 0; a < 4; a++) {
                        float y = 85.0f + (a * 135.0f) + 25.0f;
                        for (int l = 1; l <= 5; l++) {
                            int levelIndex = a * 5 + l;
                            if (levelIndex > unlockedCount) continue;//locked level skip karega
                            float x = 150.0f + ((l - 1) * 145.0f);
                            if (mx >= x && mx <= x + 115.0f && my >= y && my <= y + 52.0f) {
                                levels.isEndlessMode = false;
                                levels.loadLevel(a + 1, l);
                                gameState = STATE_PLAY;//selected level start
                            }
                        }
                    }
                }
                else if (gameState == STATE_PLAY) {
                    levels.handleNodeClick(mx, my);//tree ke node par click handle karega
                }
            }
        }

        // Endless Mode live operation stream (ramps up speed)
        if (gameState == STATE_ENDLESS) {//endless mode ki automatic operations
            float interval = 1.8f - (endlessOps * 0.03f);
            if (interval < 0.45f) interval = 0.45f;//minimum speed interval
            if (opClock.getElapsedTime().asSeconds() >= interval) {
                opClock.restart();
                endlessOps++;
                if (rand() % 3 == 0 && endlessTree.root != nullptr) {
                    int r = endlessTree.root->value;
                    endlessTree.deleteKey(r);
                    endlessStatus = "Auto-Deleted " + to_string(r);//root delete karega
                }
                else {
                    int v = (rand() % 90) + 10;
                    endlessTree.insert(v);
                    endlessStatus = "Auto-Inserted " + to_string(v);//random value insert karega
                }
            }
        }
        // Play siren when alarm is full, stop it on retry or exit
        if (gameState == STATE_PLAY) {
            if (levels.levelLost && !playedFullSiren) {
                fullAlarmSound.play();
                playedFullSiren = true;
            }
            if (!levels.levelLost && playedFullSiren) {
                fullAlarmSound.stop();
                playedFullSiren = false;
            }
        }
        else if (playedFullSiren) {
            fullAlarmSound.stop();
            playedFullSiren = false;
        }
        window.clear(sf::Color(16, 18, 24));//screen clear karega
        ui.drawBackground(window, gameState == STATE_MENU); //menu par alag aur baqi screens par alag background

        if (gameState == STATE_MENU) {
            ui.drawMainMenu(window);//main menu draw
        }
        else if (gameState == STATE_SELECT) {
            ui.drawLevelSelect(window, levelStars, unlockedCount);//levels draw
        }
        else if (gameState == STATE_LEADERBOARD) {
            ui.drawLeaderboard(window, topScores, scoreCount);//scores draw
        }
        else if (gameState == STATE_ENDLESS) {
            levels.hitboxCount = 0;
            int prevAct = levels.act;
            levels.act = 3; // Show AVL balance badges
            levels.subLevel = 1;
            treeView.drawNode(window, endlessTree.root, 500.0f, 130.0f, 220.0f, levels);//endless tree draw
            levels.act = prevAct;
            ui.drawEndlessHUD(window, (int)endlessClock.getElapsedTime().asSeconds(), endlessOps, endlessStatus);//endless stats draw
        }
        else if (gameState == STATE_PLAY) {
            levels.hitboxCount = 0;

            if (levels.act == 1) {
                if (levels.subLevel == 2) treeView.drawReconstructionSlots(window, levels);//reconstruction slots draw
                else treeView.drawNode(window, levels.act1Tree.root, 500.0f, 130.0f, 220.0f, levels);//act 1 tree draw
            }
            else if (levels.act == 2) {
                treeView.drawNode(window, levels.act2Tree.root, 500.0f, 130.0f, 220.0f, levels);//act 2 tree draw
            }
            else if (levels.act == 3) {
                treeView.drawNode(window, levels.act3Tree.root, 500.0f, 130.0f, 220.0f, levels);//act 3 tree draw
            }
            else if (levels.act == 4) {
                heapView.highlightIdxA = levels.selectedHeapIdx;
                heapView.highlightIdxB = -1;
                for (int i = 0; i < levels.act4Heap.total; i++) {
                    sf::Vector2f pos = heapView.getNodePos(i);
                    levels.registerHitbox(i, levels.act4Heap.data[i], pos.x, pos.y, 20.0f);//heap nodes ke hitboxes
                }
                heapView.drawHeapTree(window, levels.act4Heap);//heap tree draw
                heapView.drawHeapArray(window, levels.act4Heap);//heap array draw
            }

            ui.drawHUD(window, levels.act, levels.subLevel, levels.title, levels.objective, levels.instruction, levels.scoreSystem);//level information draw

            if (levels.campaignComplete) {
                ui.drawCampaignCompleteDialog(window, levels.scoreSystem.score, levels.act4Heap.data, levels.act4Heap.total);//campaign complete screen
            }
            else if (levels.levelWon) {
                ui.drawResultDialog(window, true, levels.scoreSystem.stars, levels.scoreSystem.score);//win result show
            }
            else if (levels.levelLost) {
                ui.drawResultDialog(window, false, 0, 0);//lose result show
            }
        }
        else if (gameState == STATE_SANDBOX) {
            levels.hitboxCount = 0;
            if (sandboxMode == 0) {
                int prevAct = levels.act; levels.act = 3; levels.subLevel = 1;
                treeView.drawNode(window, sandboxAVL.root, 500.0f, 140.0f, 220.0f, levels);//AVL tree draw
                levels.act = prevAct;
                ui.drawSandboxHUD(window, "AVL Tree", sandboxStatus, sandboxInput);//AVL controls show
            }
            else if (sandboxMode == 1) {
                int prevAct = levels.act; levels.act = 2; levels.subLevel = 1;
                treeView.drawNode(window, sandboxBST.root, 500.0f, 140.0f, 220.0f, levels);//BST draw
                levels.act = prevAct;
                ui.drawSandboxHUD(window, "BST", sandboxStatus, sandboxInput);//BST controls show
            }
            else {
                heapView.highlightIdxA = -1; heapView.highlightIdxB = -1;
                heapView.drawHeapTree(window, sandboxHeap);//heap tree draw
                heapView.drawHeapArray(window, sandboxHeap);//heap array draw
                ui.drawSandboxHUD(window, "Max Heap", sandboxStatus, sandboxInput);//heap controls show
            }
        }

        window.display();//frame screen par show karega
    }

    return 0;
}