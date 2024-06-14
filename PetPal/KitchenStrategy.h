#pragma once
#include "RoomStrategy.h"
#include "HealthBarManager.h"
#include <vector>
#include <filesystem>
#include<iostream>

class KitchenStrategy : public RoomStrategy {
public:
    KitchenStrategy(sf::RenderWindow& window);
    void handleInteraction(sf::Event event, sf::RenderWindow& window, sf::Sprite& puppySprite, HealthBarManager& healthBarManager) override;
    void draw(sf::RenderWindow& window, sf::Sprite& puppySprite) override;
    void update(float deltaTime, HealthBarManager& healthBarManager) override;
    std::vector<sf::Sprite> foodSprites;

private:
    sf::RenderWindow& window;
    sf::RectangleShape fridgeRect;
    sf::Texture fridgeTexture;
    sf::Sprite fridgeSprite;
    std::vector<std::shared_ptr<sf::Texture>> foodTextures;
    bool isFridgeOpen = false;
    bool isFoodHeld = false;
    sf::Sprite heldFood;

    void openFridge();
    void closeFridge();
    void drawFood(sf::RenderWindow& window);
    void handleFoodInteraction(sf::Event event, sf::RenderWindow& window, sf::Sprite& puppySprite, HealthBarManager& healthBarManager);
    void handleFoodRelease(sf::Event event, sf::RenderWindow& window, sf::Sprite& puppySprite, HealthBarManager& healthBarManager);
    void initializeFoodSprites(); 
};
