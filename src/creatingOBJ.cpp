#include "../include/creatingOBJ.hpp"

#include <random>
#include <cmath>

Object::Object(
    float screenX,
    float screenY,
    const sf::Font& font,
    int number
)
    : text(font)
{
    positionX = screenX;
    positionY = screenY;

    health = 100;

    Rectangle.setSize({100.f, 100.f});

    Rectangle.setPosition({
        positionX,
        positionY
    });

    Rectangle.setFillColor(sf::Color::Blue);

    text.setString(std::to_string(number));
    text.setCharacterSize(30);
    text.setFillColor(sf::Color::White);

    text.setPosition({
        positionX + 35.f,
        positionY + 30.f
    });

    // Random starting direction
    std::random_device rd;
    std::mt19937 generator(rd());

    std::uniform_real_distribution<float> angleDistribution(
        0.f,
        2.f * 3.14159265f
    );

    float angle = angleDistribution(generator);

    velocity.x = std::cos(angle) * 0.5f;
    velocity.y = std::sin(angle) * 0.5f;

    lastDirectionChange =
        std::chrono::steady_clock::now();
}

void Object::update(float screenX, float screenY)
{
    auto now = std::chrono::steady_clock::now();

    auto elapsed =
        std::chrono::duration_cast<std::chrono::seconds>(
            now - lastDirectionChange
        );

    // Change direction every 5 seconds
    if (elapsed.count() >= 5)
    {
        std::random_device rd;
        std::mt19937 generator(rd());

        std::uniform_real_distribution<float> angleDistribution(
            0.f,
            .2f * 3.14159265f
        );

        float angle = angleDistribution(generator);

        velocity.x = std::cos(angle) * 0.5f;
        velocity.y = std::sin(angle) * 0.5f;

        lastDirectionChange =
            std::chrono::steady_clock::now();
    }

    // Move
    positionX += velocity.x;
    positionY += velocity.y;

    // Left wall
    if (positionX <= 0.f)
    {
        positionX = 0.f;
        velocity.x *= -.5f;
    }

    // Right wall
    if (positionX >= screenX - 100.f)
    {
        positionX = screenX - 100.f;
        velocity.x *= -.5f;
    }

    // Top wall
    if (positionY <= 0.f)
    {
        positionY = 0.f;
        velocity.y *= -.5f;
    }

    // Bottom wall
    if (positionY >= screenY - 100.f)
    {
        positionY = screenY - 100.f;
        velocity.y *= -.5f;
    }

    Rectangle.setPosition({
        positionX,
        positionY
    });

    text.setPosition({
        positionX + 35.f,
        positionY + 30.f
    });
}

void Object::draw(sf::RenderWindow& window)
{
    window.draw(Rectangle);
    window.draw(text);
}

sf::Vector2f Object::getPosition()
{
    return Rectangle.getPosition();
}

void Object::setColor(sf::Color color)
{
    Rectangle.setFillColor(color);
}

void Object::setHealth(int amount)
{
    health += amount;
    text.setString(std::to_string(health));
}

int Object::getHealth()
{
    return health;
}