#pragma once
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

class PuzzlePiece {
  public:
    int col;
    int row;

    // Edges
    int top;
    int left;
    int bottom;
    int right;

    // Colors
    vector<vector<tuple<int, int, int>>> colors;

    PuzzlePiece();
    PuzzlePiece(int row, int col, int pieceSize);

    void initColor(int pixelResPerPiece);
    bool operator==(const PuzzlePiece &other) const;

    void write(std::ofstream& out) const;
    int read(const std::vector<char>& buffer, int offset);
};

// Hash function for (unordered) sets and (unordered) maps' keys
namespace std {
    template <> struct hash<PuzzlePiece> {
        size_t operator()(const PuzzlePiece &piece) const noexcept {
            return hash<int>()(piece.row) ^ hash<int>()(piece.col);
        }
    };
}
