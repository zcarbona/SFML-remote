#include "../include/logic.hpp"
#include "../include/Player.hpp"
#include "../include/bullet.hpp"

void Logic::run(float screenX, float screenY)
{
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 0;

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

        // Update
        if (player.update(screenX, screenY))
        {
            window.close();
        }

        for (auto it = bullets.begin(); it != bullets.end();)
        {
            if(it->update()){
                it = bullets.erase(it);
            }
            else{it++;}
        }

        // Draw
        window.clear(sf::Color::Black);

        player.draw(window);

        for (auto& bullet : bullets)
        {
            bullet.draw(window);
        }

        window.display();
    }
}