#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include<iostream>
#include "RoomStrategy.h"
#include "HealthBarManager.h"

class PlayRoomStrategy : public RoomStrategy {
public:
    PlayRoomStrategy(sf::RenderWindow& window);
    ~PlayRoomStrategy();
    void handleInteraction(sf::Event event, sf::RenderWindow& window, sf::Sprite& puppySprite, HealthBarManager& healthBarManager) override;
    void draw(sf::RenderWindow& window, sf::Sprite& puppySprite) override;
    void update(float deltaTime, HealthBarManager& healthBarManager) override;

private:
    sf::Texture ballTexture;
    sf::Sprite ballSprite;
    bool isDragging;
    sf::Vector2f dragOffset;
    std::thread throwThread;
    std::atomic<bool> running;
    sf::Vector2f initialPosition;
    sf::Vector2f initialVelocity;
    std::atomic<int> bounceCount;
    std::mutex ballMutex;
    std::condition_variable cv;

    void throwBall(sf::RenderWindow& window, HealthBarManager& healthBarManager);
};
