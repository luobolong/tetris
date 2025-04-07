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

    int previewCount = 1;
    int score = 0;
    bool gameOver = false;
    bool paused = false;

    // DAS（Delayed Auto Shift）相关
    sf::Keyboard::Scancode moveHoldKey = sf::Keyboard::Scancode::Unknown;
    float moveHoldTimer = 0.f;
    const float dasDelay = 0.2f;
    const float dasRepeat = 0.05f;

    // 字体与文本
    sf::Font font;
    sf::Text scoreText;
    sf::Text gameOverText;
    sf::Text pausedText;

    // 事件处理
    void handleEvents();
    void handleDAS(float dt);

    // 游戏核心逻辑
    void lockAndNext();
    int clearLines();

    // 渲染逻辑
    void draw();
    void drawFixedBlocks();
    void drawBlock(int x, int y, sf::Color color, Point offset = {0, 0});
    void drawBlockAbsolute(int px, int py, sf::Color color);
    void drawGridLines();
    void drawGhost();
    void drawCurrent();
    void drawPreviews();
    void drawScore();

    // 工具函数
    sf::Color getColor(int type);
};
