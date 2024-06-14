#pragma once
#include <vector>
#include <algorithm>
#include "RoomStrategy.h"
#include "HealthBarManager.h"
#include "RoomInteraction.h"

class HealthBarManager;
class RoomInteraction;

class BathroomStrategy : public RoomStrategy {
public:
    BathroomStrategy(sf::RenderWindow& window, RoomInteraction& roomInteraction);
    void handleInteraction(sf::Event event, sf::RenderWindow& window, sf::Sprite& puppySprite, HealthBarManager& healthBarManager) override;
    void draw(sf::RenderWindow& window, sf::Sprite& puppySprite) override;
    void update(float deltaTime, HealthBarManager& healthBarManager) override;

private:
    sf::RenderWindow& window;
    RoomInteraction& roomInteraction;
    sf::Texture soapTexture;
    sf::Sprite soapSprite;
    sf::Texture bubbleTexture;
    sf::Sprite bubbleSprite;
    sf::Vector2f originalSoapPosition;
    bool isSoapGrabbed = false;
    std::vector<sf::Sprite> bubbles;
    sf::Clock bubbleTimer;

    void checkCollisionWithPuppy(sf::Sprite& puppySprite, HealthBarManager& healthBarManager);
    void addBubble(sf::Vector2f position);
    void removeStain(sf::RenderWindow& window, sf::Sprite& puppySprite, HealthBarManager& healthBarManager);
};
