#include "puzzle.h"
#include "process_image.h"
#include "puzzle_piece.h"
#include <iostream>
#include <opencv2/core.hpp>
#include <random>
using namespace std;

Puzzle::Puzzle() : puzzle(rows, vector<PuzzlePiece *>(cols)) {
    std::cout << "Patrix 🧩" << std::endl;
}

void Puzzle::generateFromImage(string imagePath) {
    cout << "PUZZLE GENERATION" << endl;
    cout << "-------------------------------------" << endl;
    cout << "Starting puzzle generation..." << std::endl;
    cv::Mat rgbMatrix =
        readImage(imagePath, cols * pieceSize, rows * pieceSize);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            PuzzlePiece *piece = new PuzzlePiece(i, j, pieceSize);

            addEdges(i, j, piece);
            addColor(i, j, piece, rgbMatrix);
            updatePuzzleStorageMaps(piece);
            puzzle[i][j] = piece;
        }
    }
    cout << "Finished puzzle generation." << endl;
    cout << "-------------------------------------" << endl;
    printPuzzleStorageMapsSize();
}

void Puzzle::addEdges(int row, int col, PuzzlePiece *piece) {
    int minColIdx = 0;
    int minRowIdx = 0;
    int maxColIdx = cols - 1;
    int maxRowIdx = rows - 1;

    // Top left corner piece
    if (col == minColIdx && row == minRowIdx) {
        piece->top = flatEdge;
        piece->left = flatEdge;
        piece->bottom = getUniqueEdge(bottomEdges);
        piece->right = getUniqueEdge(rightEdges);
    }
    // Top right corner piece
    else if (col == maxColIdx && row == minRowIdx) {
        piece->top = flatEdge;
        piece->right = flatEdge;
        piece->left = getComplementEdge(puzzle[row][col - 1]->right);
        piece->bottom = getUniqueEdge(bottomEdges);
    }
    // Bottom left corner piece
    else if (col == minColIdx && row == maxRowIdx) {
        piece->left = flatEdge;
        piece->bottom = flatEdge;
        piece->top = getComplementEdge(puzzle[row - 1][col]->bottom);
        piece->right = getUniqueEdge(rightEdges);
    }
    // Bottom right corner piece
    else if (col == maxColIdx && row == maxRowIdx) {
        piece->bottom = flatEdge;
        piece->right = flatEdge;
        piece->top = getComplementEdge(puzzle[row - 1][col]->bottom);
        piece->left = getComplementEdge(puzzle[row][col - 1]->right);
    }
    // Top edge piece
    else if (row == minRowIdx) {
        piece->top = flatEdge;
        piece->left = getComplementEdge(puzzle[row][col - 1]->right);
        piece->bottom = getUniqueEdge(bottomEdges);
        piece->right = getUniqueEdge(rightEdges);
    }
    // Left edge piece
    else if (col == minColIdx) {
        piece->top = getComplementEdge(puzzle[row - 1][col]->bottom);
        piece->left = flatEdge;
        piece->bottom = getUniqueEdge(bottomEdges);
        piece->right = getUniqueEdge(rightEdges);
    }
    // Bottom edge piece
    else if (row == maxRowIdx) {
        piece->top = getComplementEdge(puzzle[row - 1][col]->bottom);
        piece->left = getComplementEdge(puzzle[row][col - 1]->right);
        piece->bottom = flatEdge;
        piece->right = getUniqueEdge(rightEdges);
    }
    // Right edge piece
    else if (col == maxColIdx) {
        piece->top = getComplementEdge(puzzle[row - 1][col]->bottom);
        piece->left = getComplementEdge(puzzle[row][col - 1]->right);
        piece->bottom = getUniqueEdge(bottomEdges);
        piece->right = flatEdge;
    }
    // Middle piece
    else {
        piece->top = getComplementEdge(puzzle[row - 1][col]->bottom);
        piece->left = getComplementEdge(puzzle[row][col - 1]->right);
        piece->bottom = getUniqueEdge(bottomEdges);
        piece->right = getUniqueEdge(rightEdges);
    }
}

int Puzzle::getEdge() {
    string octalStr;
    random_device rd;
    unsigned long seed = rd();
    mt19937 engine(seed);

    discrete_distribution<> dist{
        {0, 1, 1, 0, 1, 1, 1}}; // Distribution  {1, 2, 4, 5, 6}

    auto rng = bind(dist, ref(engine));

    for (int sideIdx = 0; sideIdx < 8; sideIdx++) {
        int randNum = rng();
        octalStr += to_string(randNum);
    }

    int octalNum = stoi(octalStr);

    return octalNum;
}

