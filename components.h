#pragma once

#include <stdexcept>
#include <utility>
#include <utility>
#include <optional>
#include <array>
#include <string>

using Position = std::pair<int, int>;
constexpr Position START_POS = {5, 0};

struct Offsets {
    std::array<Position, 3> offsets_;

    Offsets(std::array<Position, 3> offsets) {
        for (int i{}; i<3; i++) {
            Position pos = offsets[i];
            if (pos.second < 0) {
                throw std::runtime_error("y offset for tetromino shape must be +ve");
            }
            offsets_[i] = pos;
        }
    }

    auto begin() const { return offsets_.begin(); }
    auto end() const { return offsets_.end();}

    auto begin() { return offsets_.begin(); }
    auto end() { return offsets_.end();}
};

class Tetromino {
    private:
    void rotatePointClockwise(Position& offset);
       void rotatePointAntiClockwise(Position& offset);
    public:
        std::pair<int, int> position_;
        Offsets offsets_;
    
        Tetromino(Position position, Offsets offsets) : position_(position), offsets_(offsets) {};
        Tetromino& rotateClockwise();
        Tetromino& rotateAntiClockwise();
        Tetromino& translate(int dx, int dy);
};

class Board {
    private:
        static constexpr int M = 20;
        static constexpr int N = 10;
        std::array<std::array<int, N>, M> board_{};

        bool isValidPosition(const Tetromino&) const;
    
    public:
        bool fallBlocked(const Tetromino& tetromino) const;
        void lockPieceIntoBoard(const Tetromino& tetrominoContainer);
        std::optional<Tetromino> tryEmplace(const Tetromino& tetromino) const;
        Tetromino& tryRotateClockwise(Tetromino& tetromino) const;
        Tetromino& tryRotateAntiClockwise(Tetromino& tetromino) const;
        Tetromino& tryTranslate(Tetromino& tetromino, int dx, int dy) const;
        int clearFilled();
        std::string renderWithTetromino(const std::optional<Tetromino> tetromino) const;
};

