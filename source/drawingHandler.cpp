#include "drawingHandler.h"


void DrawingHandler::drawBackground(sf::RenderWindow& window, int height, int width, int lineThickness, int dotSpacing, sf::Color color)
{
    // draws horizontal lines
    for (int i = 0; i < height; i += 100) {
        if (i == 0) continue;
        sf::RectangleShape line(sf::Vector2f(width, lineThickness));
        line.setPosition(0, i);
        line.setFillColor(color);
        window.draw(line);
    }
    // draws vertical lines
    for (int i = 0; i < width; i += 100) {
        if (i == 0) continue;
        sf::RectangleShape line(sf::Vector2f(lineThickness, height));
        line.setPosition(i, 0);
        line.setFillColor(color);
        window.draw(line);
    }
    // draws dotted horizontal lines
    for (int i = 50; i < height; i += 100) {
        for (int j = 0; j < width; j += dotSpacing) {
            sf::RectangleShape line(sf::Vector2f(lineThickness, lineThickness / 2));
            line.setPosition(j, i);
            line.setFillColor(color);
            window.draw(line);
        }
    }
    // draws dotted vertical lines
    for (int i = 50; i < width; i += 100) {
        for (int j = 0; j < height; j += dotSpacing) {
            sf::RectangleShape line(sf::Vector2f(lineThickness / 2, lineThickness));
            line.setPosition(i, j);
            line.setFillColor(color);
            window.draw(line);
        }
    }
}

void DrawingHandler::drawJoints(sf::RenderWindow& window, std::vector<Joint*>& joints, float radius)
{
    for (int i = 0; i < joints.size(); i++)
    {
		joints[i]->draw(window, radius / 2);
	}
}

void DrawingHandler::drawSticks(sf::RenderWindow& window, std::vector<Stick*>& sticks, float lineThickness)
{
    for (int i = 0; i < sticks.size(); i++)
    {
        sticks[i]->draw(window, lineThickness);
    }
}