int Puzzle::getUniqueEdge(EdgeMap &map) {
    int edge = getEdge();
    auto iter = map.find(edge);
    // Keep generating a new edge octal value for as long as it is already being
    // used in a same-side edge OR is the same as the flat edge value
    while (iter != map.end() || edge == flatEdge) {
        edge = getEdge();
        iter = map.find(edge);
    }

    return edge;
}

int Puzzle::getComplementEdge(int num) {
    string str = to_string(num);
    string comp = "";
    unordered_map<char, char> compMap = {
        {'1', '5'}, {'2', '6'}, {'4', '4'}, {'6', '2'}, {'5', '1'},
    };

    for (auto chr : str) {
        comp += compMap[chr];
    }

    return stoi(comp);
}

void Puzzle::addColor(int row, int col, PuzzlePiece *piece,
                      cv::Mat &rgbMatrix) {
    // Matrix indices for the puzzle matrix
    int baseMartixRowIdx = 1;
    int baseMartixColIdx = 1;

    for (int i = -1; i <= pieceSize; i++) {
        for (int j = -1; j <= pieceSize; j++) {
            // Color indices for the rgbMatrix created from the image
            int colorRowIdx = pieceSize * row + i;
            int colorColIdx = pieceSize * col + j;

            if (isValidColorIdx(colorRowIdx, colorColIdx) &&
                isValidMatirxIdx(i, j)) {
                cv::Vec3b pixel =
                    rgbMatrix.at<cv::Vec3b>(colorRowIdx, colorColIdx);
                int red = pixel[2], green = pixel[1], blue = pixel[0];
                piece->colors[baseMartixRowIdx + i][baseMartixColIdx + j] = {
                    red, green, blue};
            }
        }
    }
}

void Puzzle::updatePuzzleStorageMaps(PuzzlePiece *piece) {
    // Update edge storage
    topEdges[piece->top].insert(piece);
    leftEdges[piece->left].insert(piece);
    bottomEdges[piece->bottom].insert(piece);
    rightEdges[piece->right].insert(piece);

    // Update color storage
    topLeftQuadColors[hashRGBValues(piece->colors[1][1])].insert(piece);
    topRightQuadColors[hashRGBValues(piece->colors[1][2])].insert(piece);
    bottomLeftQuadColors[hashRGBValues(piece->colors[2][1])].insert(piece);
    bottomRightQuadColors[hashRGBValues(piece->colors[2][2])].insert(piece);
}

int Puzzle::hashRGBValues(tuple<int, int, int> rgb) {
    string hashValue = to_string(get<0>(rgb)) + to_string(get<1>(rgb)) +
                       to_string(get<2>(rgb));

    return stoi(hashValue);
}

void Puzzle::printPuzzleStorageMapsSize() {
    // Dev fcn to verify that the puzzle storage maps were created properly
    cout << "SIZE INFO ABOUT ALL MAPS" << endl;
    cout << "-------------------------------------" << endl;

    // Display the size of all edge maps
    unordered_map<string, unordered_map<int, unordered_set<PuzzlePiece *>>>
        edgeMaps = {{"topEdges", topEdges},
                    {"leftEdges", leftEdges},
                    {"bottomEdges", bottomEdges},
                    {"rightEdges", rightEdges}};
    for (auto ele : edgeMaps) {
        string name = ele.first;
        unordered_map<int, unordered_set<PuzzlePiece *>> map = ele.second;

        std::cout << name << " map: " << map.size() - 1 << " unique edges + "
                  << map[flatEdge].size()
                  << " flat edges = " << (map.size() + map[flatEdge].size() - 1)
                  << " edges." << std::endl;
    }

    // Display the size of all color maps
    unordered_map<string, unordered_map<int, unordered_set<PuzzlePiece *>>>
        colorMaps = {{"topLeftQuadColors", topLeftQuadColors},
                     {"topRightQuadColors", topRightQuadColors},
                     {"bottomLeftQuadColors", bottomLeftQuadColors},
                     {"bottomRightQuadColors", bottomRightQuadColors}};
    for (auto ele : colorMaps) {
        string name = ele.first;
        unordered_map<int, unordered_set<PuzzlePiece *>> map = ele.second;

        std::cout << name << " map: " << map.size() << " unique colors."
                  << std::endl;
    }

    cout << "-------------------------------------" << endl;
}

