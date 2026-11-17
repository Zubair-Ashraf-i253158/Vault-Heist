#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "MaxHeap.h"

using namespace std;

class HeapRenderer {
public:
    sf::Font font;
    bool hasFont;
    int highlightIdxA;
    int highlightIdxB;

    HeapRenderer() {
        hasFont = false;
        highlightIdxA = -1;
        highlightIdxB = -1;

        if (font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
            hasFont = true;
        }
    }

    // Helper to calculate (x, y) coordinates for any heap array index
    sf::Vector2f getNodePos(int index) {
        // Level 0: root (index 0)
        // Level 1: indices 1, 2
        // Level 2: indices 3, 4, 5, 6
        // Level 3: indices 7..14
        int level = 0;
        int temp = index + 1;
        while (temp > 1) {
            temp /= 2;
            level++;
        }

        int levelStart = (1 << level) - 1;
        int posInLevel = index - levelStart;
        int totalInLevel = (1 << level);

        float screenWidth = 1000.0f;
        float xStep = screenWidth / (totalInLevel + 1);
        float x = xStep * (posInLevel + 1);
        float y = 100.0f + (level * 75.0f);

        return sf::Vector2f(x, y);
    }

    // 1. Draw top half: Hierarchical tree view
    void drawHeapTree(sf::RenderWindow& window, MaxHeap& heap) {
        float radius = 20.0f;

        // Draw connecting lines first
        for (int i = 0; i < heap.total; i++) {
            sf::Vector2f parentPos = getNodePos(i);

            int left = 2 * i + 1;
            int right = 2 * i + 2;

            if (left < heap.total) {
                sf::Vector2f leftPos = getNodePos(left);
                sf::Vertex line[] = {
                    sf::Vertex(parentPos, sf::Color(100, 100, 110)),
                    sf::Vertex(leftPos, sf::Color(100, 100, 110))
                };
                window.draw(line, 2, sf::Lines);
            }

            if (right < heap.total) {
                sf::Vector2f rightPos = getNodePos(right);
                sf::Vertex line[] = {
                    sf::Vertex(parentPos, sf::Color(100, 100, 110)),
                    sf::Vertex(rightPos, sf::Color(100, 100, 110))
                };
                window.draw(line, 2, sf::Lines);
            }
        }

        // Draw circular nodes
        for (int i = 0; i < heap.total; i++) {
            sf::Vector2f pos = getNodePos(i);

            sf::CircleShape circle(radius);
            circle.setOrigin(radius, radius);
            circle.setPosition(pos);

            // Active heap vs sorted region styling
            if (i == highlightIdxA || i == highlightIdxB) {
                circle.setFillColor(sf::Color(90, 70, 20));
                circle.setOutlineColor(sf::Color(255, 215, 0)); // Gold glow on active swap
                circle.setOutlineThickness(3.5f);
            }
            else if (i >= heap.heapSize) {
                circle.setFillColor(sf::Color(25, 30, 40));
                circle.setOutlineColor(sf::Color(100, 110, 130)); // Muted grey outline for sorted elements
                circle.setOutlineThickness(2.0f);
            }
            else {
                circle.setFillColor(sf::Color(35, 45, 60));
                circle.setOutlineColor(sf::Color(0, 200, 120)); // Neon green outline for active heap elements
                circle.setOutlineThickness(2.5f);
            }
            window.draw(circle);

            if (hasFont) {
                sf::Text text;
                text.setFont(font);
                text.setString(to_string(heap.data[i]));
                text.setCharacterSize(16);
                text.setFillColor(sf::Color::White);

                sf::FloatRect bounds = text.getLocalBounds();
                text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
                text.setPosition(pos);
                window.draw(text);
            }
        }
    }

