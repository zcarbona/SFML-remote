#include "../include/bullet.hpp"

#include <cmath>

Bullet::Bullet(sf::Vector2f startPos, sf::Vector2f targetPos)
{
    position = startPos;
    target = targetPos;

    shape.setRadius(5.f);
    shape.setFillColor(sf::Color::White);
    shape.setPosition(position);
}

bool Bullet::update()
{
    sf::Vector2f direction = target - position;

    float distance =
        std::sqrt(
            direction.x * direction.x +
            direction.y * direction.y
        );

    constexpr float speed = 12.f;

    // Delete the bullet when it reaches or is about to reach its target.
    // This prevents it from overshooting and bouncing back and forth.
    if (distance <= speed)
    {
        return true;
    }

    direction /= distance;

    position += direction * speed;

    shape.setPosition(position);

    return false;
}

void Bullet::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}

sf::Vector2f Bullet::getPosition()
{
    // Return the center of the bullet instead of its top-left position.
    return shape.getPosition() + sf::Vector2f(5.f, 5.f);
}