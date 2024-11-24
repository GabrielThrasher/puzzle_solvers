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
};
