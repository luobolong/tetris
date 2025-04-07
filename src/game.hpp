#pragma once

#include <SFML/Graphics.hpp>
#include <deque>
#include "tetromino.hpp"
#include "grid.hpp"
#include "point.hpp"

class Game {
public:
    explicit Game();
    void run();

private:
    sf::RenderWindow window;

    Grid grid = {};
    Tetromino current;
    std::deque<Tetromino> upcoming;

    int previewCount = 5;
    int score = 0;
    bool gameOver = false;
    bool paused = false;

    // DAS
    sf::Keyboard::Scancode moveHoldKey = sf::Keyboard::Scancode::Unknown;
    float moveHoldTimer = 0.f;
    const float dasDelay = 0.2f;
    const float dasRepeat = 0.05f;

    // font and text
    sf::Font font;
    sf::Text scoreText;
    sf::Text gameOverText;
    sf::Text pausedText;

    // handle events
    void handleEvents();
    void handleDAS(float dt);

    // game logic
    void lockAndNext();
    int clearLines();

    // render
    void draw();
    void drawFixedBlocks();
    void drawBlock(int x, int y, sf::Color color, Point offset = {0, 0});
    void drawBlockAbsolute(int px, int py, sf::Color color);
    void drawGridLines();
    void drawGhost();
    void drawCurrent();
    void drawPreviews();
    void drawScore();

    // tools
    sf::Color getColor(int type);
};
