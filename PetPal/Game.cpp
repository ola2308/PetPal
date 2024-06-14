#include "Game.h"

//Używana by sprawdzic czy puppy_data.txt ma dane
bool isFileEmpty(const std::string& filename) {
    std::ifstream file(filename);
    return file.peek() == std::ifstream::traits_type::eof();
}

//obsługuje menu i wybór opcji gry
void runGame() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "PetPal");
    Menu menu(window.getSize().x, window.getSize().y, "reszta/menu.jpg");
    int selectedItem = -1;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    menu.moveUp();
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    menu.moveDown();
                }
                else if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Enter) {
                    selectedItem = menu.getPressedItem();
                    switch (selectedItem) {
                    case 0:
                        if (isFileEmpty("puppy_data.txt")) {
                            menu.setShowPuppies(true);
                        }
                        else {
                            int selectedPuppy;
                            std::string puppyName;
                            std::ifstream inFile("puppy_data.txt");
                            if (inFile.is_open()) {
                                std::string line;
                                std::getline(inFile, line);
                                std::istringstream iss(line);
                                iss >> selectedPuppy >> puppyName;
                                inFile.close();
                                Game game(window, selectedPuppy);
                                game.setPuppyName(puppyName);
                                game.loadGameState();
                                game.displayWelcomeMessage();
                                game.run();
                            }
                            else {
                                std::cerr << "Nie mozna otworzyc pliku" << std::endl;
                            }
                        }
                        break;
                    case 1:
                        std::cout << "Wyjscie" << std::endl;
                        window.close();
                        break;
                    case 2:
                        std::cout << "Reset" << std::endl;
                        if (remove("puppy_data.txt") == 0) {
                            std::cout << "Plik puppy_data.txt został usuniety" << std::endl;
                        }
                        else {
                            std::cerr << "Nie mozna usunac pliku puppy_data.txt" << std::endl;
                        }
                        if (remove("puppy_state.txt") == 0) {
                            std::cout << "Plik puppy_state.txt został usuniety" << std::endl;
                        }
                        else {
                            std::cerr << "Nie mozna usunac pliku puppy_state.txt" << std::endl;
                        }
                        window.close();
                        runGame();
                        break;
                    }
                }
            }
        }

        window.clear();
        if (menu.getShowPuppies()) {
            menu.choice(window, event);
        }
        else {
            menu.draw(window);
        }
        window.display();
    }
}

void createmyWindow(sf::RenderWindow& W, sf::String s) {
    W.create(sf::VideoMode(800, 800, 32), s);
    W.setActive(true);
    W.setFramerateLimit(30);
    W.setPosition(sf::Vector2i(0, 0));
    W.setKeyRepeatEnabled(false);
}

void loadTexture(sf::Texture& texture, const std::string& filename) {
    if (!texture.loadFromFile(filename)) {
        std::cerr << "Nie mozna zaladowac tekstury: " << filename << std::endl;
        exit(1);
    }
}

//Konstruktor 
Game::Game(sf::RenderWindow& win, int selectedPuppyIndex)
    : window(win), roomInteraction(win), healthBarManager(), bedroomStrategy(new BedroomStrategy(window, selectedPuppy)), showShelterScreen(false), messageTimer(0.0f), showWelcomeMessage(false), selectedPuppyIndex(selectedPuppyIndex) {
    roomInteraction.setHealthBarManager(&healthBarManager);
    healthBarManager.setRoomInteraction(roomInteraction);

    if (!font.loadFromFile("OpenSans-ExtraBoldItalic.ttf")) {
        std::cerr << "Nie mozna zaladowac czcionki OpenSans-ExtraBoldItalic.ttf" << std::endl;
        exit(1);
    }

    sleepMessage.setFont(font);
    sleepMessage.setString("Spie, najpierw musisz mnie obudzic");
    sleepMessage.setCharacterSize(24);
    sleepMessage.setFillColor(sf::Color::Red);
    sleepMessage.setPosition(200, 400);

    welcomeMessage.setFont(font);
    welcomeMessage.setCharacterSize(40);
    welcomeMessage.setFillColor(sf::Color::Black);
    welcomeMessage.setOutlineColor(sf::Color::White);
    welcomeMessage.setOutlineThickness(2);

    try {
        loadRoomTextures();
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Nie udalo sie zaladowac tekstury: " << e.what() << std::endl;
        throw;
    }

    PuppyBuilder* builder;
    switch (selectedPuppyIndex) {
    case 1: builder = new BordiBuilder(); break;
    case 2: builder = new MaltiBuilder(); break;
    case 3: builder = new GoldiBuilder(); break;
    default: throw std::invalid_argument("Nie ma takiego psiaka");
    }
    builder->buildHappyTexture();
    builder->buildSadTexture();
    builder->buildSleepTexture();
    selectedPuppy = builder->getPuppy();
    puppySprite = selectedPuppy->sprite;

    builders.push_back(builder);

    changeRoom(0);

    loadButtons();

    if (!shelterTexture.loadFromFile("reszta/shelter.png")) {
        std::cerr << "Nie mozna zaladowac tekstury shelter.png" << std::endl;
        exit(1);
    }
    shelterSprite.setTexture(shelterTexture);
    shelterSprite.setPosition((window.getSize().x - shelterSprite.getGlobalBounds().width) / 2,
        (window.getSize().y - shelterSprite.getGlobalBounds().height) / 2);
}

