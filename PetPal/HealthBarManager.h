#pragma once
#include <map>
#include "HealthBar.h"
#include <vector>
#include <ranges> 

class RoomInteraction;

class EmotionTextureManager {
public:
    EmotionTextureManager();
    ~EmotionTextureManager();
    sf::Texture& getTexture(const std::string& emotionName);
    void loadTextures();

private:
    std::map<std::string, sf::Texture> textures;
};

class HealthBarManager {
public:
    HealthBarManager();
    void increaseSleep(float amount);
    void updateHealth(float deltaTime, sf::Sprite& puppySprite);
    void draw(sf::RenderWindow& window);
    bool isHealthCritical() const;
    float getCriticalHealthTime() const;
    float getHygieneLevel() const;
    EmotionTextureManager& getTextureManager();
    void increaseHygiene(float amount);
    void setRoomInteraction(RoomInteraction& interaction);
    RoomInteraction* getRoomInteraction();
    void increaseFood(float amount);
    void increaseHappiness(float amount);
    std::vector<HealthBar>& getBars();
    void setBars(const std::vector<float>& values);
    std::vector<HealthBar> getCriticalHealthBars() const;  // Dodaj tê metodê

private:
    RoomInteraction* roomInteraction = nullptr;
    EmotionTextureManager textureManager;
    std::vector<HealthBar> bars;
    float criticalHealthTime;
    bool wasHealthCritical;
};
