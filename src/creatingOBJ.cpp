#include "../include/creatingOBJ.hpp"

Object::Object(
    float screenX,
    float screenY,
    const sf::Font& font,
    int number
)
    : text(font)
{
    positionX = screenX / 2.f;
    positionY = screenY / 2.f;
    health = 100;
    Rectangle.setSize({100.f, 100.f});
    Rectangle.setPosition({positionX, positionY});
    Rectangle.setFillColor(sf::Color::Blue);

    text.setString(std::to_string(100));
    text.setCharacterSize(30);
    text.setFillColor(sf::Color::White);

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

void Object::setHealth(int amount){
    health += amount;
}

int Object::getHealth(){
    return health;
}