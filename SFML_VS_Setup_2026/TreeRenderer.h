#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "TreeNode.h"
#include "LevelManager.h"

using namespace std;

class TreeRenderer {
public:
    sf::Font font;
    bool hasFont;

    TreeRenderer() {
        hasFont = false;
        if (font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
            hasFont = true;
        }
    }

    int getNodeHeight(TreeNode* n) {
        return (n == nullptr) ? 0 : n->height;
    }

    // Special renderer for Act 1.2 (Empty slots + Token Bank)
    void drawReconstructionSlots(sf::RenderWindow& window, LevelManager& mgr) {
        float xPos[7] = { 500, 300, 700, 200, 400, 600, 800 };
        float yPos[7] = { 140, 240, 240, 350, 350, 350, 350 };
        int parents[7] = { -1, 0, 0, 1, 1, 2, 2 };

        // Draw edges
        for (int i = 1; i < 7; i++) {
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(xPos[parents[i]], yPos[parents[i]]), sf::Color(100, 100, 115)),
                sf::Vertex(sf::Vector2f(xPos[i], yPos[i]), sf::Color(100, 100, 115))
            };
            window.draw(line, 2, sf::Lines);
        }

        // Draw 7 tree slots
        for (int i = 0; i < 7; i++) {
            mgr.registerHitbox(i, mgr.slotValues[i], xPos[i], yPos[i], 24.0f);

            sf::CircleShape circle(24.0f);
            circle.setOrigin(24.0f, 24.0f);
            circle.setPosition(xPos[i], yPos[i]);
            if (mgr.slotValues[i] == -1) {
                circle.setFillColor(sf::Color(25, 30, 40));
                circle.setOutlineColor(sf::Color(255, 215, 0));
                circle.setOutlineThickness(2.0f);
            }
            else {
                circle.setFillColor(sf::Color(25, 75, 45));
                circle.setOutlineColor(sf::Color(0, 255, 120));
                circle.setOutlineThickness(3.0f);
            }
            window.draw(circle);

            if (hasFont) {
                sf::Text t;
                t.setFont(font);
                t.setString(mgr.slotValues[i] == -1 ? "?" : to_string(mgr.slotValues[i]));
                t.setCharacterSize(18);
                t.setFillColor(sf::Color::White);
                sf::FloatRect b = t.getLocalBounds();
                t.setOrigin(b.left + b.width / 2.0f, b.top + b.height / 2.0f);
                t.setPosition(xPos[i], yPos[i]);
                window.draw(t);
            }
        }

        // Draw Token Bank at bottom
        if (hasFont) {
            sf::Text lbl;
            lbl.setFont(font);
            lbl.setString("AVAILABLE ROOM TOKENS (Click token, then click a [?] slot):");
            lbl.setCharacterSize(14);
            lbl.setFillColor(sf::Color(180, 200, 230));
            lbl.setPosition(200.0f, 525.0f);
            window.draw(lbl);
        }

        for (int i = 0; i < 7; i++) {
            if (mgr.tokenUsed[i]) continue;
            float tx = 200.0f + i * 100.0f;
            float ty = 580.0f;

            sf::CircleShape tok(22.0f);
            tok.setOrigin(22.0f, 22.0f);
            tok.setPosition(tx, ty);
            if (mgr.selectedTokenIdx == i) {
                tok.setFillColor(sf::Color(90, 70, 20));
                tok.setOutlineColor(sf::Color(255, 215, 0));
                tok.setOutlineThickness(3.5f);
            }
            else {
                tok.setFillColor(sf::Color(35, 45, 65));
                tok.setOutlineColor(sf::Color(0, 200, 255));
                tok.setOutlineThickness(2.0f);
            }
            window.draw(tok);

            if (hasFont) {
                sf::Text t;
                t.setFont(font);
                t.setString(to_string(mgr.tokenBank[i]));
                t.setCharacterSize(17);
                t.setFillColor(sf::Color::White);
                sf::FloatRect b = t.getLocalBounds();
                t.setOrigin(b.left + b.width / 2.0f, b.top + b.height / 2.0f);
                t.setPosition(tx, ty);
                window.draw(t);
            }
        }
    }

    void drawNode(sf::RenderWindow& window, TreeNode* node, float x, float y, float xOffset, LevelManager& mgr) {
        if (node == nullptr) return;

        float radius = 24.0f;
        float yGap = 75.0f;

        mgr.registerHitbox(node->id, node->value, x, y, radius);

        if (node->left != nullptr) {
            float childX = x - xOffset;
            float childY = y + yGap;
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(x, y), sf::Color(100, 100, 115)),
                sf::Vertex(sf::Vector2f(childX, childY), sf::Color(100, 100, 115))
            };
            window.draw(line, 2, sf::Lines);
            drawNode(window, node->left, childX, childY, xOffset * 0.52f, mgr);
        }

        if (node->right != nullptr) {
            float childX = x + xOffset;
            float childY = y + yGap;
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(x, y), sf::Color(100, 100, 115)),
                sf::Vertex(sf::Vector2f(childX, childY), sf::Color(100, 100, 115))
            };
            window.draw(line, 2, sf::Lines);
            drawNode(window, node->right, childX, childY, xOffset * 0.52f, mgr);
        }

        sf::CircleShape circle(radius);
        circle.setOrigin(radius, radius);
        circle.setPosition(x, y);

        bool isSkipped = (mgr.act == 2 && mgr.subLevel == 4 && (node->value < mgr.rangeLo || node->value > mgr.rangeHi));

        if (isSkipped) {
            circle.setFillColor(sf::Color(25, 27, 32));
            circle.setOutlineColor(sf::Color(70, 75, 85));
            circle.setOutlineThickness(1.5f);
        }
        else if (mgr.levelWon) {
            circle.setFillColor(sf::Color(25, 75, 45));
            circle.setOutlineColor(sf::Color(0, 255, 120));
            circle.setOutlineThickness(3.5f);
        }
        else if (mgr.levelLost) {
            circle.setFillColor(sf::Color(75, 25, 25));
            circle.setOutlineColor(sf::Color(255, 60, 60));
            circle.setOutlineThickness(3.5f);
        }
        else if (mgr.act == 1 && mgr.subLevel == 3 && mgr.selectedHeightNode == node->value) {
            circle.setFillColor(sf::Color(90, 70, 20));
            circle.setOutlineColor(sf::Color(255, 215, 0));
            circle.setOutlineThickness(3.5f);
        }
        else if (mgr.act == 2 && mgr.subLevel == 4 && !isSkipped) {
            circle.setFillColor(sf::Color(70, 55, 20));
            circle.setOutlineColor(sf::Color(255, 215, 0));
            circle.setOutlineThickness(3.0f);
        }
        else {
            circle.setFillColor(sf::Color(35, 45, 60));
            circle.setOutlineColor(sf::Color(0, 200, 120));
            circle.setOutlineThickness(2.5f);
        }
        window.draw(circle);

        if (hasFont) {
            sf::Text text;
            text.setFont(font);
            text.setString(to_string(node->value));
            text.setCharacterSize(17);
            text.setFillColor(isSkipped ? sf::Color(120, 125, 135) : sf::Color::White);
            sf::FloatRect bounds = text.getLocalBounds();
            text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
            text.setPosition(x, y);
            window.draw(text);

            // Act 1.3 Height Badge
            if (mgr.act == 1 && mgr.subLevel == 3 && node->value >= 1 && node->value <= 7) {
                int h = mgr.enteredHeights[node->value];
                sf::CircleShape badge(11.0f);
                badge.setOrigin(11.0f, 11.0f);
                badge.setPosition(x + 20.0f, y - 20.0f);
                badge.setFillColor(h == -1 ? sf::Color(60, 60, 70) : sf::Color(0, 150, 80));
                badge.setOutlineThickness(1.0f);
                badge.setOutlineColor(sf::Color::White);
                window.draw(badge);

                sf::Text bt;
                bt.setFont(font);
                bt.setString(h == -1 ? "?" : to_string(h));
                bt.setCharacterSize(11);
                bt.setFillColor(sf::Color::White);
                sf::FloatRect bb = bt.getLocalBounds();
                bt.setOrigin(bb.left + bb.width / 2.0f, bb.top + bb.height / 2.0f);
                bt.setPosition(x + 20.0f, y - 20.0f);
                window.draw(bt);
            }

            // Act 3 Badges: Color-only Balance Badge (3.1-3.4), Subtree Size (3.5)
            if (mgr.act == 3) {
                sf::CircleShape badge(9.0f);
                badge.setOrigin(9.0f, 9.0f);
                badge.setPosition(x + 20.0f, y - 20.0f);
                badge.setOutlineThickness(1.5f);
                badge.setOutlineColor(sf::Color::White);

                if (mgr.subLevel == 5) {
                    badge.setFillColor(sf::Color(50, 65, 95));
                    window.draw(badge);

                    sf::Text badgeText;
                    badgeText.setFont(font);
                    badgeText.setCharacterSize(11);
                    badgeText.setString(to_string(node->size));
                    badgeText.setFillColor(sf::Color(255, 215, 0));
                    sf::FloatRect bBounds = badgeText.getLocalBounds();
                    badgeText.setOrigin(bBounds.left + bBounds.width / 2.0f, bBounds.top + bBounds.height / 2.0f);
                    badgeText.setPosition(x + 20.0f, y - 20.0f);
                    window.draw(badgeText);
                }
                else {
                    int bf = getNodeHeight(node->left) - getNodeHeight(node->right);
                    badge.setFillColor((bf < -1 || bf > 1) ? sf::Color(220, 40, 40) : sf::Color(0, 180, 90));
                    window.draw(badge); // Color badge only, no number
                }
            }
        }
    }
};