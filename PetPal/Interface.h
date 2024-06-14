#pragma once
#include "Game.h"

class Menu {
public:
    Menu(float width, float height, const std::string& backgroundPath);
    ~Menu();

    // Metody do zarzadzania menu
    virtual void draw(sf::RenderWindow& window);
    void moveUp();
    void moveDown();
    int getPressedItem() const { return selectedItemIndex; }
    void choice(sf::RenderWindow& window, const sf::Event& event);
    bool getShowPuppies() const { return showPuppies; }
    void setShowPuppies(bool show);
    sf::Font font;
    sf::Sprite bordiSprite, maltiSprite, goldiSprite;
    PuppyBuilder* bordiBuilder;
    PuppyBuilder* maltiBuilder;
    PuppyBuilder* goldiBuilder;

private:
    int pies;
    int selectedItemIndex;
    std::vector<sf::Text> menuItems;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    bool showPuppies;
    sf::Text title;
    sf::Text bordiLabel, maltiLabel, goldiLabel;
};