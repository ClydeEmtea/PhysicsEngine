#pragma once

#include <SFML/Graphics.hpp>

class Joint
{
public:
	sf::Vector2f position;
	sf::Vector2f oldPosition;
	bool fixed;
	sf::Color color;
	bool deleted = false;

	Joint(sf::Vector2f position, sf::Color color, bool fixed = false);
	void draw(sf::RenderWindow& window, float radius) const;
	void update(float deltaTime, sf::Vector2f gravity);
};