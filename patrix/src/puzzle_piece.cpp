#include "puzzle_piece.h"

PuzzlePiece::PuzzlePiece(int row, int col, int pieceSize) {
    this->col = col;
    this->row = row;
    initColor(pieceSize);
}

bool PuzzlePiece::operator==(const PuzzlePiece &other) const {
    return (this->row == other.row && this->col == other.col);
}

void PuzzlePiece::initColor(int pieceSize) {
    std::tuple<int, int, int> arr = {-1, -1, -1};

    for (int i = 0; i < pieceSize + 2; i++) {
        vector<tuple<int, int, int>> row;
        for (int j = 0; j < pieceSize + 2; j++) {
            row.push_back(arr);
        }
        colors.push_back(row);
    }
}
