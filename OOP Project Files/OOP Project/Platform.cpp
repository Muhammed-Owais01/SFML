#include "Platform.h"

void Platform::initVariables(sf::RenderWindow& window)
{
	this->shape.setSize(sf::Vector2f(100.f, 100.f));
	this->shape.setFillColor(sf::Color::Green);
	this->shape.setPosition(window.getSize().x / 2, window.getSize().y / 2);
}

Platform::Platform()
{

}

Platform::Platform(sf::RenderWindow& window)
{
	this->initVariables(window);
}

Platform::~Platform()
{

}

const sf::FloatRect& Platform::getBounds() const
{
	return this->shape.getGlobalBounds();
}

sf::Vector2f Platform::getPos()
{
	return this->shape.getPosition();
}

void Platform::setPos(int x, int y)
{
	this->shape.setPosition(x, y);
}

void Platform::update()
{

}

void Platform::render(sf::RenderTarget* target)
{
	target->draw(this->shape);
}
