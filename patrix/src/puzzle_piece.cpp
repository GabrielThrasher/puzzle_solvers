#include "puzzle_piece.h"

PuzzlePiece::PuzzlePiece(int row, int col) {
    this->col = col;
    this->row = row;
}

bool PuzzlePiece::operator==(const PuzzlePiece& other) const {
    return (this->row == other.row && this->col == other.col);
}