void Game::loadRoomTextures() {
    sf::Texture texture;
    for (const auto& path : roomPaths) {
        loadTexture(texture, path);
        roomTextures.push_back(texture);
    }
}

void Game::changeRoom(int index) {
    BedroomStrategy* bedroomStrategy = dynamic_cast<BedroomStrategy*>(roomInteraction.getCurrentStrategy());
    if (roomInteraction.getCurrentRoomIndex() == 0 && bedroomStrategy && bedroomStrategy->isNight) {
        displaySleepMessage();
        std::cout << "Nie mozna zmienic pokoju, gdy jest noc." << std::endl;
        return;
    }

    if (index >= 0 && index < roomTextures.size()) {
        roomSprite.setTexture(roomTextures[index]);
        roomSprite.setPosition(0, 0);
        currentRoomIndex = index;
        roomInteraction.setCurrentRoomIndex(index);

        if (index == 2) {
            roomInteraction.setStrategy(new BathroomStrategy(window, roomInteraction));
        }
        else if (index == 1) {
            roomInteraction.setStrategy(new KitchenStrategy(window));
        }
        else if (index == 0) {
            roomInteraction.setStrategy(new BedroomStrategy(window, selectedPuppy));
        }
        else if (index == 3) {
            roomInteraction.setStrategy(new PlayRoomStrategy(window));
        }
        else {
            roomInteraction.setStrategy(nullptr);
        }
    }
}

void Game::loadButtons() {
    if (!exitTexture.loadFromFile("reszta/exit.png") || !resetTexture.loadFromFile("reszta/reset.png")) {
        std::cerr << "Nie mozna zaladowac tekstur przyciskow reset.png i exit.png" << std::endl;
        exit(1);
    }

    exitSprite.setTexture(exitTexture);
    exitSprite.setPosition(window.getSize().x - 70, 10);
    exitSprite.setScale(0.75f, 0.75f);

    resetSprite.setTexture(resetTexture);
    resetSprite.setPosition(window.getSize().x - 140, 10);
    resetSprite.setScale(0.75f, 0.75f);
}

Game::~Game() {
    for (auto builder : builders) {
        delete builder;
    }
}

void Game::handleInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            saveGameState();
            window.close();
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            handleMouseClick(mousePos);
        }

        roomInteraction.handleInteraction(currentRoomIndex, event, selectedPuppy->sprite);

        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            std::cout << "Klikniecie myszka na pozycji: (" << mousePos.x << ", " << mousePos.y << ")" << std::endl;
        }

        BedroomStrategy* bedroomStrategy = dynamic_cast<BedroomStrategy*>(roomInteraction.getCurrentStrategy());
        bool isNight = bedroomStrategy && bedroomStrategy->isNight;

        if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::Left)) {
            if (isNight) {
                displaySleepMessage();
            }
            else {
                if (event.key.code == sf::Keyboard::Right) {
                    currentRoomIndex = (currentRoomIndex + 1) % roomTextures.size();
                }
                else if (event.key.code == sf::Keyboard::Left) {
                    currentRoomIndex = (currentRoomIndex - 1 + roomTextures.size()) % roomTextures.size();
                }
                changeRoom(currentRoomIndex);
            }
        }
    }
}

void Game::handleMouseClick(sf::Vector2i mousePos) {
    if (showShelterScreen) {
        if (exitSprite.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
            saveGameState();
            if (remove("puppy_data.txt") == 0) {
                std::cout << "Plik puppy_data.txt zostal usuniety" << std::endl;
            }
            else {
                std::cerr << "Nie mozna usunac pliku puppy_data.txt" << std::endl;
            }
            if (remove("puppy_state.txt") == 0) {
                std::cout << "Plik puppy_state.txt został usuniety" << std::endl;
            }
            else {
                std::cerr << "Nie mozna usunac pliku puppy_state.txt" << std::endl;
            }
            window.close();
        }
        if (resetSprite.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
            saveGameState();
            if (remove("puppy_data.txt") == 0) {
                std::cout << "Plik puppy_data.txt zostal usuniety" << std::endl;
            }
            else {
                std::cerr << "Nie mozna usunac pliku puppy_data.txt" << std::endl;
            }
            if (remove("puppy_state.txt") == 0) {
                std::cout << "Plik puppy_state.txt został usuniety" << std::endl;
            }
            else {
                std::cerr << "Nie mozna usunac pliku puppy_state.txt" << std::endl;
            }
            window.close();
            runGame();
        }
    }
    else {
        if (exitSprite.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
            saveGameState();
            window.close();
        }
        if (resetSprite.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
            saveGameState();
            if (remove("puppy_data.txt") == 0) {
                std::cout << "Plik puppy_data.txt zostal usuniety" << std::endl;
            }
            else {
                std::cerr << "Nie mozna usunac pliku puppy_data.txt" << std::endl;
            }
            if (remove("puppy_state.txt") == 0) {
                std::cout << "Plik puppy_state.txt został usuniety" << std::endl;
            }
            else {
                std::cerr << "Nie mozna usunac pliku puppy_state.txt" << std::endl;
            }
            window.close();
            runGame();
        }
    }
}

