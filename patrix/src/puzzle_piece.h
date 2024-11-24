#include <iostream>
#include <unordered_map>
#include <fstream>

class PuzzlePiece {
public:
    std::unordered_map<std::string, int> edges;
    std::unordered_map<std::string, int[3]> colors;

    int col;
    int row;

    PuzzlePiece(int row, int col);
    bool operator==(const PuzzlePiece& other) const;
};

// Hash function for (unordered) sets and (unordered) maps' keys
namespace std {
    template <>
    struct hash<PuzzlePiece> {
        std::size_t operator()(const PuzzlePiece& piece) const noexcept {
            return std::hash<int>()(piece.row) ^ std::hash<int>()(piece.col);
        }
    };
}
