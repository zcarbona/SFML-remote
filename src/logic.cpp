#include "../include/logic.hpp"

#include "../include/Player.hpp"
#include "../include/bullet.hpp"
#include "../include/creatingOBJ.hpp"

#include <cmath>
#include <chrono>
#include <random>
#include <algorithm>

void Logic::run(float screenX, float screenY)
{
    sf::Font font;

    // Load the bundled font from the game's assets folder.
    if (!font.openFromFile("assets/font/GameFont.TTF"))
    {
        std::cerr << "Failed to load font: assets/font/GameFont.TTF"
                  << std::endl;
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

    window.setFramerateLimit(60);

    std::vector<Bullet> bullets;
    std::vector<Object> objects;

    Player player(screenX, screenY);

    int level = 1;

    auto levelStart = std::chrono::steady_clock::now();

    std::random_device rd;
    std::mt19937 spawnGen(rd());

    std::uniform_real_distribution<float> spawnX(
        0.f,
        screenX - 100.f
    );

    std::uniform_real_distribution<float> spawnY(
        0.f,
        screenY - 100.f
    );

    auto spawnInitialWave = [&]()
    {
        objects.clear();

        objects.emplace_back(
            screenX / 2.f - 50.f,
            screenY / 2.f - 50.f,
            font,
            level
        );

        objects.emplace_back(
            200.f,
            200.f,
            font,
            level
        );

        objects.emplace_back(
            800.f,
            400.f,
            font,
            level
        );
    };

    spawnInitialWave();

    sf::Vector2f mouseWorldPos = player.getCenter();

    sf::Text hud(font);

    hud.setCharacterSize(24);
    hud.setFillColor(sf::Color::White);
    hud.setPosition({10.f, 10.f});

    bool gameOver = false;

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
                if (
                    mouse->button == sf::Mouse::Button::Left &&
                    !gameOver
                )
                {
                    sf::Vector2f clickPos =
                        window.mapPixelToCoords(mouse->position);

                    bullets.emplace_back(
                        player.getBarrelTip(),
                        clickPos
                    );
                }
            }
        }

        mouseWorldPos =
            window.mapPixelToCoords(
                sf::Mouse::getPosition(window)
            );

        // =========================
        // Game Over / Restart
        // =========================

        if (gameOver)
        {
            if (sf::Keyboard::isKeyPressed(
                    sf::Keyboard::Key::R))
            {
                player = Player(screenX, screenY);

                bullets.clear();

                level = 1;

                levelStart =
                    std::chrono::steady_clock::now();

                spawnInitialWave();

                gameOver = false;
            }
        }
        else
        {
            // =========================
            // Update Player
            // =========================

            if (player.update(
                    screenX,
                    screenY,
                    mouseWorldPos))
            {
                window.close();
            }

            if (!player.isAlive())
            {
                gameOver = true;
            }

            // =========================
            // Leveling
            // =========================

            int levelUpSeconds =
                std::max(5, 15 - level);

            auto elapsedLevel =
                std::chrono::duration_cast<
                    std::chrono::seconds
                >(
                    std::chrono::steady_clock::now() -
                    levelStart
                ).count();

            if (elapsedLevel >= levelUpSeconds)
            {
                level++;

                levelStart =
                    std::chrono::steady_clock::now();

                objects.emplace_back(
                    spawnX(spawnGen),
                    spawnY(spawnGen),
                    font,
                    level
                );
            }

            // =========================
            // Update Bullets
            // =========================

            for (auto it = bullets.begin();
                 it != bullets.end();)
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

            sf::Vector2f playerCenter =
                player.getCenter();

            for (auto& object : objects)
            {
                object.update(
                    screenX,
                    screenY,
                    playerCenter,
                    level
                );
            }

            // =========================
            // Bullet vs Enemy Collision
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
                            bulletIt->getPosition().x -
                            objectCenterX
                        ) < 50.f
                        &&
                        std::abs(
                            bulletIt->getPosition().y -
                            objectCenterY
                        ) < 50.f
                    )
                    {
                        objectIt->setHealth(-25);

                        if (objectIt->getHealth() <= 0)
                        {
                            objectIt =
                                objects.erase(objectIt);
                        }
                        else
                        {
                            ++objectIt;
                        }

                        bulletHit = true;

                        break;
                    }

                    ++objectIt;
                }

                if (bulletHit)
                {
                    bulletIt =
                        bullets.erase(bulletIt);
                }
                else
                {
                    ++bulletIt;
                }
            }

            // =========================
            // Enemy vs Player Collision
            // Only a charging run hurts you
            // =========================

            for (auto& object : objects)
            {
                float objectCenterX =
                    object.getPosition().x + 50.f;

                float objectCenterY =
                    object.getPosition().y + 50.f;

                if (
                    object.isCharging()
                    &&
                    std::abs(
                        playerCenter.x -
                        objectCenterX
                    ) < 125.f
                    &&
                    std::abs(
                        playerCenter.y -
                        objectCenterY
                    ) < 100.f
                )
                {
                    player.takeDamage(
                        object.getDamage(level)
                    );

                    object.forceRetreat();
                }
            }
        }

        // =========================
        // HUD
        // =========================

        hud.setString(
            "Level: " +
            std::to_string(level) +
            "   Health: " +
            std::to_string(player.getHealth()) +
            (
                gameOver
                    ? "   -- GAME OVER -- press R to restart"
                    : ""
            )
        );

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

        window.draw(hud);

        window.display();
    }
}