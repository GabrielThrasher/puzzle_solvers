#pragma once
#include "puzzle.h"
#include "puzzle_piece.h"
#include "utilities.h"
#include <chrono>
#include <iostream>
#include <opencv2/core.hpp>
#include <random>
#include <string>
#include <unordered_map>
#include <unordered_set>
using namespace std;

using EdgeMap = unordered_map<int, unordered_set<PuzzlePiece *>>;
using ColorMap = unordered_map<int, unordered_map<int, PuzzlePiece *>>;

class Puzzle {
    // Puzzle properties
    int cols = 317; // X dimension of puzzle
    int rows = 317; // Y dimension of puzzle
    int pieceSize = 2;
    int flatEdge = 44444444; // Octal value for a flat edge
    vector<vector<PuzzlePiece *>> puzzle;

    // Storage by edge
    EdgeMap topEdges;
    EdgeMap bottomEdges;
    EdgeMap leftEdges;
    EdgeMap rightEdges;
    EdgeMap TESTrightEdges;

    // Storage by quadrant color and edge combos
    ColorMap topLeftQuadTopEdge;
    ColorMap topLeftQuadLeftEdge;
    ColorMap topRightQuadRightEdge;
    ColorMap bottomLeftQuadBottomEdge;
    ColorMap TESTbottomLeftQuadBottomEdge;

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
    void writePieceToFile(PuzzlePiece *element, ofstream &file);
    PuzzlePiece *getColorPiece(PuzzlePiece *piece, int idx, int edgeValue,
                               int startingHashRGBValue);
    tuple<int, int, int> getPixelRGB(PuzzlePiece *piece, int idx);

    cv::Mat readImage(string imagePath, int newWidth, int newHeight);
    void displayImage(string imagePath, cv::Mat &img);

    void edgeAlgorithm(string filename);
    void colorAlgorithm(string filename);

    void savePuzzleEdgeMap(string file, EdgeMap &map);
    void savePuzzleColorMap(string file, ColorMap &map);
    void loadPuzzleEdgeMap(string file, EdgeMap &map);
    void loadPuzzleColorMap(string file, ColorMap &map);

  public:
    Puzzle();
    void generate(string imagePath);
    void solve();
    void save();
    void load();
    // void Puzzle::save();
    // void Puzzle::load();
};
