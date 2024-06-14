#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <algorithm>

class HealthBar {
public:
    HealthBar(const std::string& name, sf::Texture& texture, const sf::Vector2f& position);
    void updateHealth(float deltaTime, sf::Sprite& puppySprite);
    void draw(sf::RenderWindow& window) const;

    float getCurrentValue() const;
    float getMaxValue() const;
    void setCurrentValue(float x);
    std::string getName() const;  // Metoda zwracająca nazwe paska zdrowia

private:
    std::string name;  // Nazwa paska zdrowia
    sf::Texture& iconTexture;
    sf::Sprite iconSprite;
    sf::RectangleShape barBackground;
    sf::RectangleShape barForeground;
    float currentValue;
    float maxValue;
    float decreaseRate;
};
