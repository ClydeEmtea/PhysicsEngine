#pragma once
#include "imgui.h"
#include "imgui-SFML.h"
#include "SFML/Graphics.hpp"
#include "stick.h"
#include "joint.h"

class DrawingHandler {
public: 
	void drawBackground(sf::RenderWindow& window, int height, int width, int lineThickness, int dotSpacing, sf::Color color);
	void drawJoints(sf::RenderWindow& window, std::vector<Joint*>& joints, float radius);
	void drawSticks(sf::RenderWindow& window, std::vector<Stick*>& sticks, float lineThickness);
};