bool Puzzle::isValidColorIdx(int row, int col) {
    return ((row >= 0 && row < pieceSize * rows) &&
            (col >= 0 && col < pieceSize * cols));
}

bool Puzzle::isValidMatirxIdx(int row, int col) {
    return ((row >= 0 || col >= 0) && (row < pieceSize || col < pieceSize));
}

void Puzzle::EdgeAlgorithm() {
    //Locate top left corner piece: Has two flat edges
    auto set1 = topEdges[flatEdge];
    auto set2 = leftEdges[flatEdge];
    auto set3 = rightEdges[flatEdge];
    auto set4 = bottomEdges[flatEdge];
    int complement1;  //Complement of right edge: Left edge
    int complement2;  //Complement of bottom edge: Top edge
    int complement3;  //Complement of left edge: Right edge
    int complement4;  //Complement of top edge: Bottom edge

    //For every element in the set of flat tops
    for (auto element: set1) {
        //If that element can be found in the set of flat lefts
        if (set2.find(element) != set2.end()) {
            //Write to file location + rgb values
            complement1 = getComplementEdge(element->right);
            //cout << "Row: " << element->row << "Col: " << element->col << endl;
            break;
        }
    }

    //Start at col = 1
    //Fetch complement: repeatedly get left edge of adjacent piece
    for (int i = 1; i < cols; i++) {
        //Index leftEdges map with the complement-> returns set of all possible pieces
        auto leftSet = leftEdges[complement1];
        //For every element in the left set with that particular edge
        for(auto element: leftSet) {
            //If that element can be found in the set of flat tops
            if (set1.find(element) != set1.end()) {
                //Write to file location + rgb values
                complement1 = getComplementEdge(element->right);
                //If i is the index of the last puzzle piece
                if(i == cols - 1) {
                    //Get the complement of the bottom edge to prepare for vertical portion of algorithm
                    complement2 = getComplementEdge(element->bottom);
                }
                //cout << "Row: " << element->row << "Col: " << element->col << endl;
            }
        }
    }

    //Start at row = 1
    for (int i = 1; i < rows; i++) {
        //Index topEdges map with the complement-> returns set of all possible pieces
        auto topSet = topEdges[complement2];
        //For every element in the top set with that particular edge
        for(auto element: topSet) {
            //If that element can be found in the set of flat rights
            if (set3.find(element) != set3.end()) {
                //Write to file location + rgb values
                complement2 = getComplementEdge(element->bottom);
                //If i is the index of the bottommost puzzle piece
                if (i == rows - 1) {
                    //Get complement of left edge to prepare for right to left portion of algorithm
                    complement3 = getComplementEdge(element->left);
                }
                //cout << "Row: " << element->row << "Col: " << element->col << endl;
            }
        }
    }

    //Start at col = 315
    for (int i = cols - 2; i >= 0; i--) {
        //Index rightEdges map with the complement-> returns set of all possible pieces
        auto rightSet = rightEdges[complement3];
        //For every element in the right set with that particular edge
        for(auto element: rightSet) {
            //If that element can be found in the set of flat bottoms
            if (set4.find(element) != set4.end()) {
                //Write to file location + rgb values
                complement3 = getComplementEdge(element->left);
                //If i is the index of the bottommost puzzle piece
                if (i == 0) {
                    //Get complement of top edge to prepare for bottom to top portion of algorithm
                    complement4 = getComplementEdge(element->top);
                }
                //cout << "Row: " << element->row << "Col: " << element->col << endl;
            }
        }
    }

    //Start at row = 315
    for (int i = rows - 2; i > 0; i--) {
        //Index bottomEdges map with the complement-> returns set of all possible pieces
        auto bottomSet = bottomEdges[complement4];
        //For every element in the bottom set with that particular edge
        for(auto element: bottomSet) {
            //If that element can be found in the set of flat lefts
            if (set2.find(element) != set2.end()) {
                //Write to file location + rgb values
                complement4 = getComplementEdge(element->top);
                //If i is the index of the bottommost puzzle piece
                if (i == 1) {
                    //Get complement of right edge to prepare for another left to right portion of algorithm
                    complement1 = getComplementEdge(element->right);
                }
                //cout << "Row: " << element->row << "Col: " << element->col << endl;
            }
        }
    }


}

