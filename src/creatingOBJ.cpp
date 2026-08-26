#include "../include/creatingOBJ.hpp"

#include <random>
#include <cmath>
#include <algorithm>

namespace
{
    std::mt19937& rng()
    {
        static std::random_device rd;
        static std::mt19937 generator(rd());
        return generator;
    }

    float randomAngle()
    {
        std::uniform_real_distribution<float> dist(0.f, 2.f * 3.14159265f);
        return dist(rng());
    }

    float randomChance()
    {
        std::uniform_real_distribution<float> dist(0.f, 1.f);
        return dist(rng());
    }
}

Object::Object(
    float screenX,
    float screenY,
    const sf::Font& font,
    int level
)
    : text(font)
{
    positionX = screenX;
    positionY = screenY;

    // More powerful the further in you are
    maxHealth = 100 + level * 20;
    health = maxHealth;

    state = ObjectState::Wander;

    Rectangle.setSize({100.f, 100.f});

    Rectangle.setPosition({
        positionX,
        positionY
    });

    Rectangle.setFillColor(sf::Color::Blue);

    text.setString(std::to_string(health));
    text.setCharacterSize(30);
    text.setFillColor(sf::Color::White);

    text.setPosition({
        positionX + 35.f,
        positionY + 30.f
    });

    // Random starting direction (full circle, not the old narrow 0-0.2*pi slice)
    float angle = randomAngle();
    float wanderSpeed = 0.5f + level * 0.05f;

    velocity.x = std::cos(angle) * wanderSpeed;
    velocity.y = std::sin(angle) * wanderSpeed;

    lastDirectionChange = std::chrono::steady_clock::now();
}

void Object::update(float screenX, float screenY, sf::Vector2f playerCenter, int level)
{
    auto now = std::chrono::steady_clock::now();
    sf::Vector2f center = {positionX + 50.f, positionY + 50.f};

    if (state == ObjectState::Charging)
    {
        auto chargeElapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(
            now - chargeStartTime
        ).count();

        // End of the run - peel off like a plane breaking off a game of chicken
        if (chargeElapsedMs >= 1200)
        {
            state = ObjectState::Retreating;
            lastDirectionChange = now;

            sf::Vector2f away = center - playerCenter;
            float dist = std::sqrt(away.x * away.x + away.y * away.y);
            if (dist > 0.f) away /= dist;

            float retreatSpeed = 0.6f + level * 0.05f;
            velocity = away * retreatSpeed;
        }
    }
    else if (state == ObjectState::Retreating)
    {
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
            now - lastDirectionChange
        ).count();

        if (elapsed >= 2)
        {
            state = ObjectState::Wander;
            lastDirectionChange = now;

            float angle = randomAngle();
            float wanderSpeed = 0.5f + level * 0.05f;
            velocity.x = std::cos(angle) * wanderSpeed;
            velocity.y = std::sin(angle) * wanderSpeed;
        }
    }
    else // Wander
    {
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
            now - lastDirectionChange
        ).count();

        // Every few seconds, re-roll: keep wandering, or lock on and charge
        if (elapsed >= 4)
        {
            float chargeChance = std::min(0.85f, 0.15f + level * 0.12f);

            if (randomChance() < chargeChance)
            {
                state = ObjectState::Charging;
                chargeStartTime = now;

                sf::Vector2f toPlayer = playerCenter - center;
                float dist = std::sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);
                if (dist > 0.f) toPlayer /= dist;

                float chargeSpeed = 1.2f + level * 0.35f;
                velocity = toPlayer * chargeSpeed;
            }
            else
            {
                float angle = randomAngle();
                float wanderSpeed = 0.5f + level * 0.05f;
                velocity.x = std::cos(angle) * wanderSpeed;
                velocity.y = std::sin(angle) * wanderSpeed;
            }

            lastDirectionChange = now;
        }
    }

    // Move
    positionX += velocity.x;
    positionY += velocity.y;

    // Walls
    if (positionX <= 0.f) { positionX = 0.f; velocity.x *= -0.5f; }
    if (positionX >= screenX - 100.f) { positionX = screenX - 100.f; velocity.x *= -0.5f; }
    if (positionY <= 0.f) { positionY = 0.f; velocity.y *= -0.5f; }
    if (positionY >= screenY - 100.f) { positionY = screenY - 100.f; velocity.y *= -0.5f; }

    Rectangle.setPosition({positionX, positionY});
    text.setPosition({positionX + 35.f, positionY + 30.f});

    // Tint red mid-charge so an incoming run is readable
    Rectangle.setFillColor(state == ObjectState::Charging ? sf::Color(220, 40, 40) : sf::Color::Blue);
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

bool Object::isCharging() const
{
    return state == ObjectState::Charging;
}

int Object::getDamage(int level) const
{
    return 10 + level * 5;
}

void Object::forceRetreat()
{
    state = ObjectState::Retreating;
    lastDirectionChange = std::chrono::steady_clock::now();
}
