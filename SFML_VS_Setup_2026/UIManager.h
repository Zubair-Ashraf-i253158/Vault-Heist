#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "ScoreSystem.h"

using namespace std;

class UIManager {
public:
    sf::Font font;
    bool hasFont;

  

    sf::Texture backgroundTexture; //levels ka background
    sf::Sprite backgroundSprite;
    bool hasBackground;

    sf::Texture menuBgTexture; //start menu ka background
    sf::Sprite menuBgSprite;
    bool hasMenuBg;

    UIManager() {
        hasFont = false;
        hasBackground = false;
        hasMenuBg = false;

        if (font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
            hasFont = true;
        }

        if (backgroundTexture.loadFromFile("assets/back.jpg")) { //level background load karega
            backgroundSprite.setTexture(backgroundTexture);
            hasBackground = true;
        }

        if (menuBgTexture.loadFromFile("assets/menu.jpg")) { //start menu background load karega
            menuBgSprite.setTexture(menuBgTexture);
            hasMenuBg = true;
        }
    }

    void drawBackground(sf::RenderWindow& window, bool isMenu = false) //menu aur level ke liye alag background draw karega
    {
        if (isMenu && hasMenuBg) {
            menuBgSprite.setScale(
                (float)window.getSize().x / menuBgTexture.getSize().x,
                (float)window.getSize().y / menuBgTexture.getSize().y
            );
            window.draw(menuBgSprite);
        }
        else if (hasBackground) {
            backgroundSprite.setScale(
                (float)window.getSize().x / backgroundTexture.getSize().x,
                (float)window.getSize().y / backgroundTexture.getSize().y
            );
            window.draw(backgroundSprite);
        }
    }

    // 1. Main Menu Screen (6 options)
    void drawMainMenu(sf::RenderWindow& window) {
        if (!hasFont) return;

        sf::Text title;
		title.setStyle(sf::Text::Bold);
        title.setFont(font);
        title.setCharacterSize(40);
        title.setFillColor(sf::Color(235, 255, 245));
        title.setString("VAULT HEIST: ECHOES OF THE LIVING VAULT");
        sf::FloatRect b = title.getLocalBounds();
        title.setOrigin(b.left + b.width / 2.0f, 0.0f);
        title.setPosition(500.0f, 55.0f);
        window.draw(title);

        string menuItems[] = {
            "1 PLAY CAMPAIGN",
            "2 CAMPAIGN MAP (Stars and Levels)",
            "3 ENDLESS SURVIVAL MODE",
            "4 SANDBOX MODE (BST / AVL / Heap)",
            "5 LEADERBOARD",
            "6 EXIT THE GAME"
        };

        for (int i = 0; i < 6; i++) {
            sf::RectangleShape btn(sf::Vector2f(440.0f, 44.0f));
            btn.setOrigin(220.0f, 22.0f);
            btn.setPosition(500.0f, 165.0f + (i * 62.0f));
            btn.setFillColor(sf::Color::Black);
            btn.setOutlineThickness(4.0f);
            btn.setOutlineColor(sf::Color(0, 200, 120));
            window.draw(btn);

            sf::Text opt;
            opt.setFont(font);
            opt.setStyle(sf::Text::Bold);
            opt.setCharacterSize(20);
            opt.setFillColor(sf::Color(242, 248, 245));
            opt.setString(menuItems[i]);
            b = opt.getLocalBounds();
            opt.setOrigin(b.left + b.width / 2.0f, b.top + b.height / 2.0f);
            opt.setPosition(500.0f, 165.0f + (i * 62.0f));
            window.draw(opt);
        }
    }

