#include "puzzle_piece.h"

PuzzlePiece::PuzzlePiece(int row, int col, int pixelResPerPiece) {
    this->col = col;
    this->row = row;
    initColor(pixelResPerPiece);
}

bool PuzzlePiece::operator==(const PuzzlePiece& other) const {
    return (this->row == other.row && this->col == other.col);
}

void PuzzlePiece::initColor(int pixelResPerPiece) {
    std::tuple<int, int, int> arr = {-1, -1, -1};

    for (int i = 0; i < pixelResPerPiece + 2; i++) {
        vector<tuple<int, int, int>> row;
        for (int j = 0; j < pixelResPerPiece + 2; j++) {
            row.push_back(arr);
        }
        colors.push_back(row);
    }
}


