#pragma once
#include "RoomStrategy.h"
#include "HealthBarManager.h"
#include "Puppy.h"

class BedroomStrategy : public RoomStrategy {
public:
    BedroomStrategy(sf::RenderWindow& window, Puppy* puppy);
    void handleInteraction(sf::Event event, sf::RenderWindow& window, sf::Sprite& puppySprite, HealthBarManager& healthBarManager) override;
    void draw(sf::RenderWindow& window, sf::Sprite& puppySprite) override;
    void update(float deltaTime, HealthBarManager& healthBarManager) override;

    bool isNight;

private:
    sf::Texture nightTexture;
    sf::Sprite nightSprite;
    sf::RenderWindow& window;
    float sleepAccumulator;
    Puppy* puppy; 

    void drawPuppy(sf::RenderWindow& window, sf::Sprite& puppySprite);
};

