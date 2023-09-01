#pragma once

#include <SFML/Graphics.hpp>
#include "joint.h"

class Stick
{
public:
	Joint* jointA;
	Joint* jointB;
	float length;
	sf::Color color;

	Stick(Joint* jointA, Joint* jointB, sf::Color color);
	void draw(sf::RenderWindow& window, float thickness) const;
	void update() const;

};