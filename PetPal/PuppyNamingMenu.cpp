#include "PuppyNamingMenu.h"

void savePuppyData(int selectedPuppy, const std::string& puppyName) {
    std::ofstream outFile("puppy_data.txt", std::ios::out);
    if (outFile.is_open()) {
        outFile << selectedPuppy << " " << puppyName << std::endl;
        outFile.close();
    }
    else {
        std::cerr << "Nie mozna otworzyc pliku puppy_data.txt" << std::endl;
    }
}

PuppyNamingMenu::PuppyNamingMenu(sf::RenderWindow& win, float width, float height, const std::string& backgroundPath, int selectedPuppy)
    : Menu(width, height, backgroundPath), window(win), selectedPuppy(selectedPuppy), namePattern("[A-Z]+[a-zA-Z]*") {

    if (!font.loadFromFile("OpenSans-ExtraBoldItalic.ttf")) {
        std::cerr << "Nie udalo sie zaladowac czcionki" << std::endl;
    }

    nameInputPrompt.setFont(font);
    nameInputPrompt.setString("Podaj imie swojego zwierzaka:");
    nameInputPrompt.setCharacterSize(24);
    nameInputPrompt.setFillColor(sf::Color::Black);
    nameInputPrompt.setPosition(50, 50);

    puppyNameDisplay.setFont(font);
    puppyNameDisplay.setCharacterSize(24);
    puppyNameDisplay.setFillColor(sf::Color::Black);
    puppyNameDisplay.setPosition(50, 120);
    puppyNameDisplay.setString("Imie psiaka: ");

    textBox.setSize(sf::Vector2f(400, 50));
    textBox.setFillColor(sf::Color::White);
    textBox.setOutlineColor(sf::Color::Black);
    textBox.setOutlineThickness(2);
    textBox.setPosition(50, 100);

    switch (selectedPuppy) {
    case 1:
        bordiBuilder->buildHappyTexture();
        selectedPuppySprite.setTexture(bordiBuilder->getPuppy()->textureHappy);
        break;
    case 2:
        maltiBuilder->buildHappyTexture();
        selectedPuppySprite.setTexture(maltiBuilder->getPuppy()->textureHappy);
        break;
    case 3:
        goldiBuilder->buildHappyTexture();
        selectedPuppySprite.setTexture(goldiBuilder->getPuppy()->textureHappy);
        break;
    default:
        std::cerr << "Nie ma takiego psa" << std::endl;
    }
    selectedPuppySprite.setPosition(300, 300);
}

void PuppyNamingMenu::draw(sf::RenderWindow& window) {
    window.clear(sf::Color::White);
    window.draw(nameInputPrompt);
    window.draw(textBox);
    window.draw(puppyNameDisplay);
    window.draw(selectedPuppySprite);
}

void PuppyNamingMenu::handleInput(const sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::TextEntered) {
        if (event.text.unicode < 128) {
            char typedChar = static_cast<char>(event.text.unicode);

            if (typedChar == '\b' && !puppyName.empty()) {
                puppyName.pop_back();
            }
            else if (std::isprint(typedChar)) {
                puppyName += typedChar;
            }

            puppyNameDisplay.setString(puppyName);

            if (typedChar == '\r' || typedChar == '\n') {
                if (std::regex_match(puppyName, namePattern)) {
                    std::cout << "Imie zaakceptowane: " << puppyName << std::endl;
                    savePuppyData(selectedPuppy, puppyName);
                    Game game(window, selectedPuppy);
                    game.setPuppyName(puppyName); // Set the puppy name in the game
                    game.run();
                }
                else {
                    puppyNameDisplay.setString("Bledne imie. \n Imie musi sie zaczynac duza litera \n oraz skladac sie tylko z liter.");
                    puppyName.clear();
                }
            }
        }
    }
}

void PuppyNamingMenu::activate(sf::RenderWindow& window) {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            handleInput(event, window);
        }

        draw(window);
        window.display();
    }
}
