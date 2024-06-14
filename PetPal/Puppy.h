#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Puppy {
public:
    Puppy() : isHappy(true), isSleeping(false) {};
    sf::Sprite sprite;
    sf::Texture textureHappy;
    sf::Texture textureSad;
    sf::Texture textureSleep;
    bool isHappy;
    bool isSleeping;  // Pole do śledzenia stanu snu
    void updateTexture(bool isHappy);
    void setHappyTexture(const std::string& path);
    void setSadTexture(const std::string& path);
    void setSleepTexture(const std::string& path); 
    void display(sf::RenderWindow& window);
};