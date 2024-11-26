#pragma once
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

class PuzzlePiece {
  public:
    vector<vector<tuple<int, int, int>>> colors;

    int col;
    int row;

    // edges
    int top;
    int bottom;
    int left;
    int right;

    PuzzlePiece(int row, int col, int pieceSize);

    void initColor(int pixelResPerPiece);
    bool operator==(const PuzzlePiece &other) const;
};

// Hash function for (unordered) sets and (unordered) maps' keys
namespace std {
template <> struct hash<PuzzlePiece> {
    size_t operator()(const PuzzlePiece &piece) const noexcept {
        return hash<int>()(piece.row) ^ hash<int>()(piece.col);
    }
};
} // namespace std
