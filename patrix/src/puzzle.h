#pragma once
#include "puzzle_piece.h"
#include <opencv2/core.hpp>
#include <string>
#include <unordered_map>
#include <unordered_set>
using namespace std;
#include <chrono>

using StorageMap = unordered_map<int, unordered_set<PuzzlePiece>>;

class Puzzle {
    // Puzzle properties
    int cols = 317; // X dimension of puzzle
    int rows = 317; // Y dimension of puzzle
    int pieceSize = 2;
    int flatEdge = 44444444; // Octal value for a flat edge

    // underlying 2D 8x8 matrix of the puzzle
    vector<vector<PuzzlePiece*>> puzzle;

    // Storage by edge
    StorageMap topEdges;
    StorageMap bottomEdges;
    StorageMap leftEdges;
    StorageMap rightEdges;

    // Storage by quadrant color
    StorageMap topLeftQuadColors;
    StorageMap topRightQuadColors;
    StorageMap bottomLeftQuadColors;
    StorageMap bottomRightQuadColors;

    void addEdges(int row, int col, PuzzlePiece *piece);
    void addColor(int row, int col, PuzzlePiece *piece, cv::Mat &rgbMatrix);

    int getUniqueEdge(StorageMap &map);
    int getEdge();
    int getComplementEdge(int num);

    void updatePuzzleStorageMaps(PuzzlePiece* piecePtr);
    void printPuzzleStorageMapsSize();
    int hashRGBValues(tuple<int, int, int> rgb);

    bool isValidColorIdx(int row, int col);
    bool isValidMatirxIdx(int row, int col);

    void savePuzzleStorageMap(string file, StorageMap& map);
    void loadPuzzleStorageMap(string file, StorageMap& map);

  public:
    Puzzle();

    void generate(string imagePath);
    void EdgeAlgorithm();
    void save();
    void load();
};


