#pragma once
#include "imgui-SFML.h"
#include "SFML/Graphics.hpp"
#include "stick.h"
#include "joint.h"

class DrawingHandler {
public:
	static void drawBackground(sf::RenderWindow& window, int height, int width, int lineThickness, int dotSpacing, sf::Color color);
	static void drawJoints(sf::RenderWindow& window, const std::vector<Joint*>& joints, float radius);
	static void drawSticks(sf::RenderWindow& window, const std::vector<Stick*>& sticks, float lineThickness);
};