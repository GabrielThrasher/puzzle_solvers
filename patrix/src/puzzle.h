#pragma once
#include "puzzle_piece.h"
#include <opencv2/core.hpp>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <chrono>
using namespace std;

using EdgeMap = unordered_map<int, unordered_set<PuzzlePiece *>>;
using ColorMap = unordered_map<int, unordered_set<PuzzlePiece *>>;

class Puzzle {
    // Puzzle properties
    int cols = 317; // X dimension of puzzle
    int rows = 317; // Y dimension of puzzle
    int pieceSize = 2;
    int flatEdge = 44444444; // Octal value for a flat edge

    // underlying 2D 8x8 matrix of the puzzle
    vector<vector<PuzzlePiece *>> puzzle;

    // Storage by edge
    EdgeMap topEdges;
    EdgeMap bottomEdges;
    EdgeMap leftEdges;
    EdgeMap rightEdges;

    // Storage by quadrant color
    ColorMap topLeftQuadColors;
    ColorMap topRightQuadColors;
    ColorMap bottomLeftQuadColors;
    ColorMap bottomRightQuadColors;

    void addEdges(int row, int col, PuzzlePiece *piece);
    void addColor(int row, int col, PuzzlePiece *piece, cv::Mat &rgbMatrix);

    int getUniqueEdge(EdgeMap &map);
    int getEdge();
    int getComplementEdge(int num);

    void updatePuzzleStorageMaps(PuzzlePiece *piece);
    void printPuzzleStorageMapsSize();
    int hashRGBValues(tuple<int, int, int> rgb);

    bool isValidColorIdx(int row, int col);
    bool isValidMatirxIdx(int row, int col);

    void WriteToFile(PuzzlePiece* element, ofstream& file);

  public:
    Puzzle();

    void generateFromImage(string imagePath);
    void EdgeAlgorithm(string filename);

};


