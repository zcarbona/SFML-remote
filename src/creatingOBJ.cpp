#include "../include/creatingOBJ.hpp"

Object::Object(float screenX, float screenY)
{
    positionX = screenX / 2.f;
    positionY = screenY / 2.f;

    Rectangle.setSize({100.f, 100.f});
    Rectangle.setPosition({positionX, positionY});
    Rectangle.setFillColor(sf::Color::Blue);
}

void Object::draw(sf::RenderWindow& window){
    window.draw(Rectangle);
}

sf::Vector2f Object::getPosition(){
    return Rectangle.getPosition();
}


void Object::setColor(sf::Color color){
    Rectangle.setFillColor(color);
}