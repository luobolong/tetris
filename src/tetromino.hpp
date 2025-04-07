#pragma once
#include <vector>
#include "point.hpp"
#include "grid.hpp"

class Tetromino {
public:
    std::vector<Point> blocks;
    int type;
    Point pos;
    int rotation = 0;

    Tetromino(int t);
    std::vector<Point> getAbsoluteCoords(Point offset = {0, 0}) const;
    bool move(Point d, const Grid& grid);
    void rotate(const Grid& grid);
    bool isValid(const Grid& grid) const;
};
