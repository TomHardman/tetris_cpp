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
        int modTickCount_{};
        int ticksPerGravTick_;
        int frameLengthMs_;
        std::mt19937 rng_;
        Action actionBuffer_;
        std::optional<termios> old_settings_;
        std::mutex actionMtx_;
        std::atomic<bool> gameOverFlag_ = false;
        
        Tetromino createTetromino(Position position);
        void step();
        void render();
        void playLoop();
        void applyAction(Action action);
        void inputLoop();

    public:
        Game(int frameLengthMs, int ticksPerGravTick);
        ~Game();
        void play();
};