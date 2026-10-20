#include "components.h"
#include <chrono>
#include <thread>
#include <utility>
#include <iostream>
#include <string>
#include <thread>

void Tetromino::rotatePointClockwise(Position& point) {
        int dx = point.first;
        int dy = point.second;
        std::swap(dx, dy);
        dx = -dx;
        point.first = dx;
        point.second = dy;
        
    }

Tetromino& Tetromino::rotateClockwise() {
     for (Position& offset : offsets_) {
        rotatePointClockwise(offset);
    }
    return *this;
}

void Tetromino::rotatePointAntiClockwise(Position& point) {
        int dx = point.first;
        int dy = point.second;
        std::swap(dx, dy);
        dy = -dy;
        point.first = dx;
        point.second = dy;
}

Tetromino& Tetromino::rotateAntiClockwise() {
     for (Position& offset : offsets_) {
        rotatePointAntiClockwise(offset);
    }
    return *this;
}

Tetromino& Tetromino::translate(int dx, int dy) {
    position_.first += dx;
    position_.second += dy;
    return *this;
}

bool Board::isValidPosition(const Tetromino& tetromino) const {
    Position centre = tetromino.position_;

    auto isPointValid = [this](Position point){
        if (point.second < 0 || point.second >= M || point.first < 0 || point.first >= N){
            return false;
        }
        if (board_[point.second][point.first] == 1){
            return false;
        }
        return true;
    };

    if (!isPointValid(centre)) {
        return false;
    }

    for (Position offset : tetromino.offsets_) {
        Position pos = {centre.first + offset.first, centre.second + offset.second};
        if (!isPointValid(pos)) return false;
    }
    return true;
}

bool Board::fallBlocked(const Tetromino& tetromino) const {
    Position centre = tetromino.position_;

    auto checkSquareConnected = [this](Position point){
        if (point.second >= M-1) return true;
        if(board_[point.second+1][point.first] == 1) return true;
        return false;
    };

    if (checkSquareConnected(centre)) return true;
    for (Position offset : tetromino.offsets_) {
        Position pos = {centre.first + offset.first, centre.second + offset.second};
        if (checkSquareConnected(pos)) return true;
    }
    return false;
}

void Board::lockPieceIntoBoard(const Tetromino& tetromino){
    Position centre = (tetromino).position_;
     board_[centre.second][centre.first] = 1;

    for (Position offset : (tetromino).offsets_) {
        Position pos = {centre.first + offset.first, centre.second + offset.second};
        board_[pos.second][pos.first] = 1;
    }
}

std::optional<Tetromino> Board::tryEmplace(const Tetromino& tetromino) const {
    std::optional<Tetromino> maybeTetromino{};
    if (isValidPosition(tetromino)) {
        maybeTetromino.emplace(tetromino);
    }
    return maybeTetromino;

}

Tetromino& Board::tryRotateClockwise(Tetromino& tetromino) const {
    tetromino.rotateClockwise();
    if (!isValidPosition(tetromino)) {
        tetromino.rotateAntiClockwise();
    }
    return tetromino;
}

Tetromino& Board::tryRotateAntiClockwise(Tetromino& tetromino) const {
    tetromino.rotateAntiClockwise();
    if (!isValidPosition(tetromino)) {
        tetromino.rotateClockwise();
    }
    return tetromino;
}

Tetromino& Board::tryTranslate(Tetromino& tetromino, int dx, int dy) const {
    tetromino.translate(dx, dy);
    if (!isValidPosition(tetromino)) {
        tetromino.translate(-dx, -dy);
    }
    return tetromino;
}

int Board::clearFilled() {
    auto clearRow = [this](int rowIdx){
        for (int colIdx{}; colIdx < N; colIdx++) {
            board_[rowIdx][colIdx] = 0;
        }
    };
    auto rowFilled = [this](int rowIdx){
        for (int colIdx{}; colIdx < N; colIdx++) {
            if (board_[rowIdx][colIdx] == 0) return false;
        }
        return true;
    };
    auto shiftRowsDown = [this, clearRow](int baseIdx){
        for (int rowIdx = baseIdx; rowIdx >= 1; rowIdx--) {
            board_.at(rowIdx) = board_.at(rowIdx-1);
        }
        clearRow(0);
    };

    int top = 0;
    for (int rowIdx = M-1; rowIdx > top; rowIdx--){
        if (rowFilled(rowIdx)){
            clearRow(rowIdx);
            shiftRowsDown(rowIdx);
            top++;
        }
    }
    // top = lines cleared
    switch (top) {
        case 1: return 100;
        case 2: return 200;
        case 3: return 300;
        case 4: return 400;
        default: return 0;
    }
}

std::string Board::renderWithTetromino(const Tetromino& tetromino) const {
    std::string result;
    std::array<std::array<int, N>, M> boardCopy = board_;

    Position centre = tetromino.position_;
    boardCopy[centre.second][centre.first] = 2;
    for (Position offset : tetromino.offsets_) {
        Position pos  = {centre.first + offset.first, centre.second + offset.second};
        boardCopy[pos.second][pos.first] = 2;
    }

    for (const auto& row : boardCopy) {
        for (int cell : row) {
            switch(cell){
                case 0: 
                    result += ". ";
                    break;
                case 1: 
                    result += "■ ";
                    break;
                case 2: 
                    result += "# ";
                    break;
                default: throw std::runtime_error("Invalid cell value: " + std::to_string(cell));
            }
        }
        result += '\n';
    }
    return result;
}