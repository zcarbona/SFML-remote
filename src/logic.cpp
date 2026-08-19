    #include "../include/logic.hpp"
    #include "../include/Player.hpp"
    #include "../include/bullet.hpp"
    #include "../include/creatingOBJ.hpp"


    void Logic::run(float screenX, float screenY)
    {
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
        Object object(screenX,screenY);
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

            for(auto& bullet:bullets){
                if (std::abs(bullet.getPosition().x - object.getPosition().x) < 10.f &&
                    std::abs(bullet.getPosition().y - object.getPosition().y) < 10.f)
                {
                    object.setColor(sf::Color::Red);
                }
            }

            //the begining of the vector itirate unteill it reach the end
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
            object.draw(window);
            for (auto& bullet : bullets)
            {
                bullet.draw(window);
            }

            window.display();
        }//end while
    }