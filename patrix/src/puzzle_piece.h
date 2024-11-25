#pragma once
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <vector>

using namespace std;

class PuzzlePiece {
public:
    unordered_map<string, int> edges;
    vector<vector<tuple<int, int, int>>> colors;

    int col;
    int row;

    PuzzlePiece(int row, int col, int pixelResPerPiece);

    bool operator==(const PuzzlePiece &other) const;

    void initColor(int pixelResPerPiece);


};

// Hash function for (unordered) sets and (unordered) maps' keys
namespace std {
    template <>
    struct hash<PuzzlePiece> {
        size_t operator()(const PuzzlePiece& piece) const noexcept {
            return hash<int>()(piece.row) ^ hash<int>()(piece.col);
        }
    };
}