void Game::updateHealth(float deltaTime) {
    healthBarManager.updateHealth(deltaTime, selectedPuppy->sprite);

    if (healthBarManager.getCriticalHealthTime() > 30.0f) {
        showShelterScreen = true;
    }

    if (roomInteraction.getCurrentRoomIndex() == 0) {
        BedroomStrategy* bedroomStrategy = dynamic_cast<BedroomStrategy*>(roomInteraction.getCurrentStrategy());
        if (bedroomStrategy && bedroomStrategy->isNight) {
            bedroomStrategy->update(deltaTime, healthBarManager);
        }
    }

    bool isHealthCritical = healthBarManager.isHealthCritical();
    selectedPuppy->updateTexture(!isHealthCritical);

    if (showSleepMessage) {
        messageTimer += deltaTime;
        if (messageTimer >= 2.0f) {
            showSleepMessage = false;
            messageTimer = 0.0f;
        }
    }

    if (showWelcomeMessage) {
        messageTimer += deltaTime;
        if (messageTimer >= 3.0f) {
            showWelcomeMessage = false;
            messageTimer = 0.0f;
        }
    }
}

void Game::draw() {
    window.clear();

    if (showShelterScreen) {
        handleShelterScreen();
    }
    else {
        window.draw(roomSprite);

        bool isHealthCritical = healthBarManager.isHealthCritical();
        selectedPuppy->updateTexture(!isHealthCritical);

        sf::Vector2u windowSize = window.getSize();
        sf::FloatRect puppyBounds = selectedPuppy->sprite.getGlobalBounds();
        selectedPuppy->sprite.setPosition((windowSize.x - puppyBounds.width) / 2, 450);

        window.draw(selectedPuppy->sprite);

        roomInteraction.draw(puppySprite, currentRoomIndex, window);

        healthBarManager.draw(window);

        if (showSleepMessage) {
            window.draw(sleepMessage);
        }

        if (showWelcomeMessage) {
            window.draw(welcomeMessage);
        }

        window.draw(exitSprite);
        window.draw(resetSprite);
    }

    window.display();
}

void Game::handleShelterScreen() {
    window.draw(shelterSprite);
    window.draw(exitSprite);
    window.draw(resetSprite);
}

void Game::displaySleepMessage() {
    showSleepMessage = true;
    messageTimer = 0.0f;
}

void Game::setPuppyName(const std::string& name) {
    puppyName = name;
    welcomeMessage.setString("Tesknilem za Toba - " + puppyName);

    float windowWidth = window.getSize().x;
    float messageWidth = welcomeMessage.getGlobalBounds().width;
    welcomeMessage.setPosition((windowWidth - messageWidth) / 2, window.getSize().y / 2 - welcomeMessage.getGlobalBounds().height / 2);
}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Time elapsed = clock.restart();
        handleInput();
        updateHealth(elapsed.asSeconds());
        draw();
    }
}

void Game::displayWelcomeMessage() {
    showWelcomeMessage = true;
    messageTimer = 0.0f;
}

void Game::saveGameState() {
    std::ofstream outFile("puppy_data.txt");
    if (outFile.is_open()) {
        outFile << selectedPuppyIndex << " " << puppyName << "\n";
        for (const auto& bar : healthBarManager.getBars()) {
            outFile << bar.getCurrentValue() << ",";
        }
        outFile.close();
    }
    else {
        std::cerr << "Nie mozna otworzyc pliku do zapisu" << std::endl;
    }
    roomInteraction.saveState("puppy_state.txt");
}

void Game::loadGameState() {
    std::ifstream inFile("puppy_data.txt");
    if (inFile.is_open()) {
        std::string line;
        std::getline(inFile, line);
        std::getline(inFile, line);
        std::istringstream iss(line);
        std::string value;
        std::vector<float> values;
        while (std::getline(iss, value, ',')) {
            values.push_back(std::stof(value));
        }
        healthBarManager.setBars(values);
        inFile.close();
    }
    else {
        std::cerr << "Nie mozna otworzyc pliku do odczytu" << std::endl;
    }
    roomInteraction.loadState("puppy_state.txt");
}
