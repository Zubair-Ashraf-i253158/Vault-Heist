#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "MaxHeap.h"

using namespace std;

class HeapRenderer
{
public:
    sf::Font font;
    bool hasFont;
    int highlightIdxA;
    int highlightIdxB;

    HeapRenderer()//heap renderer initialize karega
    {
        hasFont = false;
        highlightIdxA = -1;
        highlightIdxB = -1;

        if (font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf"))//font file load karega
        {
            hasFont = true;
        }
    }

    sf::Vector2f getNodePos(int index)//heap ke node ki screen position calculate karega
    {
        int level = 0;
        int temp = index + 1;
        while (temp > 1)//node kis level par hai check karega
        {
            temp /= 2;
            level++;
        }

        int levelStart = (1 << level) - 1;//current level ka starting index
        int posInLevel = index - levelStart;//level mein node ki position
        int totalInLevel = (1 << level);//current level mein total nodes

        float screenWidth = 1000.0f;
        float xStep = screenWidth / (totalInLevel + 1);//nodes ke darmiyan distance
        float x = xStep * (posInLevel + 1);
        float y = 100.0f + (level * 75.0f);//level ke hisaab se vertical position

        return sf::Vector2f(x, y);//node ki position return karega
    }

    void drawHeapTree(sf::RenderWindow& window, MaxHeap& heap)//heap ko tree ki form mein draw karega
    {
        float radius = 20.0f;

        for (int i = 0; i < heap.total; i++)//pehle tamam nodes ki connecting lines draw karega
        {
            sf::Vector2f parentPos = getNodePos(i);

            int left = 2 * i + 1;//left child ka index
            int right = 2 * i + 2;//right child ka index

            if (left < heap.total)//left child exist karta hai ya nahi
            {
                sf::Vector2f leftPos = getNodePos(left);
                sf::Vertex line[] = {
                    sf::Vertex(parentPos, sf::Color(100, 100, 110)),
                    sf::Vertex(leftPos, sf::Color(100, 100, 110))
                };
                window.draw(line, 2, sf::Lines);//left child ki line draw karega
            }

            if (right < heap.total)//right child exist karta hai ya nahi
            {
                sf::Vector2f rightPos = getNodePos(right);
                sf::Vertex line[] = {
                    sf::Vertex(parentPos, sf::Color(100, 100, 110)),
                    sf::Vertex(rightPos, sf::Color(100, 100, 110))
                };
                window.draw(line, 2, sf::Lines);//right child ki line draw karega
            }
        }

        for (int i = 0; i < heap.total; i++)//tamam heap nodes draw karega
        {
            sf::Vector2f pos = getNodePos(i);

            sf::CircleShape circle(radius);
            circle.setOrigin(radius, radius);
            circle.setPosition(pos);

            if (i == highlightIdxA || i == highlightIdxB)//swap hone wale nodes highlight karega
            {
                circle.setFillColor(sf::Color(90, 70, 20));
                circle.setOutlineColor(sf::Color(255, 215, 0));
                circle.setOutlineThickness(3.5f);
            }
            else if (i >= heap.heapSize)//sorted elements ko grey color dega
            {
                circle.setFillColor(sf::Color(25, 30, 40));
                circle.setOutlineColor(sf::Color(100, 110, 130));
                circle.setOutlineThickness(2.0f);
            }
            else//active heap elements ko green outline dega
            {
                circle.setFillColor(sf::Color(35, 45, 60));
                circle.setOutlineColor(sf::Color(0, 200, 120));
                circle.setOutlineThickness(2.5f);
            }
            window.draw(circle);

            if (hasFont)//node ki value screen par show karega
            {
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

    void drawHeapArray(sf::RenderWindow& window, MaxHeap& heap)//heap ko array ki form mein draw karega
    {
        if (heap.total == 0)//heap empty ho to function end karega
            return;

        float boxWidth = 55.0f;
        float boxHeight = 45.0f;
        float totalWidth = heap.total * (boxWidth + 8.0f);
        float startX = (1000.0f - totalWidth) / 2.0f;//array ko center mein rakhega
        float startY = 480.0f;

        if (hasFont)//array ke upar label show karega
        {
            sf::Text label;
            label.setFont(font);
            label.setCharacterSize(15);
            label.setFillColor(sf::Color(180, 190, 210));
            label.setString("CONTIGUOUS MEMORY ARRAY [Active Heap: 0 to " +
                to_string(heap.heapSize > 0 ? heap.heapSize - 1 : 0) + "]");
            label.setPosition(startX, startY - 30.0f);
            window.draw(label);
        }

        for (int i = 0; i < heap.total; i++)//array ke tamam boxes draw karega
        {
            float x = startX + i * (boxWidth + 8.0f);

            sf::RectangleShape box(sf::Vector2f(boxWidth, boxHeight));
            box.setPosition(x, startY);

            if (i == highlightIdxA || i == highlightIdxB)//swap hone wale boxes highlight karega
            {
                box.setFillColor(sf::Color(90, 70, 20));
                box.setOutlineColor(sf::Color(255, 215, 0));
                box.setOutlineThickness(3.0f);
            }
            else if (i >= heap.heapSize)//sorted part ko grey color dega
            {
                box.setFillColor(sf::Color(25, 30, 40));
                box.setOutlineColor(sf::Color(100, 110, 130));
                box.setOutlineThickness(1.5f);
            }
            else//active heap part ko green outline dega
            {
                box.setFillColor(sf::Color(35, 45, 60));
                box.setOutlineColor(sf::Color(0, 200, 120));
                box.setOutlineThickness(2.0f);
            }
            window.draw(box);

            if (hasFont)//box ke andar value aur neeche index show karega
            {
                sf::Text valText;
                valText.setFont(font);
                valText.setString(to_string(heap.data[i]));
                valText.setCharacterSize(16);
                valText.setFillColor(sf::Color::White);

                sf::FloatRect bounds = valText.getLocalBounds();
                valText.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
                valText.setPosition(x + boxWidth / 2.0f, startY + boxHeight / 2.0f);
                window.draw(valText);

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

    void drawHUD(sf::RenderWindow& window, string statusMsg, int heapSize, int total)//screen ke top par status show karega
    {
        sf::RectangleShape topBar(sf::Vector2f(1000.0f, 45.0f));
        topBar.setFillColor(sf::Color(25, 28, 36));
        window.draw(topBar);

        if (!hasFont)//font na ho to text draw nahi karega
            return;

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

    void renderDualView(sf::RenderWindow& window, MaxHeap& heap, string statusMsg)//heap ke dono views draw karega
    {
        drawHUD(window, statusMsg, heap.heapSize, heap.total);//top status bar draw karega
        drawHeapTree(window, heap);//heap tree view draw karega
        drawHeapArray(window, heap);//heap array view draw karega
    }
};