#include "Interface.h"
#include"PuppyNamingMenu.h"

// Konstruktor inicjalizuje komponenty menu oraz ³aduje tekstury psów
Menu::Menu(float width, float height, const std::string& backgroundPath)
    : selectedItemIndex(0), bordiBuilder(new BordiBuilder()), maltiBuilder(new MaltiBuilder()), goldiBuilder(new GoldiBuilder()) {
    // £adowanie czcionki
    if (!font.loadFromFile("OpenSans-ExtraBoldItalic.ttf")) {
        std::cerr << "Nie udalo sie zaladowac czcionki" << std::endl;
    }

    // £adowanie t³a
    if (!backgroundTexture.loadFromFile(backgroundPath)) {
        std::cerr << "Nie udalo sie zaladowac tla" << std::endl;
    }
    backgroundSprite.setTexture(backgroundTexture);

    // Inicjalizacja przycisków menu
    sf::Text startButton("Start", font, 65);
    startButton.setFillColor(sf::Color::Magenta);
    startButton.setPosition(width / 2 - startButton.getGlobalBounds().width / 2, height / 8 * 5.25);

    sf::Text exitButton("Wyjscie", font, 50);
    exitButton.setFillColor(sf::Color::White);
    exitButton.setPosition(width / 2 - exitButton.getGlobalBounds().width / 2, height / 8 * 6.25);

    sf::Text resetButton("Reset", font, 50);
    resetButton.setFillColor(sf::Color::White);
    resetButton.setPosition(width / 2 - resetButton.getGlobalBounds().width / 2, height / 8 * 7);

    menuItems.push_back(startButton);
    menuItems.push_back(exitButton);
    menuItems.push_back(resetButton);

    // £adowanie tekstur psów za pomoc¹ PuppyBuilder
    bordiBuilder->buildHappyTexture();
    maltiBuilder->buildHappyTexture();
    goldiBuilder->buildHappyTexture();

    bordiSprite.setTexture(bordiBuilder->getPuppy()->textureHappy);
    maltiSprite.setTexture(maltiBuilder->getPuppy()->textureHappy);
    goldiSprite.setTexture(goldiBuilder->getPuppy()->textureHappy);

    bordiSprite.setPosition(130, 640);
    maltiSprite.setPosition(360, 640);
    goldiSprite.setPosition(600, 640);
}

Menu::~Menu() {
    delete bordiBuilder;
    delete maltiBuilder;
    delete goldiBuilder;
}

// Rysowanie menu i wszystkich jego komponentów
void Menu::draw(sf::RenderWindow& window) {
    window.draw(backgroundSprite);
    for (const auto& item : menuItems) {
        window.draw(item);
    }
}

// Logika przesuwania wyboru w menu w górê
void Menu::moveUp() {
    if (selectedItemIndex - 1 >= 0) {
        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
        selectedItemIndex--;
        menuItems[selectedItemIndex].setFillColor(sf::Color::Magenta);
    }
}

// Logika przesuwania wyboru w menu w dó³
void Menu::moveDown() {
    if (selectedItemIndex+1 < menuItems.size()) {
        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
        selectedItemIndex++;
        menuItems[selectedItemIndex].setFillColor(sf::Color::Magenta);
    }
}


// Logika wyboru opcji w menu
void Menu::choice(sf::RenderWindow& window, const sf::Event& event) {
    bordiSprite.setPosition(30, 300);
    maltiSprite.setPosition(300, 340);
    goldiSprite.setPosition(460, 210);

    bordiLabel.setFont(font);
    bordiLabel.setString("Bordi");
    bordiLabel.setCharacterSize(34);
    bordiLabel.setFillColor(sf::Color::Black);
    bordiLabel.setPosition(130, 640);

    maltiLabel.setFont(font);
    maltiLabel.setString("Malti");
    maltiLabel.setCharacterSize(34);
    maltiLabel.setFillColor(sf::Color::Black);
    maltiLabel.setPosition(360, 640);

    goldiLabel.setFont(font);
    goldiLabel.setString("Goldi");
    goldiLabel.setCharacterSize(34);
    goldiLabel.setFillColor(sf::Color::Black);
    goldiLabel.setPosition(600, 640);
    sf::RectangleShape background(sf::Vector2f(window.getSize().x, window.getSize().y));
    background.setFillColor(sf::Color(150, 50, 250));
    window.draw(background);

    // Rysowanie sprite'ów psów
    window.draw(bordiSprite);
    window.draw(maltiSprite);
    window.draw(goldiSprite);
    window.draw(bordiLabel);
    window.draw(maltiLabel);
    window.draw(goldiLabel);

    // Tytu³
    title.setFont(font);
    title.setString("Wybierz psa");
    title.setCharacterSize(40);
    title.setFillColor(sf::Color::Black);
    title.setPosition(window.getSize().x / 2 - title.getGlobalBounds().width / 2, 150);
    window.draw(title);
    sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

    float bordiScale = bordiSprite.getGlobalBounds().contains(mousePos) ? 1.2f : 1.0f;
    float maltiScale = maltiSprite.getGlobalBounds().contains(mousePos) ? 1.1f : 1.0f;
    float goldiScale = goldiSprite.getGlobalBounds().contains(mousePos) ? 1.3f : 1.2f;

    // Ustaw nowe skale
    bordiSprite.setScale(bordiScale, bordiScale);
    maltiSprite.setScale(maltiScale, maltiScale);
    goldiSprite.setScale(goldiScale, goldiScale);

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

        if (bordiSprite.getGlobalBounds().contains(mousePos)) {
            int pies = 1;
            PuppyNamingMenu namingMenu(window, 800, 800, "reszta/salon.jpg", pies);
            namingMenu.activate(window);
        }
        else if (maltiSprite.getGlobalBounds().contains(mousePos)) {
            int pies = 2;
            PuppyNamingMenu namingMenu(window, 800, 800, "reszta/salon.jpg", pies);
            namingMenu.activate(window);
        }
        else if (goldiSprite.getGlobalBounds().contains(mousePos)) {
            int pies = 3;
            PuppyNamingMenu namingMenu(window, 800, 800, "reszta/salon.jpg", pies);
            namingMenu.activate(window);
        }
    }
}

void Menu::setShowPuppies(bool show) {
    showPuppies = show;
}