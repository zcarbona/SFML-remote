#include "../include/Player.hpp"
#include <cmath>

Player::Player(float screenX, float screenY)
{
    positionX = (screenX / 2.f) - 25.f;
    positionY = (screenY / 2.f) - 25.f;

    maxHealth = 100;
    health = maxHealth;

    Rectangle.setSize(sf::Vector2f(150.f, 100.f));
    Rectangle.setFillColor(sf::Color::Blue);
    Rectangle.setPosition({positionX, positionY});

    // Barrel setup to track aiming
    barrel.setSize({barrelLength, 14.f});
    barrel.setOrigin({0.f, 7.f});
    barrel.setFillColor(sf::Color(60, 60, 60));
}

bool Player::update(float screenX, float screenY, sf::Vector2f aimPoint)
{
    // Movement controls
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {
        if (positionX < screenX - 75.f)
            positionX += 5.f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    {
        if (positionX > -75.f)
            positionX -= 5.f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    {
        if (positionY > -50.f)
            positionY -= 5.f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    {
        if (positionY < screenY - 50.f)
            positionY += 5.f;
    }

    // Exit conditions (Escape or Q)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape) || 
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
    {
        return true;
    }

    // Reset player position and state
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
    {
        positionX = (screenX / 2.f) - 25.f;
        positionY = (screenY / 2.f) - 25.f;
        health = maxHealth;
        lastDamageTime = std::chrono::steady_clock::now();
    }

    Rectangle.setPosition({positionX, positionY});

    // Rotate barrel toward aim point
    sf::Vector2f center = getCenter();
    sf::Vector2f toAim = aimPoint - center;
    float angleDeg = std::atan2(toAim.y, toAim.x) * 180.f / 3.14159265f;

    barrel.setPosition(center);
    barrel.setRotation(sf::degrees(angleDeg));

    return false;
}

void Player::draw(sf::RenderWindow& window)
{
    window.draw(Rectangle);
    window.draw(barrel);
}

sf::Vector2f Player::getPosition()
{
    return {positionX, positionY};
}

sf::Vector2f Player::getCenter()
{
    return {positionX + 75.f, positionY + 50.f};
}

sf::Vector2f Player::getBarrelTip()
{
    float rad = barrel.getRotation().asRadians();
    return getCenter() + sf::Vector2f(std::cos(rad), std::sin(rad)) * barrelLength;
}

void Player::takeDamage(int amount)
{
    const auto now = std::chrono::steady_clock::now();
    if (now - lastDamageTime < std::chrono::milliseconds(invulnerabilityMs))
    {
        return;
    }

    lastDamageTime = now;
    health -= amount;
    if (health < 0) health = 0;
}

int Player::getHealth() const
{
    return health;
}

bool Player::isAlive() const
{
    return health > 0;
}

bool Player::isInvulnerable() const
{
    const auto now = std::chrono::steady_clock::now();
    return now - lastDamageTime < std::chrono::milliseconds(invulnerabilityMs);
}