#include "game.hpp"

int main() {
    std::srand(static_cast<unsigned>(time(0)));
    Game game;
    game.run();
}
