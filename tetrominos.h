#include "components.h"


class SquareTetromino {
    public:
        static Tetromino create(Position position) {
            std::array<Position, 3> offsetPositions = {{{1, 0}, {0, 1}, {1, 1}}};
            Offsets offsets = Offsets(offsetPositions);
            return Tetromino(position, offsets);
        }
    };

class LineTetromino {
    public:
        static Tetromino create(Position position) {
            std::array<Position, 3> offsetPositions = {{{0, 1}, {0, 2}, {0, 3}}};
            Offsets offsets = Offsets(offsetPositions);
            return Tetromino(position, offsets);
        }
};

class LTetromino {
    public:
        static Tetromino create(Position position) {
            std::array<Position, 3> offsetPositions = {{{0, 1}, {1, 1}, {2, 1}}};
            Offsets offsets = Offsets(offsetPositions);
            return Tetromino(position, offsets);
        }
};

class STetromino {
    public:
        static Tetromino create(Position position) {
            std::array<Position, 3> offsetPositions = {{{1, 0}, {0, 1}, {-1, 1}}};
            Offsets offsets = Offsets(offsetPositions);
            return Tetromino(position, offsets);
        }
};

class ZTetromino {
    public:
        static Tetromino create(Position position) {
            std::array<Position, 3> offsetPositions = {{{-1, 0}, {0, 1}, {1, 1}}};
            Offsets offsets = Offsets(offsetPositions);
            return Tetromino(position, offsets);
        }
};

class TTetromino {
    public:
        static Tetromino create(Position position) {
            std::array<Position, 3> offsetPositions = {{{-1, 1}, {0, 1}, {1, 1}}};
            Offsets offsets = Offsets(offsetPositions);
            return Tetromino(position, offsets);
        }
};

class JTetromino {
    public:
        static Tetromino create(Position position) {
            std::array<Position, 3> offsetPositions = {{{0, 1}, {-1, 1}, {-2, 1}}};
            Offsets offsets = Offsets(offsetPositions);
            return Tetromino(position, offsets);
        }
};