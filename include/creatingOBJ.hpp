#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Object
{
private:
    sf::RectangleShape Rectangle;
    sf::Text text;

    float positionX;
    float positionY;

    int health;

public:
    Object(float screenX, float screenY, const sf::Font& font, int number);

    void draw(sf::RenderWindow& window);

    sf::Vector2f getPosition();

    void setColor(sf::Color color);

    void setHealth(int amount);

    int getHealth();
};