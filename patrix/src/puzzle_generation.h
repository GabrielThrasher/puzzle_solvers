#include <iostream>
#include <random>
#include <unordered_map>
#include "puzzle_piece.h"
#include <functional>
#include <vector>
#include <unordered_set>
using namespace std;

class PuzzleGeneration{
    vector<vector<PuzzlePiece>> puzzle;
    vector<PuzzlePiece> puzzleRow;

    unordered_map<int, unordered_set<PuzzlePiece>> topEdges;
    unordered_map<int, unordered_set<PuzzlePiece>> leftEdges;
    unordered_map<int, unordered_set<PuzzlePiece>> bottomEdges;
    unordered_map<int, unordered_set<PuzzlePiece>> rightEdges;

    int cols = 424; // X dimension of puzzle
    int rows = 238; // Y dimension of puzzle
    int minColIdx = 0;
    int minRowIdx = 0;
    int maxColIdx = cols-1;
    int maxRowIdx = rows-1;

    int flatEdge = 44444444; // Octal value for a flat edge
    string topEdgeName = "topEdge";
    string leftEdgeName = "leftEdge";
    string bottomEdgeName = "bottomEdge";
    string rightEdgeName = "rightEdge";

    void generatePiece(int row, int col);
    int getEdge();
    int getUniqueEdge(unordered_map<int, unordered_set<PuzzlePiece>>* map);
    int getComplementEdge(int num);
    void updatePuzzleStorageMaps(int col);
    void printPuzzleStorageMapsSize();

public:
    void generatePuzzle();
    void savePuzzle();
};

