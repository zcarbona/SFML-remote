#include "../include/logic.hpp"
#include "../include/Player.hpp"
#include "../include/bullet.hpp"
#include "../include/creatingOBJ.hpp"

#include <cmath>

void Logic::run(float screenX, float screenY)
{
    sf::Font font;

    if (!font.openFromFile(
        "C:\\Users\\Ali Mohamed\\Desktop\\SFML\\assets\\font\\GameFont.TTF"))
    {
        return;
    }

    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;

    sf::RenderWindow window(
        sf::VideoMode({
            static_cast<unsigned>(screenX),
            static_cast<unsigned>(screenY)
        }),
        "Testing Graphics SFML API",
        sf::State::Windowed,
        settings
    );

    std::vector<Bullet> bullets;
    std::vector<Object> objects;

    Player player(screenX, screenY);

    objects.emplace_back(
        screenX / 2.f - 50.f,
        screenY / 2.f - 50.f,
        font,
        100
    );

    objects.emplace_back(
        200.f,
        200.f,
        font,
        100
    );

    objects.emplace_back(
        800.f,
        400.f,
        font,
        100
    );

    while (window.isOpen())
    {
        // =========================
        // Input
        // =========================

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (const auto* mouse =
                event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouse->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2f mousePos =
                        window.mapPixelToCoords(mouse->position);

                    bullets.emplace_back(
                        player.getPosition(),
                        mousePos
                    );
                }
            }
        }

        // =========================
        // Update Player
        // =========================

        if (player.update(screenX, screenY))
        {
            window.close();
        }

        // =========================
        // Update Bullets
        // =========================

        for (auto it = bullets.begin(); it != bullets.end();)
        {
            if (it->update())
            {
                it = bullets.erase(it);
            }
            else
            {
                ++it;
            }
        }

        // =========================
        // Update Objects
        // =========================

        for (auto& object : objects)
        {
            object.update(screenX, screenY);
        }

        // =========================
        // Collision
        // =========================

        for (auto bulletIt = bullets.begin();
             bulletIt != bullets.end();)
        {
            bool bulletHit = false;

            for (auto objectIt = objects.begin();
                 objectIt != objects.end();)
            {
                float objectCenterX =
                    objectIt->getPosition().x + 50.f;

                float objectCenterY =
                    objectIt->getPosition().y + 50.f;

                if (
                    std::abs(
                        bulletIt->getPosition().x - objectCenterX
                    ) < 50.f
                    &&
                    std::abs(
                        bulletIt->getPosition().y - objectCenterY
                    ) < 50.f
                )
                {
                    // Damage object
                    objectIt->setHealth(-25);

                    // Remove object if dead
                    if (objectIt->getHealth() <= 0)
                    {
                        objectIt = objects.erase(objectIt);
                    }
                    else
                    {
                        ++objectIt;
                    }

                    // Bullet can only hit one object
                    bulletHit = true;

                    break;
                }

                ++objectIt;
            }

            // Remove bullet after collision
            if (bulletHit)
            {
                bulletIt = bullets.erase(bulletIt);
            }
            else
            {
                ++bulletIt;
            }
        }

        // =========================
        // Draw
        // =========================

        window.clear(sf::Color::Black);

        player.draw(window);

        for (auto& object : objects)
        {
            object.draw(window);
        }

        for (auto& bullet : bullets)
        {
            bullet.draw(window);
        }

        window.display();
    }
}