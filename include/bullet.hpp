#pragma once

#include <SFML/Graphics.hpp>

class Bullet
{
private:
    sf::CircleShape shape;

    sf::Vector2f position;
    sf::Vector2f target;

public:
    Bullet(sf::Vector2f startPos, sf::Vector2f targetPos);
    sf::Vector2f getPosition();
    bool update();
    void draw(sf::RenderWindow& window);
};