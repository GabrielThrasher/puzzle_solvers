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
    //This will fetch top left corner piece first
    int leftComplement;
    int topComplement;
    int rightComplement;
    int bottomComplement;
    int leftIdx = 0;
    int rightIdx = cols;
    int topIdx = 1;
    int bottomIdx = rows;
    bool isStarting = true;
    int counter;
    bool isOdd = false;

    if (rows % 2 == 0) {
        counter = rows/2;
        isOdd = false;
    }
    else {
        counter = (rows-1)/2;
        isOdd = true;
    }

    while(counter > 0) {
        for (int i = leftIdx; i < rightIdx; i++) {
            if (isStarting) {
                //Locate top left corner piece
                for (auto element: topEdges[flatEdge]) {
                    if (leftEdges[flatEdge].find(element) != leftEdges[flatEdge].end()) {
                        //Write to file location + rgb values
                        cout << "Row: " << element->row << "Col: " << element->col << endl;
                        leftComplement = getComplementEdge(element->right);
                        isStarting = false;
                    }
                }
                continue;
            }
            //Index leftEdges map with the complement
            auto leftSet = leftEdges[leftComplement];
            auto element = leftSet.begin();

            //Write to file location + rgb values

            leftComplement = getComplementEdge((*element)->right);
            //If i is the index of the last puzzle piece
            if (i == rightIdx - 1) {
                //Get the complement of the bottom edge to prepare for top to down portion of algorithm
                topComplement = getComplementEdge((*element)->bottom);
            }
            cout << "Row: " << (*element)->row << "Col: " << (*element)->col << endl;
        }

        //Start at row = 1
        for (int i = topIdx; i < bottomIdx; i++) {
            //Index topEdges map with the complement-> returns set of all possible pieces
            auto topSet = topEdges[topComplement];
            auto element = topSet.begin();
            //Write to file location + rgb values
            topComplement = getComplementEdge((*element)->bottom);
            //If i is the index of the bottommost puzzle piece
            if (i == bottomIdx - 1) {
                //Get complement of left edge to prepare for right to left portion of algorithm
                rightComplement = getComplementEdge((*element)->left);
            }
            cout << "Row: " << (*element)->row << "Col: " << (*element)->col << endl;
        }

        //Start at col = 315
        for (int i = rightIdx - 2; i >= leftIdx; i--) {
            //Index rightEdges map with the complement
            auto rightSet = rightEdges[rightComplement];
            auto element = rightSet.begin();
            //Write to file location + rgb values
            rightComplement = getComplementEdge((*element)->left);
            //If i is the index of the bottommost puzzle piece
            if (i == leftIdx) {
                //Get complement of top edge to prepare for bottom to top portion of algorithm
                bottomComplement = getComplementEdge((*element)->top);
            }
            cout << "Row: " << (*element)->row << "Col: " << (*element)->col << endl;
        }

        //Start at row = 315
        for (int i = bottomIdx - 2; i >= topIdx; i--) {
            //Index bottomEdges map with the complement-> returns set of all possible pieces
            auto bottomSet = bottomEdges[bottomComplement];
            auto element = bottomSet.begin();
            //Write to file location + rgb values
            bottomComplement = getComplementEdge((*element)->top);
            //If i is the index of the bottommost puzzle piece
            if (i == topIdx) {
                //Get complement of right edge to prepare for another left to right portion of algorithm
                leftComplement = getComplementEdge((*element)->right);
            }
            cout << "Row: " << (*element)->row << "Col: " << (*element)->col << endl;
        }

        leftIdx++;
        rightIdx--;
        topIdx++;
        bottomIdx--;
        counter -= 2;
    }

    if (isOdd) {
        auto lastSet = leftEdges[leftComplement];
        auto element = lastSet.begin();
        //Write to file location + rgb values
        cout << "Row: " << (*element)->row << "Col: " << (*element)->col << endl;

    }
}

