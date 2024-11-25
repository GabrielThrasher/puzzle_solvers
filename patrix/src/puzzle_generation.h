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
    cv::Mat rgbMatrix;

    // Storage by edge
    unordered_map<int, unordered_set<PuzzlePiece>> topEdges;
    unordered_map<int, unordered_set<PuzzlePiece>> leftEdges;
    unordered_map<int, unordered_set<PuzzlePiece>> bottomEdges;
    unordered_map<int, unordered_set<PuzzlePiece>> rightEdges;

    // Storage by quadrant color
    unordered_map<int, unordered_set<PuzzlePiece>> topLeftQuadColors;
    unordered_map<int, unordered_set<PuzzlePiece>> topRightQuadColors;
    unordered_map<int, unordered_set<PuzzlePiece>> bottomLeftQuadColors;
    unordered_map<int, unordered_set<PuzzlePiece>> bottomRightQuadColors;

    int cols = 424; // X dimension of puzzle
    int rows = 238; // Y dimension of puzzle
    int minColIdx = 0;
    int minRowIdx = 0;
    int maxColIdx = cols-1;
    int maxRowIdx = rows-1;
    // How many pixels wide (and tall) per piece; higher value = greater image detail and more screen space used
    int pixelResPerPiece = 2;

    int flatEdge = 44444444; // Octal value for a flat edge
    string topEdgeName = "topEdge";
    string leftEdgeName = "leftEdge";
    string bottomEdgeName = "bottomEdge";
    string rightEdgeName = "rightEdge";

    void addEdges(int row, int col);
    void addColor(int row, int col);
    int getEdge();
    int getUniqueEdge(unordered_map<int, unordered_set<PuzzlePiece>>* map);
    int getComplementEdge(int num);
    void updatePuzzleStorageMaps(int col);
    void printPuzzleStorageMapsSize();
    bool isValidColorIdx(int row, int col);
    bool isValidMatirxIdx(int row, int col);
    int hashRGBValues(tuple<int, int, int> rgb);

public:
    void generatePuzzle();
    void savePuzzle();
};

