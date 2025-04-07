#pragma once
#include <vector>
#include <array>
#include "grid.hpp"
#include "point.hpp"

struct Tetromino;

void applyRotation(Tetromino& piece, const Grid& grid);
