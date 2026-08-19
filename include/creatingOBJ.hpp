// creatingOBJ.hpp

#pragma once

#include <SFML/Graphics.hpp>

class Object
{
private:
    sf::RectangleShape Rectangle;
    //sf::Text text;

    float positionX;
    float positionY;

public:
    Object(float screenX, float screenY);

    void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition();
    
    void setColor(sf::Color);
};