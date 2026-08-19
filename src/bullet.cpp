#include "..\include\bullet.hpp"
#include <cmath>

Bullet::Bullet(sf::Vector2f startPos, sf::Vector2f targetPos){
    position = startPos;
    target = targetPos;

    shape.setRadius(5.f);
    shape.setFillColor(sf::Color::White);

    shape.setPosition(position);
}


bool Bullet::update(){
    sf::Vector2f direction = target - position;

    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance < 2.f){
        return true;
    }

    direction /= distance;

    position += direction * 0.1f;
    //shape.setRotation(sf::degrees(10.f));
    shape.setPosition(position);
    return false;
}


void Bullet::draw(sf::RenderWindow& window){
    window.draw(shape);
}

sf::Vector2f Bullet::getPosition(){
    return shape.getPosition();
}