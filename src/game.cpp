#include "game.hpp"
#include <sstream>
#include <iostream>
#include <algorithm>

constexpr int BLOCK_SIZE = 30;

Game::Game()
    : current(rand() % 7),
      scoreText(font, "", 20),
      gameOverText(font, "Game Over!", 30),
      pausedText(font, "Paused", 30) {

    if (!font.openFromFile("res/arial.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
        std::exit(1);
    }

    scoreText = sf::Text(font, "", 20);
    scoreText.setPosition({10.f, 10.f});

    gameOverText = sf::Text(font, "Game Over!", 30);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition({COLS * BLOCK_SIZE / 2.f - 70.f, ROWS * BLOCK_SIZE / 2.f - 20.f});

    pausedText = sf::Text(font, "Paused", 30);
    pausedText.setFillColor(sf::Color::Yellow);
    pausedText.setPosition({COLS * BLOCK_SIZE / 2.f - 50.f, ROWS * BLOCK_SIZE / 2.f - 20.f});

    previewCount = std::clamp(previewCount, 1, 5);
    for (int i = 0; i < previewCount; ++i) {
        upcoming.push_back(Tetromino(rand() % 7));
    }

    current = upcoming.front();
    upcoming.pop_front();
    upcoming.push_back(Tetromino(rand() % 7));

    sf::VideoMode mode({COLS * BLOCK_SIZE + 150, ROWS * BLOCK_SIZE});
    window.create(mode, "Tetris");
}

void Game::run() {
    sf::Clock clock;
    float delay = 0.5f;
    float timer = 0;
    while (window.isOpen()) {
        float time = clock.restart().asSeconds();
        timer += time;

        handleEvents();
        if (!paused && !gameOver) {
            handleDAS(time);
            if (timer > delay) {
                timer = 0;
                if (!current.move({0, 1}, grid)) lockAndNext();
            }
        }
        draw();
    }
}

void Game::handleEvents() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            auto key = keyPressed->scancode;

            if (key == sf::Keyboard::Scancode::P) {
                paused = !paused;
                return;
            }

            if (paused || gameOver) return;

            switch (key) {
                case sf::Keyboard::Scancode::A:
                    current.move({-1, 0}, grid);
                    moveHoldKey = key;
                    moveHoldTimer = -dasDelay;
                    break;
                case sf::Keyboard::Scancode::D:
                    current.move({1, 0}, grid);
                    moveHoldKey = key;
                    moveHoldTimer = -dasDelay;
                    break;
                case sf::Keyboard::Scancode::S:
                    current.move({0, 1}, grid);
                    moveHoldKey = key;
                    moveHoldTimer = -dasDelay;
                    break;
                case sf::Keyboard::Scancode::W:
                    current.rotate(grid);
                    break;
                case sf::Keyboard::Scancode::Space:
                    while (current.move({0, 1}, grid)) {}
                    lockAndNext();
                    break;
                default:
                    break;
            }
        }

        if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>()) {
            if (moveHoldKey == keyReleased->scancode) {
                moveHoldKey = sf::Keyboard::Scancode::Unknown;
            }
        }
    }
}

void Game::lockAndNext() {
    for (auto& b : current.getAbsoluteCoords())
        if (b.y >= 0) grid[b.y][b.x] = current.type + 1;

    int lines = clearLines();
    score += lines * 100;
    if (lines == 3) score += 100;
    if (lines == 4) score += 200;

    current = upcoming.front();
    upcoming.pop_front();
    upcoming.push_back(Tetromino(rand() % 7));

    if (!current.isValid(grid)) gameOver = true;
}

int Game::clearLines() {
    int cleared = 0;
    for (int y = ROWS - 1; y >= 0; --y) {
        bool full = true;
        for (int x = 0; x < COLS; ++x)
            if (!grid[y][x]) { full = false; break; }
        if (full) {
            for (int row = y; row > 0; --row)
                grid[row] = grid[row - 1];
            grid[0].fill(0);
            ++cleared; ++y;
        }
    }
    return cleared;
}

void Game::draw() {
    window.clear(sf::Color::Black);
    drawFixedBlocks();
    drawGridLines();
    drawGhost();
    drawCurrent();
    drawPreviews();
    drawScore();
    if (paused) window.draw(pausedText);
    if (gameOver) window.draw(gameOverText);
    window.display();
}

