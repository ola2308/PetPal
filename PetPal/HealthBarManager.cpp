#include "HealthBarManager.h"
#include "Game.h"
#include <iostream>

EmotionTextureManager::EmotionTextureManager() {
    loadTextures();
}

EmotionTextureManager::~EmotionTextureManager() {}

void EmotionTextureManager::loadTextures() {
    std::vector<std::string> emotionNames = { "happiness", "sleep", "hygiene", "food" };
    for (const auto& name : emotionNames) {
        sf::Texture texture;
        if (!texture.loadFromFile(name + ".png")) {
            std::cerr << "Nie udalo sie zaladowac tekstury: " + name + ".png" << std::endl;
            continue;
        }
        textures[name] = std::move(texture);
    }
}

sf::Texture& EmotionTextureManager::getTexture(const std::string& emotionName) {
    return textures.at(emotionName);
}

HealthBarManager::HealthBarManager()
    : criticalHealthTime(0.0f), wasHealthCritical(false) {
    bars.emplace_back("sleep", textureManager.getTexture("sleep"), sf::Vector2f(10, 10));
    bars.emplace_back("hygiene", textureManager.getTexture("hygiene"), sf::Vector2f(10, 40));
    bars.emplace_back("happiness", textureManager.getTexture("happiness"), sf::Vector2f(10, 70));
    bars.emplace_back("food", textureManager.getTexture("food"), sf::Vector2f(10, 100));
}

void HealthBarManager::setRoomInteraction(RoomInteraction& interaction) {
    roomInteraction = &interaction;
}

//Zwraca wskaźnik na RoomInteraction, umożliwia interakcje z pokojami
RoomInteraction* HealthBarManager::getRoomInteraction() {
    return roomInteraction;
}

void HealthBarManager::updateHealth(float deltaTime, sf::Sprite& puppySprite) {
    bool isCurrentlyCritical = isHealthCritical();

    if (isCurrentlyCritical) {
        criticalHealthTime += wasHealthCritical ? deltaTime : 0.0f;
        wasHealthCritical = true;
    }
    else {
        wasHealthCritical = false;
        criticalHealthTime = 0.0f;
    }

    for (auto& bar : bars) {
        float previousValue = bar.getCurrentValue();
        bar.updateHealth(deltaTime, puppySprite);

        if (bar.getName() == "hygiene") {
            auto addStain = [&](float threshold, int stainType) {
                if (previousValue > threshold && bar.getCurrentValue() <= threshold) {
                    roomInteraction->addStain(puppySprite, stainType);
                }
                };
            addStain(84, 120);
            addStain(90, 120);
            addStain(95, 120);
            addStain(75, 186);
           // addStain(50, 170);
            //addStain(25, 200);
            //addStain(0, 200);

            auto addPoop = [&](float threshold) {
                if (previousValue > threshold && bar.getCurrentValue() <= threshold) {
                    roomInteraction->addPoop(puppySprite);
                }
                };

            addPoop(90);
            addPoop(65);
            addPoop(50);
            addPoop(40);
            addPoop(15);
        }
    }
}

std::vector<HealthBar> HealthBarManager::getCriticalHealthBars() const {
    auto criticalBars = bars
        | std::views::filter([](const HealthBar& bar) {
        return bar.getCurrentValue() / bar.getMaxValue() <= 0.25;
            });
    return std::vector<HealthBar>(criticalBars.begin(), criticalBars.end());
}

void HealthBarManager::draw(sf::RenderWindow& window) {
    for (auto& bar : bars) {
        bar.draw(window);
    }
}

bool HealthBarManager::isHealthCritical() const {
    return std::any_of(bars.begin(), bars.end(), [](const HealthBar& bar) {
        return bar.getCurrentValue() / bar.getMaxValue() <= 0.25;
        });
}

float HealthBarManager::getHygieneLevel() const {
    auto it = std::find_if(bars.begin(), bars.end(), [](const HealthBar& bar) {
        return bar.getName() == "hygiene";
        });
    return it != bars.end() ? it->getCurrentValue() : 100;
}

float HealthBarManager::getCriticalHealthTime() const {
    return criticalHealthTime;
}

EmotionTextureManager& HealthBarManager::getTextureManager() {
    return textureManager;
}

void HealthBarManager::increaseHygiene(float amount) {
    auto it = std::find_if(bars.begin(), bars.end(), [](const HealthBar& bar) {
        return bar.getName() == "hygiene";
        });
    if (it != bars.end()) {
        float newValue = std::min(it->getCurrentValue() + amount, it->getMaxValue());
        it->setCurrentValue(newValue);
        std::cout << "Aktualny procent higieny: " << it->getCurrentValue();
    }
}

void HealthBarManager::increaseFood(float amount) {
    auto it = std::find_if(bars.begin(), bars.end(), [](const HealthBar& bar) {
        return bar.getName() == "food";
        });
    if (it != bars.end()) {
        float newValue = std::min(it->getCurrentValue() + amount, it->getMaxValue());
        it->setCurrentValue(newValue);
        std::cout << "Aktualny procent jedzenia: " << it->getCurrentValue() << std::endl;
    }
}

void HealthBarManager::increaseSleep(float amount) {
    auto it = std::find_if(bars.begin(), bars.end(), [](const HealthBar& bar) {
        return bar.getName() == "sleep";
        });
    if (it != bars.end()) {
        float newValue = std::min(it->getCurrentValue() + amount, it->getMaxValue());
        it->setCurrentValue(newValue);
        std::cout << "Aktualny procent snu: " << it->getCurrentValue() << std::endl;
    }
}

void HealthBarManager::increaseHappiness(float amount) {
    auto it = std::find_if(bars.begin(), bars.end(), [](const HealthBar& bar) {
        return bar.getName() == "happiness";
        });
    if (it != bars.end()) {
        float newValue = std::min(it->getCurrentValue() + amount, it->getMaxValue());
        it->setCurrentValue(newValue);
        std::cout << "Aktualny procent szczescia: " << it->getCurrentValue() << std::endl;
    }
}

std::vector<HealthBar>& HealthBarManager::getBars() {
    return bars;
}

void HealthBarManager::setBars(const std::vector<float>& values) {
    for (size_t i = 0; i < bars.size() && i < values.size(); ++i) {
        bars[i].setCurrentValue(values[i]);
    }
}
