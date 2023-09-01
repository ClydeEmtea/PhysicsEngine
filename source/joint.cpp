#include "joint.h"

Joint::Joint(const sf::Vector2f position, const sf::Color color, const bool fixed)
{
	this->position = position;
	this->oldPosition = position;
	this->color = color;
	this->fixed = fixed;
}

void Joint::draw(sf::RenderWindow& window, const float radius) const
{
	sf::CircleShape circle(radius);
	circle.setOrigin(radius, radius);
	circle.setPosition(position);
	circle.setFillColor(color);
	window.draw(circle);
}

void Joint::update(float deltaTime, sf::Vector2f gravity)
{
	if (!fixed)
	{
		const sf::Vector2f velocity = position - oldPosition;
		oldPosition = position;
		position += velocity + gravity * deltaTime * deltaTime;
	}
}
