#include <iostream>
#include "game.h"
#include <string>
#include <unordered_map>
#include "logging.h"

std::unordered_map<std::string, std::string> OPTIONS = {
    {"-s", "Sandbox mode - can increase level with u"}
};

std::string optionsMessage() {
    std::string msg = "Options:\n";
    for (auto entry : OPTIONS) {
        msg += entry.first + ": " + entry.second + "\n";
    }
    return msg;
}

int main(int argc, char* argv[]) {
    LOG("Beginning tetris.cpp")
    if (argc > OPTIONS.size() + 1) {
        std::cout << "Invalid arguments. Usage: tetris [OPTIONS]" << std::endl;
        std::cout << "\n" << optionsMessage() << std::endl;
        return 1;
    }
    bool sandboxMode = false;
    for (int i=1; i < argc; i++) {
        if (!OPTIONS.contains(argv[i])) {
            std::cout << "Unrecognised option " << std::string(argv[i]) << std::endl;
            std::cout << "\n" << optionsMessage() << std::endl;
            return 1;
        }
        switch(argv[i][1]) {
            case 's':
                sandboxMode = true;
        }
    }
    Game game(sandboxMode);
    game.play();
    return 0;
}
    