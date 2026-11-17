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

    void drawNode(sf::RenderWindow& window, TreeNode* node, float x, float y, float xOffset, LevelManager& mgr) {
        if (node == nullptr) return;

        float radius = 24.0f;
        float yGap = 75.0f;

        // Register coordinates for mouse detection
        mgr.registerHitbox(node->id, node->value, x, y, radius);

        // Connecting lines
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

        // Room Circle
        sf::CircleShape circle(radius);
        circle.setOrigin(radius, radius);
        circle.setPosition(x, y);

        if (mgr.levelWon) {
            circle.setFillColor(sf::Color(25, 75, 45));
            circle.setOutlineColor(sf::Color(0, 255, 120));
            circle.setOutlineThickness(3.5f);
        }
        else if (mgr.levelLost) {
            circle.setFillColor(sf::Color(75, 25, 25));
            circle.setOutlineColor(sf::Color(255, 60, 60));
            circle.setOutlineThickness(3.5f);
        }
        else {
            circle.setFillColor(sf::Color(35, 45, 60));
            circle.setOutlineColor(sf::Color(0, 200, 120));
            circle.setOutlineThickness(2.5f);
        }
        window.draw(circle);

        // Number Value
        if (hasFont) {
            sf::Text text;
            text.setFont(font);
            text.setString(to_string(node->value));
            text.setCharacterSize(17);
            text.setFillColor(sf::Color::White);

            sf::FloatRect bounds = text.getLocalBounds();
            text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
            text.setPosition(x, y);
            window.draw(text);

            // Subtree Size Badge (Section 3.3 requirement for AVL order statistics)
            if (mgr.act == 3) {
                sf::CircleShape badge(10.0f);
                badge.setOrigin(10.0f, 10.0f);
                badge.setPosition(x + 18.0f, y - 18.0f);
                badge.setFillColor(sf::Color(50, 65, 95));
                badge.setOutlineThickness(1.0f);
                badge.setOutlineColor(sf::Color::White);
                window.draw(badge);

                sf::Text badgeText;
                badgeText.setFont(font);
                badgeText.setString(to_string(node->size));
                badgeText.setCharacterSize(11);
                badgeText.setFillColor(sf::Color(255, 215, 0));

                sf::FloatRect bBounds = badgeText.getLocalBounds();
                badgeText.setOrigin(bBounds.left + bBounds.width / 2.0f, bBounds.top + bBounds.height / 2.0f);
                badgeText.setPosition(x + 18.0f, y - 18.0f);
                window.draw(badgeText);
            }
        }
    }
};