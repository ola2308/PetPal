#pragma once
#include "Game.h"
#include <regex>

class PuppyNamingMenu : public Menu {
public:
    // Constructor
    PuppyNamingMenu(sf::RenderWindow& window, float width, float height, const std::string& backgroundPath, int selectedPuppy);
    void draw(sf::RenderWindow& window) override;
    void handleInput(const sf::Event& event, sf::RenderWindow& window);
    void activate(sf::RenderWindow& window);

private:
    sf::RenderWindow& window;
    sf::Text nameInputPrompt;
    sf::Text puppyNameDisplay;
    sf::RectangleShape textBox;
    std::string puppyName;
    std::regex namePattern;
    int selectedPuppy; // ID wybranego psa
    sf::Sprite selectedPuppySprite; // Sprite dla wybranego psa
};