    // 2. Level Select Map (Shows Stars & Locked Levels)
    void drawLevelSelect(sf::RenderWindow& window, int stars[4][5], int unlockedCount) {
        if (!hasFont) return;

        sf::Text title;
        title.setFont(font);
        title.setCharacterSize(26);
        title.setFillColor(sf::Color(255, 215, 0));
        title.setString("CAMPAIGN MAP: SELECT LEVEL");
        sf::FloatRect b = title.getLocalBounds();
        title.setOrigin(b.left + b.width / 2.0f, 0.0f);
        title.setPosition(500.0f, 25.0f);
        window.draw(title);

        string actNames[] = {
            "ACT 1: Blueprint Wing (Binary Tree)",
            "ACT 2: Sorted Wing (BST)",
            "ACT 3: Architect's Wing (AVL)",
            "ACT 4: Heap Fortress (Max Heap)"
        };

        for (int a = 0; a < 4; a++) {
            float y = 85.0f + (a * 135.0f);

            sf::Text actLbl;
            actLbl.setFont(font);
            actLbl.setCharacterSize(20);
            actLbl.setFillColor(sf::Color::White);
            actLbl.setString(actNames[a]);
            actLbl.setPosition(140.0f, y);
            window.draw(actLbl);

            for (int l = 1; l <= 5; l++) {
                int levelIndex = a * 5 + l;
                bool isLocked = (levelIndex > unlockedCount);
                float x = 150.0f + ((l - 1) * 145.0f);

                sf::RectangleShape cell(sf::Vector2f(115.0f, 52.0f));
                cell.setPosition(x, y + 25.0f);
                cell.setFillColor(isLocked ? sf::Color(20, 22, 28) : sf::Color(25, 35, 50));
                cell.setOutlineThickness(1.5f);
                cell.setOutlineColor(isLocked ? sf::Color(70, 75, 85) : sf::Color(0, 200, 120));
                window.draw(cell);

                sf::Text num;
                num.setFont(font);
                num.setCharacterSize(15);
                num.setFillColor(isLocked ? sf::Color(100, 105, 115) : sf::Color::White);
                num.setString(isLocked ? "LOCKED" : (to_string(a + 1) + "." + to_string(l)));
                b = num.getLocalBounds();
                num.setOrigin(b.left + b.width / 2.0f, 0.0f);
                num.setPosition(x + 57.0f, y + 32.0f);
                window.draw(num);

                if (!isLocked && stars[a][l - 1] > 0) {
                    string st = "";
                    for (int s = 0; s < stars[a][l - 1]; s++) st += "* ";
                    sf::Text starTxt;
                    starTxt.setFont(font);
                    starTxt.setCharacterSize(14);
                    starTxt.setFillColor(sf::Color(255, 215, 0));
                    starTxt.setString(st);
                    b = starTxt.getLocalBounds();
                    starTxt.setOrigin(b.left + b.width / 2.0f, 0.0f);
                    starTxt.setPosition(x + 57.0f, y + 53.0f);
                    window.draw(starTxt);
                }
            }
        }

        sf::Text back;
        back.setFont(font);
        back.setCharacterSize(14);
        back.setFillColor(sf::Color(150, 160, 180));
        back.setString("ESC  Return to Menu");
        back.setPosition(20.0f, 645.0f);
        window.draw(back);
    }
	//4 Sandbox Mode HUD
    void drawSandboxHUD(sf::RenderWindow& window, string treeType, string status, string typedInput) {
        sf::RectangleShape topBar(sf::Vector2f(1000.0f, 76.0f));
        topBar.setFillColor(sf::Color(24, 28, 38));
        window.draw(topBar);
        if (!hasFont) return;

        sf::Text title;
        title.setFont(font);
        title.setCharacterSize(17);
        title.setFillColor(sf::Color(0, 220, 255));
        title.setString("SANDBOX: " + treeType);
        title.setPosition(20.0f, 8.0f);
        window.draw(title);

        sf::Text inputTxt;
        inputTxt.setFont(font);
        inputTxt.setCharacterSize(16);
        inputTxt.setFillColor(sf::Color(0, 255, 120));
        inputTxt.setString("KEY: " + (typedInput.empty() ? "Random" : typedInput) );
        inputTxt.setPosition(300.0f, 8.0f);
        window.draw(inputTxt);

        sf::Text stText;
        stText.setFont(font);
        stText.setCharacterSize(14);
        stText.setFillColor(sf::Color(255, 215, 0));
        stText.setString(status);
        stText.setPosition(600.0f, 10.0f);
        window.draw(stText);

        sf::Text controls;
        controls.setFont(font);
        controls.setCharacterSize(12);
        controls.setFillColor(sf::Color(180, 200, 220));
        controls.setString("ENTER VALUE   |   I INSERT   D DELETE   H HEAPSORT   T SWITCH   C CLEAR   S SAVE   L LOAD   ESC MENU");
        controls.setPosition(20.0f, 44.0f);
        window.draw(controls);
    }

    // 6. Endless Mode HUD
    void drawEndlessHUD(sf::RenderWindow& window, int survivalSec, int opsCount, string status) {
        sf::RectangleShape topBar(sf::Vector2f(1000.0f, 72.0f));
        topBar.setFillColor(sf::Color(30, 20, 32));
        window.draw(topBar);
        if (!hasFont) return;

        sf::Text title;
        title.setFont(font);
        title.setCharacterSize(18);
        title.setFillColor(sf::Color(255, 100, 100));
        title.setString("ENDLESS SURVIVAL MODE");
        title.setPosition(20.0f, 10.0f);
        window.draw(title);

        sf::Text info;
        info.setFont(font);
        info.setCharacterSize(13);
        info.setFillColor(sf::Color::White);
        info.setString("Status: " + status + " |  ESC Save Score and Quit");
        info.setPosition(20.0f, 40.0f);
        window.draw(info);

        sf::Text scoreTxt;
        scoreTxt.setFont(font);
        scoreTxt.setCharacterSize(16);
        scoreTxt.setFillColor(sf::Color(255, 215, 0));
        scoreTxt.setString("TIME: " + to_string(survivalSec) + "s | OPS: " + to_string(opsCount));
        scoreTxt.setPosition(700.0f, 22.0f);
        window.draw(scoreTxt);
    }

