#include "components.h"
#include <mutex>
#include <atomic>
#include <optional>
#include <termios.h>
#include <unistd.h>
#include <random>

enum class Action {
    None,
    Left,
    Right,
    RotateCW,
    RotateACW,
    Quit
};

class Game {
    private:
        Board board_{};
        std::optional<Tetromino> liveTetromino_{};
        int score_{};
        int level_ = 1;
        int inputPeriodMs_;
        int gravityPeriodMs_ = 400;
        long long lastActionTimeMs_;
        std::mt19937 rng_;
        std::optional<termios> old_settings_;
        std::atomic<bool> gameOverFlag_ = false;
        
        Tetromino createTetromino(Position position);
        void step();
        void render();
        void gravityLoop();
        bool canApplyAction();
        long long nowMs();
        void applyAction(Action action);
        void inputLoop();

    public:
        Game(int inputFreq);
        ~Game();
        void play();
};