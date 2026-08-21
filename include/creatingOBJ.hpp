#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <chrono>

class Object
{
private:
    sf::RectangleShape Rectangle;
    sf::Text text;

    float positionX;
    float positionY;

    int health;

    sf::Vector2f velocity;

    std::chrono::steady_clock::time_point lastDirectionChange;

public:
    Object(
        float screenX,
        float screenY,
        const sf::Font& font,
        int number
    );

    void update(float screenX, float screenY);

    void draw(sf::RenderWindow& window);

    sf::Vector2f getPosition();

    void setColor(sf::Color color);

    void setHealth(int amount);

    int getHealth();
};