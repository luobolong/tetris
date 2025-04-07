#include "rotation.hpp"
#include "tetromino.hpp"

using KickTable = std::array<std::array<std::vector<Point>, 4>, 4>;

// SRS kick table for JLSTZ
const KickTable JLSTZ_KICK_TABLE = {{
    {   // from 0
        std::vector<Point>{{0,0}, {-1,0}, {-1,1}, {0,-2}, {-1,-2}}, // to 1
        std::vector<Point>{{0,0}, {1,0}, {1,1}, {0,-2}, {1,-2}},    // to 2
        std::vector<Point>{{0,0}, {1,0}, {1,-1}, {0,2}, {1,2}},     // to 3
        std::vector<Point>{{0,0}, {-1,0}, {-1,-1}, {0,2}, {-1,2}},  // to 0
    },
    {   // from 1
        std::vector<Point>{{0,0}, {1,0}, {1,-1}, {0,2}, {1,2}},
        std::vector<Point>{{0,0}, {1,0}, {1,1}, {0,-2}, {1,-2}},
        std::vector<Point>{{0,0}, {-1,0}, {-1,-1}, {0,2}, {-1,2}},
        std::vector<Point>{{0,0}, {-1,0}, {-1,1}, {0,-2}, {-1,-2}},
    },
    {   // from 2
        std::vector<Point>{{0,0}, {1,0}, {1,1}, {0,-2}, {1,-2}},
        std::vector<Point>{{0,0}, {-1,0}, {-1,-1}, {0,2}, {-1,2}},
        std::vector<Point>{{0,0}, {-1,0}, {-1,1}, {0,-2}, {-1,-2}},
        std::vector<Point>{{0,0}, {1,0}, {1,-1}, {0,2}, {1,2}},
    },
    {   // from 3
        std::vector<Point>{{0,0}, {-1,0}, {-1,-1}, {0,2}, {-1,2}},
        std::vector<Point>{{0,0}, {-1,0}, {-1,1}, {0,-2}, {-1,-2}},
        std::vector<Point>{{0,0}, {1,0}, {1,-1}, {0,2}, {1,2}},
        std::vector<Point>{{0,0}, {1,0}, {1,1}, {0,-2}, {1,-2}},
    }
}};

// SRS kick table for I piece
const KickTable I_KICK_TABLE = {{
    {
        std::vector<Point>{{0,0}, {-2,0}, {1,0}, {-2,-1}, {1,2}},
        std::vector<Point>{{0,0}, {-1,0}, {2,0}, {-1,2}, {2,-1}},
        std::vector<Point>{{0,0}, {2,0}, {-1,0}, {2,1}, {-1,-2}},
        std::vector<Point>{{0,0}, {1,0}, {-2,0}, {1,-2}, {-2,1}},
    },
    {
        std::vector<Point>{{0,0}, {2,0}, {-1,0}, {2,1}, {-1,-2}},
        std::vector<Point>{{0,0}, {1,0}, {-2,0}, {1,-2}, {-2,1}},
        std::vector<Point>{{0,0}, {-2,0}, {1,0}, {-2,-1}, {1,2}},
        std::vector<Point>{{0,0}, {-1,0}, {2,0}, {-1,2}, {2,-1}},
    },
    {
        std::vector<Point>{{0,0}, {1,0}, {-2,0}, {1,-2}, {-2,1}},
        std::vector<Point>{{0,0}, {2,0}, {-1,0}, {2,1}, {-1,-2}},
        std::vector<Point>{{0,0}, {-1,0}, {2,0}, {-1,2}, {2,-1}},
        std::vector<Point>{{0,0}, {-2,0}, {1,0}, {-2,-1}, {1,2}},
    },
    {
        std::vector<Point>{{0,0}, {-1,0}, {2,0}, {-1,2}, {2,-1}},
        std::vector<Point>{{0,0}, {-2,0}, {1,0}, {-2,-1}, {1,2}},
        std::vector<Point>{{0,0}, {1,0}, {-2,0}, {1,-2}, {-2,1}},
        std::vector<Point>{{0,0}, {2,0}, {-1,0}, {2,1}, {-1,-2}},
    }
}};

void applyRotation(Tetromino& piece, const Grid& grid) {
    int from = piece.rotation;
    int to = (from + 1) % 4;

    Point center = piece.blocks[1]; // 中心旋转点
    std::vector<Point> rotated;
    for (const auto& b : piece.blocks) {
        int dx = b.x - center.x;
        int dy = b.y - center.y;
        rotated.push_back({ -dy + center.x, dx + center.y }); // 顺时针
    }

    if (piece.type == 0) return; // O 不旋转

    const KickTable* kickTable = (piece.type == 1) ? &I_KICK_TABLE : &JLSTZ_KICK_TABLE;
    const auto& kicks = (*kickTable)[from][to];

    for (const auto& offset : kicks) {
        bool valid = true;
        for (const auto& b : rotated) {
            int x = b.x + piece.pos.x + offset.x;
            int y = b.y + piece.pos.y + offset.y;
            if (x < 0 || x >= COLS || y < 0 || y >= ROWS || grid[y][x]) {
                valid = false;
                break;
            }
        }

        if (valid) {
            piece.blocks = rotated;
            piece.pos.x += offset.x;
            piece.pos.y += offset.y;
            piece.rotation = to;
            return;
        }
    }

    // 所有 kick 都失败，则不旋转
}
