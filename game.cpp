#include "game.h"
#include <atomic>
#include <string>
#include <thread>
#include <chrono>
#include <termios.h>
#include <unistd.h>
#include <random>
#include "tetrominos.h"
#include "logging.h"
#include <iostream>

Game::Game(int inputFreq) 
    : rng_(std::random_device{}()) 
{
    inputPeriodMs_ = 1000 / inputFreq;

    // allow us to use stdin to receive game input
    tcgetattr(STDIN_FILENO, &*old_settings_);
    termios new_settings = *old_settings_;
    new_settings.c_lflag &= ~(ICANON | ECHO);
    new_settings.c_cc[VMIN] = 1;
    new_settings.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);
}

Game::~Game() {
    if (old_settings_.has_value()) tcsetattr(STDIN_FILENO, TCSANOW, &*old_settings_);
}

void Game::step() {
    if (!liveTetromino_.has_value()) {
        LOG("Spawning new tetromino");
        Tetromino spawningTetromino = createTetromino(START_POS);
        LOG("Spawned tetromino")
        liveTetromino_ = board_.tryEmplace(spawningTetromino);
        if (!liveTetromino_.has_value()) {
            gameOverFlag_.store(true, std::memory_order_release);
            return;
        }
    }
    else {
            if (board_.fallBlocked(*liveTetromino_)){
                LOG("Locking piece and clearing filled rows");
                board_.lockPieceIntoBoard(*liveTetromino_);
                LOG("Clearing tetromino");
                liveTetromino_.reset();
                score_ += board_.clearFilled();
                level_ = 1 + score_ / 1000;
                double gravityPeriodS = std::pow((0.8 - (level_ - 1) * 0.007), level_ - 1);
                gravityPeriodMs_ = gravityPeriodS * 1000;
                return;
            }
            // by applying gravity after lock check
            // player has one tick to perform movement/rotation
            LOG("Apply gravity")
            board_.tryTranslate(*liveTetromino_, 0, 1);
    }
}

void Game::render() {
    std::string boardRender = board_.renderWithTetromino(liveTetromino_);
    std::cout << "\033[2J";  // Clear screen
    std::cout << "\033[H";   // Move cursor home
    std::cout << boardRender;
    std::cout << "Score: " << score_ << "\n";
    std::cout << "Level:" << level_ << "\n";
    std::cout << std::endl;
    std::cout.flush();
}

long long Game::nowMs() {
    using namespace std::chrono;
     return duration_cast<milliseconds>(
        system_clock::now().time_since_epoch()
    ).count();
}

bool Game::canApplyAction() {
    return nowMs() > lastActionTimeMs_ + inputPeriodMs_;
}

void Game::inputLoop() {
    while(!gameOverFlag_.load(std::memory_order_acquire)) { // need to fully understand this
        char c;
        read(STDIN_FILENO, &c, 1);
        LOG("Received keypress " + std::string{c})
        if (!canApplyAction()) continue;
        
        LOG("Applying action for keypress " + std::string{c});
        switch(c) {
            case 'a':
                applyAction(Action::Left);
                break;
            case 'd':
                applyAction(Action::Right);
                break;
            case 's':
                applyAction(Action::RotateCW);
                break;
            case 'w':
                applyAction(Action::RotateACW);
                break;
            case 'q':
                applyAction(Action::Quit);
                break;
            default: //noop
        }
        lastActionTimeMs_ = nowMs();
        render();
    }
}

void Game::gravityLoop() {
    while(!gameOverFlag_) {
        this->step();
        this->render();
        std::this_thread::sleep_for(std::chrono::milliseconds(gravityPeriodMs_));
    }
}

void Game::play() {
    std::thread input(&Game::inputLoop, this);
    std::thread play(&Game::gravityLoop, this);
    input.join();
    play.join();
}

Tetromino Game::createTetromino(Position position) {
    int rand  = std::uniform_int_distribution<int>(0, 6)(rng_);
    switch (rand) {
        case 0 : return SquareTetromino::create(position);
        case 1 : return LineTetromino::create(position);
        case 2 : return LTetromino::create(position);
        case 3 : return STetromino::create(position);
        case 4 : return ZTetromino::create(position);
        case 5 : return TTetromino::create(position);
        case 6 : return JTetromino::create(position);
        default: throw std::logic_error("Unexpected random number generated " + std::to_string(rand));
    }
}

void Game::applyAction(Action action) {
    switch(action) {
        case Action::Left :
            if(liveTetromino_.has_value()) board_.tryTranslate(*liveTetromino_, -1, 0);
            break;
        case Action::Right :
            if(liveTetromino_.has_value()) board_.tryTranslate(*liveTetromino_, 1, 0);
            break;
        case Action::RotateCW :
            if(liveTetromino_.has_value()) board_.tryRotateClockwise(*liveTetromino_);
            break;
        case Action::RotateACW :
            if(liveTetromino_.has_value())  board_.tryRotateAntiClockwise(*liveTetromino_);
            break;
        case Action::Quit :
            gameOverFlag_.store(true, std::memory_order_release);
            break;
        default: //noop
    }
}