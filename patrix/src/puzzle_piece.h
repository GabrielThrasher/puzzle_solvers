#pragma once
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

class PuzzlePiece {
  public:
    // Pixel colors of piece (main quads + jut out regins)
    vector<vector<tuple<int, int, int>>> colors;

    // Location in puzzle
    int col;
    int row;

    // Edge values
    int top;
    int bottom;
    int left;
    int right;

    PuzzlePiece(int row, int col, int pieceSize);
    PuzzlePiece();

    void initColor(int pixelResPerPiece);
    bool operator==(const PuzzlePiece &other) const;

    void PuzzlePiece::write(std::ofstream& out) const; // Serialization
    int PuzzlePiece::read(const std::vector<char>& buffer, int offset); // Deserialization
};

// Hash function for (unordered) sets' and (unordered) maps' keys
namespace std {
    template <> struct hash<PuzzlePiece> {
        size_t operator()(const PuzzlePiece &piece) const noexcept {
            return hash<int>()(piece.row) ^ hash<int>()(piece.col);
        }
    };
}
