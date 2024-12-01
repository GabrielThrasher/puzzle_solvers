#pragma once
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

union Edge {
    struct {
        unsigned cell1 : 3;
        unsigned cell2 : 3;
        unsigned cell3 : 3;
        unsigned cell4 : 3;
        unsigned cell5 : 3;
        unsigned cell6 : 3;
        unsigned cell7 : 3;
        unsigned cell8 : 3;
    } cells;
    unsigned int value;
};

class PuzzlePiece {
    void initColor(int pixelResPerPiece);

  public:
    // Pixel colors of piece (main quads + jut out regins)
    vector<vector<tuple<int, int, int>>> colors;

    // Location in puzzle
    int col;
    int row;

    // Edge values
    // int top;
    // int bottom;
    // int left;
    // int right;

    Edge top;
    Edge bottom;
    Edge left;
    Edge right;

    PuzzlePiece(int row, int col, int pieceSize);
    PuzzlePiece();

    bool operator==(const PuzzlePiece &other) const;

    void write(std::ofstream &out) const;                  // Serialization
    int read(const std::vector<char> &buffer, int offset); // Deserialization
};

// Hash function for (unordered) sets' and (unordered) maps' keys
namespace std {
template <> struct hash<PuzzlePiece> {
    size_t operator()(const PuzzlePiece &piece) const noexcept {
        return hash<int>()(piece.row) ^ hash<int>()(piece.col);
    }
};
} // namespace std
