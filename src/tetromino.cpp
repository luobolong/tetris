#include "tetromino.hpp"
#include "rotation.hpp"

std::vector<std::vector<Point>> shapes = {
    {{0,0}, {1,0}, {0,1}, {1,1}},        // O
    {{0,0}, {1,0}, {2,0}, {3,0}},        // I
    {{0,0}, {1,0}, {2,0}, {2,1}},        // L
    {{0,0}, {1,0}, {2,0}, {0,1}},        // J
    {{0,0}, {1,0}, {1,1}, {2,1}},        // S
    {{0,1}, {1,1}, {1,0}, {2,0}},        // Z
    {{0,0}, {1,0}, {2,0}, {1,1}},        // T
};

Tetromino::Tetromino(int t) : type(t), pos({COLS / 2 - 2, 0}) {
    blocks = shapes[t];
}

std::vector<Point> Tetromino::getAbsoluteCoords(Point offset) const {
    std::vector<Point> result;
    for (auto b : blocks)
        result.push_back({b.x + pos.x + offset.x, b.y + pos.y + offset.y});
    return result;
}

bool Tetromino::move(Point d, const Grid& grid) {
    auto coords = getAbsoluteCoords(d);
    for (auto& c : coords) {
        if (c.x < 0 || c.x >= COLS || c.y >= ROWS || (c.y >= 0 && grid[c.y][c.x]))
            return false;
    }
    pos.x += d.x; pos.y += d.y;
    return true;
}

void Tetromino::rotate(const Grid& grid) {
    applyRotation(*this, grid);
}

bool Tetromino::isValid(const Grid& grid) const {
    for (auto& c : getAbsoluteCoords()) {
        if (c.x < 0 || c.x >= COLS || c.y >= ROWS || (c.y >= 0 && grid[c.y][c.x]))
            return false;
    }
    return true;
}
