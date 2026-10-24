#include <iostream>
#include "game.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Invalid arguments. Usage: tetris <inputFreq>\n";
        return 1;
    }
    int inputFreq = std::stoi(argv[1]);
    Game game(inputFreq);
    game.play();
    return 0;
}
    