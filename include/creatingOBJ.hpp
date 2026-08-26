#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <chrono>

enum class ObjectState
{
    Wander,     // drifts around at random, like before
    Charging,   // locked onto the player, dashing straight at them
    Retreating  // peeling off after a charge, like a plane breaking off a chicken run
};

class Object
{
private:
    sf::RectangleShape Rectangle;
    sf::Text text;

    float positionX;
    float positionY;

    int health;
    int maxHealth;

    sf::Vector2f velocity;

    ObjectState state;

    std::chrono::steady_clock::time_point lastDirectionChange;
    std::chrono::steady_clock::time_point chargeStartTime;

public:
    Object(
        float screenX,
        float screenY,
        const sf::Font& font,
        int level
    );

    // playerCenter/level are needed so the enemy can charge the player and
    // scale its speed/aggression as the game gets harder.
    void update(float screenX, float screenY, sf::Vector2f playerCenter, int level);

    void draw(sf::RenderWindow& window);

    sf::Vector2f getPosition();

    void setColor(sf::Color color);

    void setHealth(int amount);

    int getHealth();

    bool isCharging() const;

    int getDamage(int level) const;

    void forceRetreat();
};
