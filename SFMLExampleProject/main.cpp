#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

class Game {
public:
    static const int MAZE_WIDTH = 10;
    static const int MAZE_HEIGHT = 10;

    char level1[MAZE_HEIGHT][MAZE_WIDTH] = {
           {'1', '1', '1', '1', '1', '1', '1', '1', '1', '1'},
           {'1', 'P', '0', '0', '0', '0', '0', '0', '0', '1'},
           {'1', '1', '1', '0', '1', '1', '1', '0', '1', '1'},
           {'1', '0', '1', '0', '0', '0', '0', '0', '0', '1'},
           {'1', '0', '1', '1', '1', '1', '1', '1', '0', '1'},
           {'1', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
           {'1', '0', '1', '1', '1', '1', '1', '1', '1', '1'},
           {'1', '0', '1', '0', '0', '0', '0', '1', 'E', '1'},
           {'1', '0', '0', '0', '1', '1', '0', '0', '0', '1'},
           {'1', '1', '1', '1', '1', '1', '1', '1', '1', '1'}
    };

    char level2[MAZE_HEIGHT][MAZE_WIDTH] = {
        {'1', '1', '1', '1', '1', '1', '1', '1', '1', '1'},
        {'1', 'P', '0', '0', '1', 'E', '0', '0', '0', '1'},
        {'1', '0', '1', '0', '1', '1', '1', '0', '1', '1'},
        {'1', '0', '1', '0', '0', '0', '0', '0', '0', '1'},
        {'1', '0', '1', '1', '1', '1', '1', '1', '0', '1'},
        {'1', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
        {'1', '0', '1', '1', '1', '1', '1', '1', '1', '1'},
        {'1', '0', '1', '0', '0', '0', '0', '1', '0', '1'},
        {'1', '0', '0', '0', '1', '1', '0', '0', '0', '1'},
        {'1', '1', '1', '1', '1', '1', '1', '1', '1', '1'}
    };

    char level3[MAZE_HEIGHT][MAZE_WIDTH] = {
     {'1', '1', '1', '1', '1', '1', '1', '1', '1', '1'},
     {'1', 'P', '0', '0', '1', '0', '0', '0', '0', '1'},
     {'1', '0', '1', '0', '1', '1', '1', '0', '1', '1'},
     {'1', '0', '1', '0', '0', '0', '0', '0', '0', '1'},
     {'1', '0', '1', '1', '1', '1', '1', '1', '0', '1'},
     {'1', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
     {'1', '0', '1', '1', '1', '1', '1', '1', '1', '1'},
     {'1', '0', '1', '0', '0', '0', '0', '1', 'E', '1'},
     {'1', '0', '0', '0', '1', '1', '0', '0', '0', '1'},
     {'1', '1', '1', '1', '1', '1', '1', '1', '1', '1'}
    };



    void updateTimer(float deltaTime) {
        // Update the timer
        timer -= deltaTime;
        if (timer <= 0.0f) {
            // Time's up, handle game over logic
            // TODO: Implement game over logic here
        }
    }

    enum class State {
        MainMenu,
        Level1,
        Level2,
        Level3,
        Paused,
        GameWin,
        GameLose
    };

    struct Entity {
        sf::Vector2f position;
        sf::Vector2f velocity;
        sf::RectangleShape shape;
    };

    Game() : window(sf::VideoMode(800, 600), "The Adventure"), state(State::MainMenu), currentLevel(1), timer(15.0f), deltaTime(0.0f) {
        // Init main menu buttons
        startButton.setSize(sf::Vector2f(200, 50));
        startButton.setFillColor(sf::Color::Green);
        startButton.setPosition(300, 200);

        player.shape.setSize(sf::Vector2f(20.0f, 20.0f));
        player.shape.setFillColor(sf::Color::Blue);
        player.shape.setOrigin(player.shape.getSize() / 2.0f); // Set origin at the center of the shape

        exitButton.setSize(sf::Vector2f(200, 50));
        exitButton.setFillColor(sf::Color::Red);
        exitButton.setPosition(300, 300);

        // Load font for button text
        font.loadFromFile("fonts/arial.ttf");

        // Set up start button text
        startButtonText.setFont(font);
        startButtonText.setString("Start");
        startButtonText.setCharacterSize(24);
        startButtonText.setFillColor(sf::Color::White);
        sf::FloatRect textBounds = startButtonText.getLocalBounds();
        startButtonText.setOrigin(textBounds.width / 2.0f, textBounds.height / 2.0f);
        startButtonText.setPosition(startButton.getPosition() + startButton.getSize() / 2.0f);

        // Set up exit button text
        exitButtonText.setFont(font);
        exitButtonText.setString("Exit");
        exitButtonText.setCharacterSize(24);
        exitButtonText.setFillColor(sf::Color::White);
        textBounds = exitButtonText.getLocalBounds();
        exitButtonText.setOrigin(textBounds.width / 2.0f, textBounds.height / 2.0f);
        exitButtonText.setPosition(exitButton.getPosition() + exitButton.getSize() / 2.0f);

        // Set up HUD
        timerText.setFont(font);
        timerText.setCharacterSize(24);
        timerText.setFillColor(sf::Color::White);
        timerText.setPosition(10.0f, 10.0f);
    }

    void run() {
        sf::Clock clock;
        while (window.isOpen()) {
            sf::Time deltaTime = clock.restart();
            float dt = deltaTime.asSeconds();

            update(dt);
            draw();

            // Process events
            sf::Event event;
            while (window.pollEvent(event)) {
                handleInput(event); // Pass the event directly to handleInput
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }
        }
    }

private:
    static constexpr float enemySpeed = 0.1f;

    float tileSize = 60.0f; // Adjust the tile size
    char maze[MAZE_HEIGHT][MAZE_WIDTH];

    sf::Text startButtonText; // Text for start button
    sf::Text exitButtonText; // Text for exit button
    sf::Text timerText; // Text for timer
    sf::Font font; // Font for button and timer text

    sf::RectangleShape startButton;
    sf::RectangleShape exitButton;
    sf::RenderWindow window;
    State state;
    int currentLevel;
    Entity player;
    float timer;
    float deltaTime;

    sf::Vector2f normalize(sf::Vector2f vector) {
        float length = std::sqrt(vector.x * vector.x + vector.y * vector.y);
        if (length != 0.0f) {
            vector.x /= length;
            vector.y /= length;
        }
        return vector;
    }

    void handleInput(const sf::Event& event) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        // Main menu state
        if (state == State::MainMenu) {
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mousePosition = window.mapPixelToCoords(mousePos);

                // Check start button
                if (startButton.getGlobalBounds().contains(mousePosition)) {
                    state = State::Level1;
                    loadLevel(1);
                }

                // Check exit button
                if (exitButton.getGlobalBounds().contains(mousePosition)) {
                    window.close();
                }
            }
        }

        // Level states
        if (state == State::Level1 || state == State::Level2 || state == State::Level3) {
            if (event.type == sf::Event::KeyPressed) {
                // Handle player movement
                if (event.key.code == sf::Keyboard::Up) {
                    player.velocity.y = -5;
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    player.velocity.y = 5;
                }
                else if (event.key.code == sf::Keyboard::Left) {
                    player.velocity.x = -5;
                }
                else if (event.key.code == sf::Keyboard::Right) {
                    player.velocity.x = 5;
                }
            }
            else if (event.type == sf::Event::KeyReleased) {
                // Stop player movement when key is released
                if (event.key.code == sf::Keyboard::Up && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                    player.velocity.y = 0;
                }
                else if (event.key.code == sf::Keyboard::Down && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                    player.velocity.y = 0;
                }
                else if (event.key.code == sf::Keyboard::Left && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                    player.velocity.x = 0;
                }
                else if (event.key.code == sf::Keyboard::Right && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                    player.velocity.x = 0;
                }
            }
        }
    }



    void update(float deltaTime) {
        if (state == State::Level1 || state == State::Level2 || state == State::Level3) {
            // Move player
            sf::Vector2f oldPosition = player.position; // Save old position
            player.position += player.velocity * tileSize * deltaTime; // Update position

           // Calculate the player's new tile position
            int newTileX = static_cast<int>(player.position.x / tileSize);
            int newTileY = static_cast<int>(player.position.y / tileSize);


            // Check collision with walls
            if (maze[newTileY][newTileX] == '1') {
                // Collision detected, revert to old position
                player.position = oldPosition;
            }

            // Update timer
            updateTimer(deltaTime);

            // Calculate the player's tile position after moving
            int tileX = static_cast<int>(player.position.x / tileSize);
            int tileY = static_cast<int>(player.position.y / tileSize);


            // Check if time's up
            if (timer <= 0.0f) {
                // Transition to the lose state
                state = State::GameLose;
            }

            // Check if player has reached the end tile ('E')
            if (maze[tileY][tileX] == 'E') {
                if (currentLevel == 3) {
                    // Player has completed all levels
                    state = State::GameWin;
                }
                else {
                    // Transition to the next level
                    currentLevel++;
                    loadLevel(currentLevel);
                }
            }

            // Update camera position to follow the player
            sf::View view = window.getView();
            view.setCenter(player.position);
            window.setView(view);

            // Set the timer text's position relative to the view's center after updating the view
            timerText.setPosition(view.getCenter().x - window.getSize().x / 2 + 10, view.getCenter().y - window.getSize().y / 2 + 10);

        }
    }

    void centerText(sf::Text& text, sf::RenderWindow& window) {
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        text.setPosition(sf::Vector2f(window.getSize().x / 2.0f - textRect.width / 2.0f, window.getSize().y / 2.0f - textRect.height / 2.0f));
    }



    void draw() {
        window.clear();

        if (state == State::MainMenu) {
            window.draw(startButton);
            window.draw(startButtonText); // Draw start button text
            window.draw(exitButton);
            window.draw(exitButtonText); // Draw exit button text
        }

        if (state == State::GameWin) {
            sf::Text winText;
            winText.setFont(font);
            winText.setString("You win! You can close the game now.");
            winText.setCharacterSize(24);
            winText.setFillColor(sf::Color::White);

            // Center the win text in the middle of the window
            sf::FloatRect textBounds = winText.getLocalBounds();
            winText.setOrigin(textBounds.width / 2.0f, textBounds.height / 2.0f);
            winText.setPosition(window.getView().getCenter());

            window.draw(winText);
        }

        if (state == State::GameLose) {
            sf::Text loseText;
            loseText.setFont(font);
            loseText.setString("You Lose! You can close the game now.");
            loseText.setCharacterSize(24);
            loseText.setFillColor(sf::Color::White);

            // Center the lose text in the middle of the window
            sf::FloatRect textBounds = loseText.getLocalBounds();
            loseText.setOrigin(textBounds.width / 2.0f, textBounds.height / 2.0f);
            loseText.setPosition(window.getView().getCenter());

            window.draw(loseText);
        }


        if (state == State::Level1 || state == State::Level2 || state == State::Level3) {
            // Draw maze
            for (int i = 0; i < MAZE_HEIGHT; i++) {
                for (int j = 0; j < MAZE_WIDTH; j++) {
                    sf::RectangleShape tile;
                    tile.setSize(sf::Vector2f(tileSize, tileSize));
                    tile.setPosition(j * tileSize, i * tileSize);

                    if (maze[i][j] == '1') {
                        tile.setFillColor(sf::Color::Red); // Collision tile
                    }
                    else if (maze[i][j] == 'E') {
                        tile.setFillColor(sf::Color::Yellow); // Finish line tile
                    }
                    else {
                        tile.setFillColor(sf::Color::White); // Path tile
                    }

                    window.draw(tile);
                }
            }

            // Draw player
            player.shape.setPosition(player.position);
            window.draw(player.shape);

            // Draw timer
            timerText.setString("Timer: " + std::to_string(static_cast<int>(timer)));
            window.draw(timerText);
        }

        window.display(); // Update the window and make the drawn objects visible
    }

    void loadLevel(int levelNumber) {
        if (levelNumber == 1) {
            for (int i = 0; i < MAZE_HEIGHT; i++) {
                for (int j = 0; j < MAZE_WIDTH; j++) {
                    maze[i][j] = level1[i][j];
                    if (maze[i][j] == 'P') {
                        player.position.x = j * tileSize + tileSize / 2.0f;  // Set player's initial x position
                        player.position.y = i * tileSize + tileSize / 2.0f;  // Set player's initial y position
                    }
                }
            }
        }
        else if (levelNumber == 2) {
            for (int i = 0; i < MAZE_HEIGHT; i++) {
                for (int j = 0; j < MAZE_WIDTH; j++) {
                    maze[i][j] = level2[i][j];
                    if (maze[i][j] == 'P') {
                        player.position.x = j * tileSize + tileSize / 2.0f;  // Set player's initial x position
                        player.position.y = i * tileSize + tileSize / 2.0f;  // Set player's initial y position
                    }
                }
            }
        }
        else if (levelNumber == 3) {
            for (int i = 0; i < MAZE_HEIGHT; i++) {
                for (int j = 0; j < MAZE_WIDTH; j++) {
                    maze[i][j] = level3[i][j];
                    if (maze[i][j] == 'P') {
                        player.position.x = j * tileSize + tileSize / 2.0f;  // Set player's initial x position
                        player.position.y = i * tileSize + tileSize / 2.0f;  // Set player's initial y position
                    }
                }
            }
        }
    }

};

int main() {
    Game game;
    game.run();
    return 0;
}