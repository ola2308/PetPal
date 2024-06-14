#pragma once
#include "KitchenStrategy.h"
#include "BathroomStrategy.h"
#include "BedroomStrategy.h"
#include <fstream>

class HealthBarManager;
class RoomStrategy;

class RoomInteraction {
public:
    RoomInteraction(sf::RenderWindow& win);
    void setHealthBarManager(HealthBarManager* manager);
    void setStrategy(RoomStrategy* strategy);
    void handleInteraction(int roomIndex, sf::Event event, sf::Sprite& puppySprite);
    void draw(sf::Sprite& puppySprite, int roomIndex, sf::RenderWindow& window);
    void drawBubbles(sf::RenderWindow& window, sf::Sprite& puppySprite);
    void drawStains(sf::RenderWindow& window);
    void drawPoops(sf::RenderWindow& window);
    std::vector<sf::CircleShape>& getStains();
    std::vector<sf::Sprite>& getPoops();
    void addPoop(sf::Sprite& puppySprite);
    void addStain(sf::Sprite& puppySprite, int transparency);
    void removePoop(sf::Vector2i mousePosition);
    int getCurrentRoomIndex() const;
    void setCurrentRoomIndex(int index);
    void openFridge();
    RoomStrategy* getCurrentStrategy() const;

    void saveState(const std::string& filename);
    void loadState(const std::string& filename);

private:
    sf::RenderWindow& window;
    HealthBarManager* healthBarManager = nullptr;
    std::unique_ptr<RoomStrategy> currentStrategy;
    sf::Texture poopTexture;
    std::vector<sf::CircleShape> stains;
    std::vector<sf::Sprite> poops;
    std::vector<sf::Sprite> bubbles;
    int currentRoomIndex = 0;
};
