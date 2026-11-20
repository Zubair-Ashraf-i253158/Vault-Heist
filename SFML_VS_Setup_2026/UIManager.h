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

    UIManager()//font load karke check karega
    {
        hasFont = false;
        if (font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf"))
        {
            hasFont = true;
        }
    }

    void drawMainMenu(sf::RenderWindow& window)//main menu screen draw karega
    {
        if (!hasFont) return;

        sf::Text title;
        title.setFont(font);
        title.setCharacterSize(42);
        title.setFillColor(sf::Color(0, 255, 120));
        title.setString("VAULT ZERO");
        sf::FloatRect b = title.getLocalBounds();
        title.setOrigin(b.left + b.width / 2.0f, 0.0f);
        title.setPosition(500.0f, 100.0f);
        window.draw(title);

        sf::Text subtitle;
        subtitle.setFont(font);
        subtitle.setCharacterSize(16);
        subtitle.setFillColor(sf::Color(160, 180, 210));
        subtitle.setString("THE HEIST BEGINS NOW");
        b = subtitle.getLocalBounds();
        subtitle.setOrigin(b.left + b.width / 2.0f, 0.0f);
        subtitle.setPosition(500.0f, 160.0f);
        window.draw(subtitle);

        string options[] = {
            "01 NEW OPERATION",
            "02 MISSION SELECT",
            "03 EXIT GAME"
        };

        for (int i = 0; i < 3; i++)//teen menu buttons draw karega
        {
            sf::RectangleShape btn(sf::Vector2f(420.0f, 50.0f));
            btn.setOrigin(210.0f, 25.0f);
            btn.setPosition(500.0f, 260.0f + (i * 80.0f));
            btn.setFillColor(sf::Color(25, 30, 42));
            btn.setOutlineThickness(2.0f);
            btn.setOutlineColor(sf::Color(0, 200, 120));
            window.draw(btn);

            sf::Text opt;
            opt.setFont(font);
            opt.setCharacterSize(18);
            opt.setFillColor(sf::Color::White);
            opt.setString(options[i]);
            b = opt.getLocalBounds();
            opt.setOrigin(b.left + b.width / 2.0f, b.top + b.height / 2.0f);
            opt.setPosition(500.0f, 260.0f + (i * 80.0f));
            window.draw(opt);
        }
    }

    void drawLevelSelect(sf::RenderWindow& window)//level select screen draw karega
    {
        if (!hasFont) return;

        sf::Text title;
        title.setFont(font);
        title.setCharacterSize(28);
        title.setFillColor(sf::Color(255, 215, 0));
        title.setString("CAMPAIGN MAP: SELECT LEVEL");
        sf::FloatRect b = title.getLocalBounds();
        title.setOrigin(b.left + b.width / 2.0f, 0.0f);
        title.setPosition(500.0f, 40.0f);
        window.draw(title);

        string actNames[] = {
            "ACT 1: Blueprint Wing (Binary Tree)",
            "ACT 2: Sorted Wing (BST)",
            "ACT 3: Balanced Wing (AVL)",
            "ACT 4: Heap Fortress (Max Heap)"
        };

        for (int a = 0; a < 4; a++)//4 acts ki rows draw karega
        {
            float y = 110.0f + (a * 125.0f);

            sf::Text actLbl;
            actLbl.setFont(font);
            actLbl.setCharacterSize(16);
            actLbl.setFillColor(sf::Color(180, 200, 230));
            actLbl.setString(actNames[a]);
            actLbl.setPosition(150.0f, y);
            window.draw(actLbl);

            for (int l = 1; l <= 5; l++)//har act ke 5 levels draw karega
            {
                float x = 180.0f + ((l - 1) * 140.0f);

                sf::RectangleShape cell(sf::Vector2f(100.0f, 45.0f));
                cell.setPosition(x, y + 28.0f);
                cell.setFillColor(sf::Color(25, 30, 42));
                cell.setOutlineThickness(1.5f);
                cell.setOutlineColor(sf::Color(0, 200, 120));
                window.draw(cell);

                sf::Text num;
                num.setFont(font);
                num.setCharacterSize(16);
                num.setFillColor(sf::Color::White);
                num.setString(to_string(a + 1) + "." + to_string(l));
                b = num.getLocalBounds();
                num.setOrigin(b.left + b.width / 2.0f, b.top + b.height / 2.0f);
                num.setPosition(x + 50.0f, y + 50.0f);
                window.draw(num);
            }
        }

        sf::Text back;
        back.setFont(font);
        back.setCharacterSize(14);
        back.setFillColor(sf::Color(150, 160, 180));
        back.setString("[ESC] Return to Main Menu | Click any button to play");
        back.setPosition(20.0f, 640.0f);
        window.draw(back);
    }

    void drawHUD(sf::RenderWindow& window, int actNum, int levelNum, string titleStr, string objective, string instruction, ScoreSystem& scoreSys)//game ki information screen par show karega
    {
        sf::RectangleShape banner(sf::Vector2f(1000.0f, 75.0f));
        banner.setFillColor(sf::Color(20, 24, 32));
        window.draw(banner);

        if (!hasFont) return;

        sf::Text title;
        title.setFont(font);
        title.setCharacterSize(16);
        title.setFillColor(sf::Color(255, 215, 0));
        title.setPosition(20.0f, 8.0f);
        title.setString(titleStr);
        window.draw(title);

        sf::Text instr;
        instr.setFont(font);
        instr.setCharacterSize(13);
        instr.setFillColor(sf::Color(170, 190, 220));
        instr.setPosition(20.0f, 32.0f);
        instr.setString(objective);
        window.draw(instr);

        sf::Text sub;
        sub.setFont(font);
        sub.setCharacterSize(12);
        sub.setFillColor(sf::Color(120, 140, 160));
        sub.setPosition(20.0f, 52.0f);
        sub.setString(instruction + " | [ESC]: Map");
        window.draw(sub);

        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setCharacterSize(14);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setString("SCORE: " + to_string(scoreSys.score));
        scoreText.setPosition(650.0f, 10.0f);
        window.draw(scoreText);

        sf::Text stepsText;
        stepsText.setFont(font);
        stepsText.setCharacterSize(12);
        stepsText.setFillColor(sf::Color(180, 190, 210));
        stepsText.setString("Steps: " + to_string(scoreSys.stepsTaken) + " / Opt: " + to_string(scoreSys.optimalSteps));
        stepsText.setPosition(650.0f, 32.0f);
        window.draw(stepsText);

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

    void drawResultDialog(sf::RenderWindow& window, bool won, int stars, int finalScore)//level complete ya fail hone ka result show karega
    {
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
        if (won)
        {
            string starStr = "";
            for (int i = 0; i < stars; i++) starStr += "* ";
            details.setString("Stars Earned: " + starStr + "\nScore: " + to_string(finalScore) +
                "\n\n[ENTER] Next Level | [ESC] Menu");
        }
        else
        {
            details.setString("Security forces caught you.\n\n[R] Retry Level | [ESC] Menu");
        }
        b = details.getLocalBounds();
        details.setOrigin(b.left + b.width / 2.0f, 0);
        details.setPosition(500.0f, 305.0f);
        window.draw(details);
    }
};