void Game::drawFixedBlocks() {
    for (int y = 0; y < ROWS; ++y)
        for (int x = 0; x < COLS; ++x)
            if (grid[y][x]) drawBlock(x, y, getColor(grid[y][x]));
}

void Game::drawBlock(int x, int y, sf::Color color, Point offset) {
    sf::RectangleShape rect;
    rect.setSize({BLOCK_SIZE - 2.f, BLOCK_SIZE - 2.f});
    rect.setPosition({
        static_cast<float>(x * BLOCK_SIZE + 1 + offset.x),
        static_cast<float>(y * BLOCK_SIZE + 1 + offset.y)
    });
    rect.setFillColor(color);
    window.draw(rect);
}

void Game::drawBlockAbsolute(int px, int py, sf::Color color) {
    sf::RectangleShape rect;
    rect.setSize({BLOCK_SIZE - 2.f, BLOCK_SIZE - 2.f});
    rect.setPosition({static_cast<float>(px), static_cast<float>(py)});
    rect.setFillColor(color);
    window.draw(rect);
}

void Game::drawGridLines() {
    sf::VertexArray lines(sf::PrimitiveType::Lines);
    for (int x = 0; x <= COLS; ++x) {
        float px = static_cast<float>(x * BLOCK_SIZE);
        lines.append(sf::Vertex{{px, 0.f}, sf::Color(50, 50, 50)});
        lines.append(sf::Vertex{{px, static_cast<float>(ROWS * BLOCK_SIZE)}, sf::Color(50, 50, 50)});
    }
    for (int y = 0; y <= ROWS; ++y) {
        float py = static_cast<float>(y * BLOCK_SIZE);
        lines.append(sf::Vertex{{0.f, py}, sf::Color(50, 50, 50)});
        lines.append(sf::Vertex{{static_cast<float>(COLS * BLOCK_SIZE), py}, sf::Color(50, 50, 50)});
    }
    window.draw(lines);
}

void Game::drawGhost() {
    Tetromino ghost = current;
    while (ghost.move({0, 1}, grid)) {}
    for (auto& b : ghost.getAbsoluteCoords()) {
        sf::Color ghostColor = getColor(current.type + 1);
        ghostColor.a = 80;
        drawBlock(b.x, b.y, ghostColor);
    }
}

void Game::drawCurrent() {
    for (auto& b : current.getAbsoluteCoords())
        drawBlock(b.x, b.y, getColor(current.type + 1));
}

void Game::drawPreviews() {
    int baseX = COLS * BLOCK_SIZE + 20;
    int baseY = 60;

    sf::Text label(font, "Next", 20);
    label.setPosition({static_cast<float>(baseX), static_cast<float>(baseY - 30)});
    window.draw(label);

    for (int i = 0; i < previewCount; ++i) {
        const Tetromino& t = upcoming[i];
        int offsetY = baseY + i * (BLOCK_SIZE * 3 + 10);
        for (const auto& b : t.blocks) {
            int px = b.x * BLOCK_SIZE + baseX;
            int py = b.y * BLOCK_SIZE + offsetY;
            drawBlockAbsolute(px, py, getColor(t.type + 1));
        }
    }
}

void Game::drawScore() {
    std::stringstream ss;
    ss << "Score: " << score;
    scoreText.setString(ss.str());
    window.draw(scoreText);
}

void Game::handleDAS(float dt) {
    if (moveHoldKey == sf::Keyboard::Scancode::A || moveHoldKey == sf::Keyboard::Scancode::D) {
        moveHoldTimer += dt;
        while (moveHoldTimer >= dasRepeat) {
            if (moveHoldKey == sf::Keyboard::Scancode::A)
                current.move({-1, 0}, grid);
            else
                current.move({1, 0}, grid);
            moveHoldTimer -= dasRepeat;
        }
    }
}

sf::Color Game::getColor(int type) {
    static sf::Color colors[] = {
        sf::Color::White, sf::Color::Cyan, sf::Color::Blue,
        sf::Color(255,165,0), sf::Color::Yellow,
        sf::Color::Green, sf::Color::Red, sf::Color::Magenta
    };
    return colors[type % 8];
}