// player.hpp

#pragma once

#include <SFML/Graphics.hpp>
#include <chrono>

class Player
{
private:
    sf::RectangleShape Rectangle;
    sf::RectangleShape barrel; // the "gun" - swings to face the mouse

    float positionX;
    float positionY;

    int health;
    int maxHealth;

    std::chrono::steady_clock::time_point lastDamageTime{};
    static constexpr float barrelLength = 60.f;
    static constexpr int invulnerabilityMs = 500;

public:
    Player(float screenX, float screenY);

    sf::Vector2f getPosition();     // top-left corner (legacy, still used by walls/HUD)
    sf::Vector2f getCenter();       // center of the body - used for aiming/collisions
    sf::Vector2f getBarrelTip();    // muzzle point - bullets now spawn from here

    // aimPoint = current mouse position in world coords, so the barrel can track it
    bool update(float screenX, float screenY, sf::Vector2f aimPoint);

    void draw(sf::RenderWindow& window);

    void takeDamage(int amount);
    int getHealth() const;
    bool isAlive() const;
    bool isInvulnerable() const;
};
