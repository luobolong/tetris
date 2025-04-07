#pragma once
#include <vector>
#include <array>
#include "grid.hpp" // 提供 Grid 类型定义
#include "point.hpp" // 提供 Point 结构定义（int x, y）

struct Tetromino;

void applyRotation(Tetromino& piece, const Grid& grid);
