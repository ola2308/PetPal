#pragma once
#include <SFML/Graphics.hpp>

class HealthBarManager;

//Klasa abstrakcyjna
class RoomStrategy {
public:
    virtual ~RoomStrategy() = default;//wirtualny destruktor
    virtual void handleInteraction(sf::Event event, sf::RenderWindow& window, sf::Sprite& puppySprite, HealthBarManager& healthBarManager) = 0;
    virtual void draw(sf::RenderWindow& window, sf::Sprite& puppySprite) = 0;
    virtual void update(float deltaTime, HealthBarManager& healthBarManager) = 0;
};