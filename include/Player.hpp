// player.hpp

#pragma once

#include <SFML/Graphics.hpp>

class Player
{
private:
    sf::RectangleShape Rectangle;

    float positionX;
    float positionY;
public:
    Player(float screenX, float screenY);

    sf::Vector2f getPosition();
    bool update(float screenX, float screenY);
    
    void draw(sf::RenderWindow& window);
};