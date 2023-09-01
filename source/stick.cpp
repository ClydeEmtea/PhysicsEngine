#include "stick.h"

Stick::Stick(Joint* jointA, Joint* jointB, const sf::Color color)
{
	this->jointA = jointA;
	this->jointB = jointB;
	this->length = sqrt(pow(jointA->position.x - jointB->position.x, 2) + pow(jointA->position.y - jointB->position.y, 2));
	this->color = color;
}

void Stick::draw(sf::RenderWindow& window, const float thickness) const
{
	const sf::Vector2f delta = jointB->position - jointA->position;
	const float angle = atan2(delta.y, delta.x) * 180.0f / 3.14159265f;
	const float length_ = std::hypot(delta.x, delta.y);

	sf::RectangleShape line(sf::Vector2f(length_, thickness));
	line.setOrigin(0.0f, thickness / 2.0f);  // Set the origin to the left edge and center vertically
	line.setPosition(jointA->position);
	line.setRotation(angle);
	line.setFillColor(color);

	window.draw(line);
}


void Stick::update() const
{
	const sf::Vector2f delta = jointB->position - jointA->position;
	const float dist = std::hypot(delta.x, delta.y);
	const float diff = (dist - length) / dist;
	const float percent = diff / 2;
	const sf::Vector2f offset = delta * percent;
		if (!jointA->fixed) jointA->position += offset;
		if (!jointB->fixed) jointB->position -= offset;
}

