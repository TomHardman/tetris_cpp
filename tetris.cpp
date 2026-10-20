#include <iostream>
#include "game.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "Invalid arguments. Usage: tetris <frameLengthMs> <ticksPerGravTick>\n";
        return 1;
    }
    int frameLengthMs = std::stoi(argv[1]);
    int ticksPerGravTick = std::stoi(argv[2]);
    Game game(frameLengthMs, ticksPerGravTick);
    game.play();
    return 0;
}
    