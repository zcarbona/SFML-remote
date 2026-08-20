#include "../include/logic.hpp"
#include "../include/Player.hpp"
#include "../include/bullet.hpp"
#include "../include/creatingOBJ.hpp"
#include <string>
#include <cmath>

void Logic::run(float screenX, float screenY)
{
    sf::Font font;

    if (!font.openFromFile("C:\\Users\\Ali Mohamed\\Desktop\\SFML\\assets\\font\\GameFont.TTF"))
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

    Player player(screenX, screenY);

    std::vector<Object> objects;

    objects.emplace_back(screenX, screenY, font, 1);
    objects.emplace_back(screenX - 150.f, screenY + 500.f, font, 2);
    objects.emplace_back(screenX + 200.f, screenY + 300.f, font, 3);

    while (window.isOpen())
    {
        // Input
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>())
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

        // Update player
        if (player.update(screenX, screenY))
        {
            window.close();
        }

        // Update bullets
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

        // Collision
        for (auto bulletIt = bullets.begin(); bulletIt != bullets.end();)
        {
            bool bulletHit = false;

            for (auto objectIt = objects.begin(); objectIt != objects.end();)
            {
                if (
                    std::abs(
                        bulletIt->getPosition().x -
                        (objectIt->getPosition().x + 50.f)
                    ) < 50.f
                    &&
                    std::abs(
                        bulletIt->getPosition().y -
                        (objectIt->getPosition().y + 50.f)
                    ) < 50.f
                )
                {
                    // Damage the object that was actually hit
                    objectIt->setHealth(-25);

                    // Remove object if health reaches zero
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
                else
                {
                    ++objectIt;
                }
            }

            // Remove bullet after hitting an object
            if (bulletHit)
            {
                bulletIt = bullets.erase(bulletIt);
            }
            else
            {
                ++bulletIt;
            }
        }

        // Draw
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