#include "components.h"
#include <atomic>
#include <optional>
#include <termios.h>
#include <unistd.h>
#include <random>

constexpr int INITIAL_GRAVITY_PERIOD_MS = 1000;
constexpr int INPUT_PERIOD_MS = 10; // minimum time between consecutively applied actions
constexpr std::array<int, 5> SCORE_BY_LINES_CLEARED = {0, 40, 100, 300, 1200};

enum class Action {
    None,
    Left,
    Right,
    RotateCW,
    RotateACW,
    Quit,
    IncreaseLevel
};

class Game {
    private:
        Board board_{};
        std::optional<Tetromino> liveTetromino_{};
        int score_{};
        int totLinesCleared_{};
        int level_ = 1;
        int baseLevel_ = 1; // this can be increased in sandbox mode
        int inputPeriodMs_ = INPUT_PERIOD_MS;
        int gravityPeriodMs_ = INITIAL_GRAVITY_PERIOD_MS;
        long long lastActionTimeMs_;
        bool sandboxMode_ = false;
        std::mt19937 rng_;
        std::optional<termios> old_settings_;
        std::atomic<bool> gameOverFlag_ = false;
        
        void step();
        void render();
        void gravityLoop();
        bool canApplyAction() const;
        long long nowMs() const;
        Tetromino createTetromino(Position position);
        void applyAction(Action action);
        void inputLoop();
        int calculateScore(int linesCleared) const;
        void updateGravity();
        void updateLevel();

    public:
        Game(bool sandboxMode);
        ~Game();
        void play();
};