    // 2. Draw bottom half: Contiguous array boxes
    void drawHeapArray(sf::RenderWindow& window, MaxHeap& heap) {
        if (heap.total == 0) return;

        float boxWidth = 55.0f;
        float boxHeight = 45.0f;
        float totalWidth = heap.total * (boxWidth + 8.0f);
        float startX = (1000.0f - totalWidth) / 2.0f;
        float startY = 480.0f;

        // Label above the array
        if (hasFont) {
            sf::Text label;
            label.setFont(font);
            label.setCharacterSize(15);
            label.setFillColor(sf::Color(180, 190, 210));
            label.setString("CONTIGUOUS MEMORY ARRAY [Active Heap: 0 to " +
                to_string(heap.heapSize > 0 ? heap.heapSize - 1 : 0) + "]");
            label.setPosition(startX, startY - 30.0f);
            window.draw(label);
        }

        for (int i = 0; i < heap.total; i++) {
            float x = startX + i * (boxWidth + 8.0f);

            sf::RectangleShape box(sf::Vector2f(boxWidth, boxHeight));
            box.setPosition(x, startY);

            // Active swap vs active heap vs sorted partition
            if (i == highlightIdxA || i == highlightIdxB) {
                box.setFillColor(sf::Color(90, 70, 20));
                box.setOutlineColor(sf::Color(255, 215, 0));
                box.setOutlineThickness(3.0f);
            }
            else if (i >= heap.heapSize) {
                box.setFillColor(sf::Color(25, 30, 40));
                box.setOutlineColor(sf::Color(100, 110, 130)); // Sorted partition
                box.setOutlineThickness(1.5f);
            }
            else {
                box.setFillColor(sf::Color(35, 45, 60));
                box.setOutlineColor(sf::Color(0, 200, 120)); // Active heap partition
                box.setOutlineThickness(2.0f);
            }
            window.draw(box);

            if (hasFont) {
                // Value inside box
                sf::Text valText;
                valText.setFont(font);
                valText.setString(to_string(heap.data[i]));
                valText.setCharacterSize(16);
                valText.setFillColor(sf::Color::White);

                sf::FloatRect bounds = valText.getLocalBounds();
                valText.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
                valText.setPosition(x + boxWidth / 2.0f, startY + boxHeight / 2.0f);
                window.draw(valText);

                // Index below box
                sf::Text idxText;
                idxText.setFont(font);
                idxText.setString("[" + to_string(i) + "]");
                idxText.setCharacterSize(12);
                idxText.setFillColor(sf::Color(130, 140, 160));

                sf::FloatRect idxBounds = idxText.getLocalBounds();
                idxText.setOrigin(idxBounds.left + idxBounds.width / 2.0f, 0.0f);
                idxText.setPosition(x + boxWidth / 2.0f, startY + boxHeight + 6.0f);
                window.draw(idxText);
            }
        }
    }

    // Top HUD banner
    void drawHUD(sf::RenderWindow& window, string statusMsg, int heapSize, int total) {
        sf::RectangleShape topBar(sf::Vector2f(1000.0f, 45.0f));
        topBar.setFillColor(sf::Color(25, 28, 36));
        window.draw(topBar);

        if (!hasFont) return;

        sf::Text statusText;
        statusText.setFont(font);
        statusText.setCharacterSize(16);
        statusText.setFillColor(sf::Color(200, 220, 255));
        statusText.setPosition(20.0f, 12.0f);
        statusText.setString(statusMsg);
        window.draw(statusText);

        sf::Text countsText;
        countsText.setFont(font);
        countsText.setCharacterSize(14);
        countsText.setFillColor(sf::Color(0, 200, 120));
        countsText.setPosition(750.0f, 13.0f);
        countsText.setString("Active Heap: " + to_string(heapSize) + " / " + to_string(total));
        window.draw(countsText);
    }

    void renderDualView(sf::RenderWindow& window, MaxHeap& heap, string statusMsg) {
        drawHUD(window, statusMsg, heap.heapSize, heap.total);
        drawHeapTree(window, heap);
        drawHeapArray(window, heap);
    }
};