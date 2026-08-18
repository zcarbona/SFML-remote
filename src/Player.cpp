#include "../include/Player.hpp"

Player::Player(float screenX, float screenY)
{
    positionX = (screenX / 2.f) - 25.f;
    positionY = (screenY / 2.f) - 25.f;

    Rectangle.setSize(sf::Vector2f(150.f,100.f));
    Rectangle.setFillColor(sf::Color::Blue);

    Rectangle.setPosition({
        positionX,
        positionY
    });
}

bool Player::update(float screenX, float screenY)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {
        if (positionX < screenX - 75.f)
            positionX += 0.1f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    {
        if (positionX > -75.f)
            positionX -= 0.1f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    {
        if (positionY > -50.f)
            positionY -= 0.1f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    {
        if (positionY < screenY - 50.f)
            positionY += 0.1f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
    {
        return true;
    }
    if((bool)sf::Mouse::Button::Left){
        return true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
    {
        positionX = (screenX / 2.f) - 25.f;
        positionY = (screenY / 2.f) - 25.f;
    }


    
    Rectangle.setPosition({positionX, positionY});
    return false;
    
    Rectangle.setPosition({
        positionX,
        positionY
    });

}

void Player::draw(sf::RenderWindow& window)
{
    window.draw(Rectangle);
    
}

sf::Vector2f Player::getPosition(){
    return{positionX,positionY};
}