    // 7. Leaderboard Screen
    void drawLeaderboard(sf::RenderWindow& window, int scores[5], int count) {
        if (!hasFont) return;

        sf::Text title;
        title.setFont(font);
        title.setCharacterSize(30);
        title.setFillColor(sf::Color(255, 215, 0));
        title.setString("ENDLESS MODE LEADERBOARD");
        sf::FloatRect b = title.getLocalBounds();
        title.setOrigin(b.left + b.width / 2.0f, 0.0f);
        title.setPosition(500.0f, 70.0f);
        window.draw(title);

        for (int i = 0; i < 5; i++) {
            sf::RectangleShape row(sf::Vector2f(420.0f, 48.0f));
            row.setOrigin(210.0f, 24.0f);
            row.setPosition(500.0f, 180.0f + i * 65.0f);
            row.setFillColor(sf::Color(25, 30, 42));
            row.setOutlineThickness(1.5f);
            row.setOutlineColor(i == 0 ? sf::Color(255, 215, 0) : sf::Color(0, 200, 120));
            window.draw(row);

            sf::Text txt;
            txt.setFont(font);
            txt.setCharacterSize(17);
            txt.setFillColor(sf::Color::White);
            string line = "#" + to_string(i + 1) + "    ";
            line += (i < count ? (to_string(scores[i]) + " pts") : "Empty");
            txt.setString(line);
            b = txt.getLocalBounds();
            txt.setOrigin(b.left + b.width / 2.0f, b.top + b.height / 2.0f);
            txt.setPosition(500.0f, 180.0f + i * 65.0f);
            window.draw(txt);
        }

        sf::Text back;
        back.setFont(font);
        back.setCharacterSize(14);
        back.setFillColor(sf::Color(150, 160, 180));
        back.setString("ESC  Return to Main Menu");
        back.setPosition(20.0f, 645.0f);
        window.draw(back);
    }

    // 3. Gameplay HUD
    void drawHUD(sf::RenderWindow& window, int actNum, int levelNum, string titleStr, string objective, string instruction, ScoreSystem& scoreSys) {
        sf::RectangleShape banner(sf::Vector2f(1000.0f, 75.0f));
        banner.setFillColor(sf::Color(20, 24, 32));
        window.draw(banner);

        if (!hasFont) return;

        sf::Text title;
        title.setFont(font);
        title.setCharacterSize(18);
        title.setFillColor(sf::Color::Green);
        title.setPosition(20.0f, 8.0f);
        title.setString(titleStr);
        window.draw(title);

        sf::Text instr;
        instr.setFont(font);
        instr.setCharacterSize(13);
        instr.setFillColor(sf::Color::White);
        instr.setPosition(20.0f, 32.0f);
        instr.setString(objective);
        window.draw(instr);

        sf::Text sub;
        sub.setFont(font);
        sub.setCharacterSize(13);
        sub.setFillColor(sf::Color::White   );
        sub.setPosition(20.0f, 52.0f);
        sub.setString(instruction + " H  HINT    |    BACKSPACE  RETRY    |    ESC  MAP");
        window.draw(sub);

        // Score
        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setCharacterSize(14);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setString("SCORE: " + to_string(scoreSys.score));
        scoreText.setPosition(650.0f, 10.0f);
        window.draw(scoreText);

        // Steps
        sf::Text stepsText;
        stepsText.setFont(font);
        stepsText.setCharacterSize(12);
        stepsText.setFillColor(sf::Color::White);
        stepsText.setString("Steps: " + to_string(scoreSys.stepsTaken) + " / Opt: " + to_string(scoreSys.optimalSteps));
        stepsText.setPosition(650.0f, 32.0f);
        window.draw(stepsText);

        // Alarm Bar Gauge
        sf::RectangleShape barOutline(sf::Vector2f(180.0f, 18.0f));
        barOutline.setPosition(790.0f, 32.0f);
        barOutline.setFillColor(sf::Color(35, 38, 48));
        barOutline.setOutlineThickness(1.5f);
		barOutline.setOutlineColor(sf::Color(100, 110, 130));
        window.draw(barOutline);

        float fillRatio = 0.0f;
        if (scoreSys.maxAlarm > 0) fillRatio = (float)scoreSys.currentAlarm / (float)scoreSys.maxAlarm;
        if (fillRatio > 1.0f) fillRatio = 1.0f;

        sf::RectangleShape barFill(sf::Vector2f(180.0f * fillRatio, 18.0f));
        barFill.setPosition(790.0f, 32.0f);
        if (fillRatio < 0.6f) barFill.setFillColor(sf::Color(0, 200, 120));
        else if (fillRatio < 0.85f) barFill.setFillColor(sf::Color(255, 180, 0));
        else barFill.setFillColor(sf::Color(230, 40, 40));
        window.draw(barFill);

        sf::Text alarmLabel;
        alarmLabel.setFont(font);
        alarmLabel.setCharacterSize(12);
        alarmLabel.setFillColor(sf::Color::White);
        alarmLabel.setString("ALARM: " + to_string(scoreSys.currentAlarm) + " / " + to_string(scoreSys.maxAlarm));
        alarmLabel.setPosition(790.0f, 12.0f);
        window.draw(alarmLabel);
    }

