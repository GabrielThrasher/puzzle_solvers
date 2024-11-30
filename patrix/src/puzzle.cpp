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

void Puzzle::WriteToFile(PuzzlePiece *element, ofstream &file) {
    file.write(reinterpret_cast<const char *>(&element->row), 2);
    file.write(reinterpret_cast<const char *>(&element->col), 2);

    for (int i = 1; i < (element->colors).size() - 1; i++) {
        for (int j = 1; j < (element->colors[0]).size() - 1; j++) {
            file.write(reinterpret_cast<const char *>(
                           &get<0>((element->colors)[i][j])),
                       2);
            file.write(reinterpret_cast<const char *>(
                           &get<1>((element->colors)[i][j])),
                       2);
            file.write(reinterpret_cast<const char *>(
                           &get<2>((element->colors)[i][j])),
                       2);
        }
    }
}

void Puzzle::EdgeAlgorithm(string filename) {
    auto timeInit = chrono::system_clock::now();

    ofstream file(filename, ios::binary);

    // This will fetch top left corner piece first
    int complement;
    int leftIdx = 0;
    int rightIdx = cols;
    int topIdx = 1;
    int bottomIdx = rows;
    bool isStarting = true;
    int counter;
    bool isOdd = false;

    if (rows % 2 == 0) {
        counter = rows / 2;
    } else {
        counter = (rows - 1) / 2;
        isOdd = true;
    }

    while (counter > 0) {
        for (int i = leftIdx; i < rightIdx; i++) {
            if (isStarting) {
                // Locate top left corner piece
                for (auto element : topEdges[flatEdge]) {
                    if (leftEdges[flatEdge].find(element) !=
                        leftEdges[flatEdge].end()) {
                        // Row Column R G B
                        // Write to file location + rgb values
                        WriteToFile(element, file);

                        complement = getComplementEdge(element->right);
                        isStarting = false;
                    }
                }
                continue;
            }
            // Index leftEdges map with the complement
            auto leftSet = leftEdges[complement];
            auto element = leftSet.begin();

            // Write to file location + rgb values
            WriteToFile(*element, file);

            complement = getComplementEdge((*element)->right);
            if (i == rightIdx - 1) {
                // Get the complement of the bottom edge to prepare for top to
                // down portion of algorithm
                complement = getComplementEdge((*element)->bottom);
            }
        }

        // Start at row = 1
        for (int i = topIdx; i < bottomIdx; i++) {
            // Index topEdges map with the complement-> returns set of all
            // possible pieces
            auto topSet = topEdges[complement];
            auto element = topSet.begin();
            // Write to file location + rgb values
            WriteToFile(*element, file);

            complement = getComplementEdge((*element)->bottom);
            if (i == bottomIdx - 1) {
                // Get complement of left edge to prepare for right to left
                // portion of algorithm
                complement = getComplementEdge((*element)->left);
            }
        }

        // Start at col = 315
        for (int i = rightIdx - 2; i >= leftIdx; i--) {
            // Index rightEdges map with the complement
            auto rightSet = rightEdges[complement];
            auto element = rightSet.begin();
            // Write to file location + rgb values
            WriteToFile(*element, file);

            complement = getComplementEdge((*element)->left);
            if (i == leftIdx) {
                // Get complement of top edge to prepare for bottom to top
                // portion of algorithm
                complement = getComplementEdge((*element)->top);
            }
        }

        // Start at row = 315
        for (int i = bottomIdx - 2; i >= topIdx; i--) {
            // Index bottomEdges map with the complement-> returns set of all
            // possible pieces
            auto bottomSet = bottomEdges[complement];
            auto element = bottomSet.begin();
            // Write to file location + rgb values
            WriteToFile(*element, file);

            complement = getComplementEdge((*element)->top);
            if (i == topIdx) {
                // Get complement of right edge to prepare for another left to
                // right portion of algorithm
                complement = getComplementEdge((*element)->right);
            }
        }

        leftIdx++;
        rightIdx--;
        topIdx++;
        bottomIdx--;
        counter--;
    }

    if (isOdd) {
        auto lastSet = leftEdges[complement];
        auto element = lastSet.begin();
        // Write to file location + rgb values
        WriteToFile(*element, file);
    }
    auto timeFinal = chrono::system_clock::now();
    auto duration = timeFinal - timeInit;
    auto time =
        (int)chrono::duration_cast<chrono::milliseconds>(duration).count();

    file.write(reinterpret_cast<const char *>(&time), 2);
    file.close();
}


