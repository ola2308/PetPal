#pragma once
#define NOMINMAX
#include <cmath>
#define _WIN32_WINNT 0x0601
#include <Windows.h>
#include <vector>
#include <string>
#include <memory> //pamięć dynamiczna (shared_ptr - RoomStrategy)
#include <fstream>
#include <iostream>
#include <cstdio>//remove()
#include <sstream>//istringstream
#include "PuppyBuilder.h"
#include "HealthBarManager.h"
#include "RoomInteraction.h"
#include "PlayRoomStrategy.h"
#include "Interface.h"

void createmyWindow(sf::RenderWindow& W, sf::String s);
void loadTexture(sf::Texture& texture, const std::string& filename);
bool isFileEmpty(const std::string& filename);
void runGame();

class HealthBarManager;

class Game {
public:
    Game(sf::RenderWindow& window, int selectedPuppy);
    ~Game();
    void run();
    void handleInput();
    void updateHealth(float deltaTime);
    void draw();
    void setPuppyName(const std::string& name);
    void displayWelcomeMessage();
    void saveGameState();
    void loadGameState();

private:
    sf::RenderWindow& window;
    sf::Sprite puppySprite;
    HealthBarManager healthBarManager;
    RoomInteraction roomInteraction;
    std::unique_ptr<BedroomStrategy> bedroomStrategy;

    std::vector<std::string> roomPaths = { "reszta/salon.jpg", "reszta/kuchnia.jpg", "reszta/lazienka.jpg", "reszta/bawialnia.jpg" };
    std::vector<sf::Texture> roomTextures;
    sf::Sprite roomSprite;
    int currentRoomIndex = 0;

    int selectedPuppyIndex;

    Puppy* selectedPuppy;
    std::vector<PuppyBuilder*> builders;

    sf::Font font;
    sf::Text sleepMessage;
    sf::Text welcomeMessage;
    bool showSleepMessage;
    bool showWelcomeMessage;
    float messageTimer;

    std::string puppyName;

    sf::Texture exitTexture;
    sf::Sprite exitSprite;
    sf::Texture resetTexture;
    sf::Sprite resetSprite;

    void loadRoomTextures();
    void changeRoom(int index);
    void displaySleepMessage();
    void handleMouseClick(sf::Vector2i mousePos);
    void loadButtons();

    sf::Texture shelterTexture;
    sf::Sprite shelterSprite;
    bool showShelterScreen;

    void handleShelterScreen();
};