    // 5. Result Dialog Modal
    void drawResultDialog(sf::RenderWindow& window, bool won, int stars, int finalScore) {
        sf::RectangleShape overlay(sf::Vector2f(1000.0f, 680.0f));
        overlay.setFillColor(sf::Color(0, 0, 0, 190));
        window.draw(overlay);

        sf::RectangleShape box(sf::Vector2f(440.0f, 220.0f));
        box.setOrigin(220.0f, 110.0f);
        box.setPosition(500.0f, 340.0f);
        box.setFillColor(sf::Color(25, 29, 38));
        box.setOutlineThickness(3.0f);
        box.setOutlineColor(won ? sf::Color(0, 220, 120) : sf::Color(230, 50, 50));
        window.draw(box);

        if (!hasFont) return;

        sf::Text header;
        header.setFont(font);
        header.setCharacterSize(22);
        header.setFillColor(won ? sf::Color(0, 255, 120) : sf::Color(255, 70, 70));
        header.setString(won ? "VAULT COMPLETED!" : "HEIST FAILED! ALARM TRIGGERED");
        sf::FloatRect b = header.getLocalBounds();
        header.setOrigin(b.left + b.width / 2.0f, 0);
        header.setPosition(500.0f, 255.0f);
        window.draw(header);

        sf::Text details;
        details.setFont(font);
        details.setCharacterSize(16);
        details.setFillColor(sf::Color::White);
        if (won) {
            string starStr = "";
            for (int i = 0; i < stars; i++) starStr += "* ";
            details.setString("Stars Earned: " + starStr + "\nScore: " + to_string(finalScore) +
                "\n\nENTER  Next Level   |   ESC  Map");
        }
        else {
            details.setString("Security forces caught you.\n\nBACKSPACE  Retry Level   |   ESC  Map");
        }
        b = details.getLocalBounds();
        details.setOrigin(b.left + b.width / 2.0f, 0);
        details.setPosition(500.0f, 305.0f);
        window.draw(details);
    }

    void drawCampaignCompleteDialog(sf::RenderWindow& window, int finalScore, int arr[], int total) {
        sf::RectangleShape overlay(sf::Vector2f(1000.0f, 680.0f));
        overlay.setFillColor(sf::Color(0, 0, 0, 210));
        window.draw(overlay);

        sf::RectangleShape box(sf::Vector2f(540.0f, 280.0f));
        box.setOrigin(270.0f, 140.0f);
        box.setPosition(500.0f, 340.0f);
        box.setFillColor(sf::Color(20, 26, 36));
        box.setOutlineThickness(4.0f);
        box.setOutlineColor(sf::Color(255, 215, 0));
        window.draw(box);

        if (!hasFont) return;

        sf::Text header;
        header.setFont(font);
        header.setCharacterSize(26);
        header.setFillColor(sf::Color(255, 215, 0));
        header.setString("FINAL VAULT UNLOCKED!");
        sf::FloatRect b = header.getLocalBounds();
        header.setOrigin(b.left + b.width / 2.0f, 0);
        header.setPosition(500.0f, 225.0f);
        window.draw(header);

        string sortedStr = "Sorted Vault Array: [ ";
        for (int i = 0; i < total; i++) {
            sortedStr += to_string(arr[i]) + (i < total - 1 ? ", " : " ");
        }
        sortedStr += "]";

        sf::Text details;
        details.setFont(font);
        details.setCharacterSize(15);
        details.setFillColor(sf::Color::White);
        details.setString(
            "Achievement Badge: Master Vault Thief (All 20 Levels Cleared)\n\n" +
            sortedStr + "\n" +
            "Final Level Score: " + to_string(finalScore) + "\n\n" +
            "ESC Return to Main Menu"
        );
        b = details.getLocalBounds();
        details.setOrigin(b.left + b.width / 2.0f, 0);
        details.setPosition(500.0f, 280.0f);
        window.draw(details);
    }
};