void Puzzle::ColorAlgorithm(string filename) {
    //Set up random
    ofstream file(filename, ios::binary);

    unordered_set<PuzzlePiece*> set1;
    unordered_set<PuzzlePiece*> set2;

    unordered_set<PuzzlePiece> SolvedPuzzlePieces;
    unordered_set<PuzzlePiece> PiecesInColorCluster;
    queue<PuzzlePiece> NewPieceQueue;
    int numPiecesSolved = 0;
    int randomIdx;
    int edgeValue;

    while(numPiecesSolved <= rows*cols) {
        cout << "Starting new color cluster..." << endl;
        cout << "NumPieces: " << numPiecesSolved << endl;

        // Randomly select key
        randomIdx = rand() % topEdges.size();
        auto it = topEdges.begin();
        advance(it, randomIdx);

        PuzzlePiece* initPiece = *it->second.begin();
        while(SolvedPuzzlePieces.find(*initPiece) != SolvedPuzzlePieces.end()) {
            randomIdx = rand() % topEdges.size();
            it = topEdges.begin();
            advance(it, randomIdx);
            initPiece = *it->second.begin();
        }

        SolvedPuzzlePieces.insert(*initPiece);

        vector<vector<tuple<int, int, int>>> colorMatrix = initPiece->colors;
        vector<int> hashedRGB;
        WriteToFile(initPiece, file);

        NewPieceQueue.push(*initPiece);

        while (!NewPieceQueue.empty()) {
            //cout << "Queue size: " << NewPieceQueue.size() << endl;
            PuzzlePiece piece = NewPieceQueue.front();
            //PiecesInColorCluster.insert(piece);
            NewPieceQueue.pop();
            numPiecesSolved++;

            vector<int> vect = {piece.top, piece.bottom, piece.left, piece.right};

            for (int i = 0; i < vect.size(); i++) {
                edgeValue = vect[i];

                //We want to avoid flat edges as the cluster will not expand in the direction of the flat edge
                if (edgeValue == flatEdge) {
                    continue;
                }

                //If 0th index: piece->top
                if (i == 0) {
                    for (int j = 1; j < colorMatrix[0].size() - 1; j++) {
                        hashedRGB.push_back(hashRGBValues(colorMatrix[0][j]));
                    }
                    set1 = bottomLeftQuadColors[hashedRGB[0]];
                    set2 = bottomRightQuadColors[hashedRGB[1]];
                }
                //If 1st index: piece->bottom
                if (i == 1) {
                    for (int j = 1; j < colorMatrix[colorMatrix.size() - 1].size() - 1; j++) {
                        hashedRGB.push_back(hashRGBValues(colorMatrix[colorMatrix.size() - 1][j]));
                    }
                    set1 = topLeftQuadColors[hashedRGB[0]];
                    set2 = topRightQuadColors[hashedRGB[1]];
                }

                //If 2nd index: piece->left
                if (i == 2) {
                    for (int j = 1; j < colorMatrix.size() - 1; j++) {
                        hashedRGB.push_back(hashRGBValues(colorMatrix[j][0]));
                    }
                    set1 = topRightQuadColors[hashedRGB[0]];
                    set2 = bottomRightQuadColors[hashedRGB[1]];
                }

                //If 3rd index: piece->right
                if (i == 3) {
                    for (int j = 1; j < colorMatrix.size() - 1; j++) {
                        hashedRGB.push_back(hashRGBValues(colorMatrix[j][colorMatrix.size() - 1]));
                    }
                    set1 = topLeftQuadColors[hashedRGB[0]];
                    set2 = bottomLeftQuadColors[hashedRGB[1]];
                }


                int complement = getComplementEdge(edgeValue);
                for (auto newPiece: set1) {
                    if (set2.find(newPiece) != set2.end()) {
                        vector<int> newPieceEdges = {newPiece->top, newPiece->bottom, newPiece->left, newPiece->right};
                        for (int k = 0; k < newPieceEdges.size(); k++) {
                            if (complement == newPieceEdges[k]) {
                                //Write new piece to file
                                WriteToFile(newPiece, file);
                                if (PiecesInColorCluster.find(*newPiece) == PiecesInColorCluster.end()) {
                                    NewPieceQueue.push(*newPiece);
                                    SolvedPuzzlePieces.insert(*newPiece);
                                    PiecesInColorCluster.insert(*newPiece);

                                }
                            }
                        }

                    }
                }

                hashedRGB.clear();
            }
        }
        PiecesInColorCluster.clear();
    }
    cout << "Num Pieces Solved: " << numPiecesSolved << endl;
}