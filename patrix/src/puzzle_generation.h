#include <iostream>
#include <string>
#include <random>
#include <unordered_map>
#include "puzzle_piece.h"
#include "read_in_image.h"
#include <functional>
#include <vector>
#include <unordered_set>

using namespace std;

class PuzzleGeneration{
    vector<vector<PuzzlePiece>> puzzle;
    vector<PuzzlePiece> puzzleRow;

    image img;
    Mat rgbMatrix = img.readImage("PixelArt.png");

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
    int pixelResPerPiece = 2;

    int flatEdge = 44444444; // Octal value for a flat edge
    string topEdgeName = "topEdge";
    string leftEdgeName = "leftEdge";
    string bottomEdgeName = "bottomEdge";
    string rightEdgeName = "rightEdge";

    void generatePiece(int row, int col);
    void generateColor(int row, int col);


    int getEdge();
    int getUniqueEdge(unordered_map<int, unordered_set<PuzzlePiece>>* map);
    int getComplementEdge(int num);
    void updatePuzzleStorageMaps(int col);
    void printPuzzleStorageMapsSize();

    bool isValidColorIndex(int row, int col);

public:
    void generatePuzzle();
    void savePuzzle();
};

