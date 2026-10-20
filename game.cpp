#include "game.h"
#include <atomic>
#include <thread>
#include <chrono>
#include <termios.h>
#include <unistd.h>
#include <random>
#include "tetrominos.h"
#include <iostream>

Game::Game(int frameLengthMs, int ticksPerGravTick) : 
    frameLengthMs_(frameLengthMs), 
    ticksPerGravTick_(ticksPerGravTick),
    rng_(std::random_device{}()) {
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
        Tetromino spawningTetromino = createTetromino(START_POS);
        liveTetromino_ = board_.tryEmplace(spawningTetromino);
        if (!liveTetromino_.has_value()) {
            gameOverFlag_.store(true, std::memory_order_release);
            return;
        }
    }
    else {
        Action action;
        {
            std::lock_guard<std::mutex> lock(actionMtx_);
            action = actionBuffer_;
            actionBuffer_ = Action::None;
        }
        applyAction(action);
        if (modTickCount_ == 0) { 
            // this is a gravity tick
            if (board_.fallBlocked(*liveTetromino_)){
                board_.lockPieceIntoBoard(*liveTetromino_);
                liveTetromino_.reset();
                score_ += board_.clearFilled();
            }
            // by applying gravity after lock check
            // player has one tick to perform any final rotations
            board_.tryTranslate(*liveTetromino_, 0, 1);
        }
    }
    modTickCount_ ++;
    modTickCount_ = modTickCount_ % ticksPerGravTick_;
}

void Game::render() {
    std::string boardRender = board_.renderWithTetromino(*liveTetromino_);
    std::cout << "\033[2J";  // Clear screen
    std::cout << "\033[H";   // Move cursor home
    std::cout << "Score: " << score_ << "\n";
    std::cout << boardRender;
    std::cout << std::endl;
    std::cout.flush();
}

void Game::inputLoop() {
    while(!gameOverFlag_.load(std::memory_order_acquire)) { // need to fully understand this
        char c;
        read(STDIN_FILENO, &c, 1);
        std::lock_guard<std::mutex> lock(actionMtx_);
        if (!(actionBuffer_==Action::None)) return;
        switch(c) {
            case 'a':
                actionBuffer_ = Action::Left;
                break;
            case 'd':
                actionBuffer_ = Action::Right;
                break;
            case 's':
                actionBuffer_ = Action::RotateCW;
                break;
            case 'w':
                actionBuffer_ = Action::RotateACW;
                break;
            case 'q':
                actionBuffer_ = Action::Quit;
                break;
            default: //noop
        }
    }
}

void Game::playLoop() {
    while(!gameOverFlag_) {
        this->step();
        this->render();
        std::this_thread::sleep_for(std::chrono::milliseconds(frameLengthMs_));
    }
}

void Game::play() {
    std::thread input(&Game::inputLoop, this);
    std::thread play(&Game::playLoop, this);
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