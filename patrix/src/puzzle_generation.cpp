#include <puzzle_generation.h>

void PuzzleGeneration::generatePuzzle() {
    cout << "PUZZLE GENERATION" << endl;
    cout << "-------------------------------------" << endl;
    cout << "Starting puzzle generation..." << std::endl;
    rgbMatrix = img.readImage("PixelArt.PNG");

    for (int i = 0; i < rows; i++) {
        puzzleRow.clear();

        for (int j = 0; j < cols; j++) {
            PuzzlePiece piece(i, j, pixelResPerPiece);
            puzzleRow.push_back(piece);
            addEdges(i, j);
            addColor(i, j);
            updatePuzzleStorageMaps(j);
        }
        puzzle.push_back(puzzleRow);
    }
    cout << "Finished puzzle generation." << endl;
    cout << "-------------------------------------" << endl;
    printPuzzleStorageMapsSize();
}

void PuzzleGeneration::addEdges(int row, int col) {
    PuzzlePiece& piece = puzzleRow[col];

    // Top left corner piece
    if (col == minColIdx && row == minRowIdx) {
        piece.edges[topEdgeName] = flatEdge;
        piece.edges[leftEdgeName] = flatEdge;
        piece.edges[bottomEdgeName] = getUniqueEdge(&bottomEdges);
        piece.edges[rightEdgeName] = getUniqueEdge(&rightEdges);
    }
    // Top right corner piece
    else if (col == maxColIdx && row == minRowIdx) {
        piece.edges[topEdgeName] = flatEdge;
        piece.edges[leftEdgeName] = getComplementEdge(puzzleRow[col-1].edges[rightEdgeName]);
        piece.edges[bottomEdgeName] = getUniqueEdge(&bottomEdges);
        piece.edges[rightEdgeName] = flatEdge;
    }
    // Bottom left corner piece
    else if (col == minColIdx && row == maxRowIdx) {
        piece.edges[topEdgeName] = getComplementEdge(puzzle[row-1][col].edges[bottomEdgeName]);
        piece.edges[leftEdgeName] = flatEdge;
        piece.edges[bottomEdgeName] = flatEdge;
        piece.edges[rightEdgeName] = getUniqueEdge(&rightEdges);
    }
    // Bottom right corner piece
    else if (col == maxColIdx && row == maxRowIdx) {
        piece.edges[topEdgeName] = getComplementEdge(puzzle[row-1][col].edges[bottomEdgeName]);
        piece.edges[leftEdgeName] = getComplementEdge(puzzleRow[col-1].edges[rightEdgeName]);
        piece.edges[bottomEdgeName] = flatEdge;
        piece.edges[rightEdgeName] = flatEdge;
    }
    // Top edge piece
    else if (row == minRowIdx) {
        piece.edges[topEdgeName] = flatEdge;
        piece.edges[leftEdgeName] = getComplementEdge(puzzleRow[col-1].edges[rightEdgeName]);
        piece.edges[bottomEdgeName] = getUniqueEdge(&bottomEdges);
        piece.edges[rightEdgeName] = getUniqueEdge(&rightEdges);
    }
    // Left edge piece
    else if (col == minColIdx) {
        piece.edges[topEdgeName] = getComplementEdge(puzzle[row-1][col].edges[bottomEdgeName]);
        piece.edges[leftEdgeName] = flatEdge;
        piece.edges[bottomEdgeName] = getUniqueEdge(&bottomEdges);
        piece.edges[rightEdgeName] = getUniqueEdge(&rightEdges);
    }
    // Bottom edge piece
    else if (row == maxRowIdx) {
        piece.edges[topEdgeName] = getComplementEdge(puzzle[row-1][col].edges[bottomEdgeName]);
        piece.edges[leftEdgeName] = getComplementEdge(puzzleRow[col-1].edges[rightEdgeName]);
        piece.edges[bottomEdgeName] = flatEdge;
        piece.edges[rightEdgeName] = getUniqueEdge(&rightEdges);
    }
    // Right edge piece
    else if (col == maxColIdx) {
        piece.edges[topEdgeName] = getComplementEdge(puzzle[row-1][col].edges[bottomEdgeName]);
        piece.edges[leftEdgeName] = getComplementEdge(puzzleRow[col-1].edges[rightEdgeName]);
        piece.edges[bottomEdgeName] = getUniqueEdge(&bottomEdges);
        piece.edges[rightEdgeName] = flatEdge;
    }
    // Middle piece
    else {
        piece.edges[topEdgeName] = getComplementEdge(puzzle[row-1][col].edges[bottomEdgeName]);
        piece.edges[leftEdgeName] = getComplementEdge(puzzleRow[col-1].edges[rightEdgeName]);
        piece.edges[bottomEdgeName] = getUniqueEdge(&bottomEdges);
        piece.edges[rightEdgeName] = getUniqueEdge(&rightEdges);
    }
}

bool PuzzleGeneration::isValidColorIdx(int row, int col) {
    return ((row >= 0 && row < pixelResPerPiece*rows) && (col >= 0 && col < pixelResPerPiece*cols));
}

bool PuzzleGeneration::isValidMatirxIdx(int row, int col) {
    return ((row >= 0 || col >= 0) && (row < pixelResPerPiece || col < pixelResPerPiece));
}

void PuzzleGeneration::addColor(int row, int col) {
    PuzzlePiece& piece = puzzleRow[col];
    // Matrix indices for the puzzle matrix
    int baseMartixRowIdx = 1;
    int baseMartixColIdx = 1;

    for (int i = -1; i <= pixelResPerPiece; i++) {
        for (int j = -1; j <= pixelResPerPiece; j++) {
            // Color indices for the rgbMatrix created from the image
            int colorRowIdx = pixelResPerPiece*row + i;
            int colorColIdx = pixelResPerPiece*col + j;

            if(isValidColorIdx(colorRowIdx, colorColIdx) && isValidMatirxIdx(i, j)) {
                cv::Vec3b pixel = rgbMatrix.at<cv::Vec3b>(colorRowIdx, colorColIdx);
                int red = pixel[2], green = pixel[1], blue = pixel[0];
                piece.colors[baseMartixRowIdx + i][baseMartixColIdx + j] = {red, green, blue};
            }
        }
    }
}

int PuzzleGeneration::getEdge() {
    string octalStr;
    random_device rd;
    unsigned long seed = rd();
    mt19937 engine(seed);

    discrete_distribution<> dist {{0, 1, 1, 0, 1, 1, 1}}; // Distribution  {1, 2, 4, 5, 6}
    auto rng = bind(dist, ref(engine));

    for (int sideIdx = 0; sideIdx < 8; sideIdx++)
    {
        int randNum = rng();
        octalStr += to_string(randNum);
    }

    int octalNum = stoi(octalStr);

    return octalNum;
}

int PuzzleGeneration::getUniqueEdge(unordered_map<int, unordered_set<PuzzlePiece>>* map) {
    int edge = getEdge();
    auto iter = map->find(edge);
    // Keep generating a new edge octal value for as long as it is already being used in an same-side edge OR is the
    // same as the flat edge value
    while (iter != map->end() || edge == flatEdge) {
        edge = getEdge();
        iter = map->find(edge);
    }

    return edge;
}

int PuzzleGeneration::getComplementEdge(int num) {
    string str = to_string(num);
    string comp = "";
    unordered_map<char, char> compMap = {
        {'1', '5'},
        {'2', '6'},
        {'4', '4'},
        {'6', '2'},
        {'5', '1'},
    };

    for (auto chr : str) {
        comp += compMap[chr];
    }

    return stoi(comp);
}

int PuzzleGeneration::hashRGBValues(tuple<int, int, int> rgb) {
    string hashValue = to_string(get<0>(rgb)) + to_string(get<1>(rgb)) + to_string(get<2>(rgb));

    return stoi(hashValue);
}

void PuzzleGeneration::updatePuzzleStorageMaps(int col) {
    PuzzlePiece& piece = puzzleRow[col];

    // Update edge storage
    topEdges[piece.edges[topEdgeName]].insert(piece);
    leftEdges[piece.edges[leftEdgeName]].insert(piece);
    bottomEdges[piece.edges[bottomEdgeName]].insert(piece);
    rightEdges[piece.edges[rightEdgeName]].insert(piece);

    // Update color storage
    topLeftQuadColors[hashRGBValues(piece.colors[1][1])].insert(piece);
    topRightQuadColors[hashRGBValues(piece.colors[1][2])].insert(piece);
    bottomLeftQuadColors[hashRGBValues(piece.colors[2][1])].insert(piece);
    bottomRightQuadColors[hashRGBValues(piece.colors[2][2])].insert(piece);
}

void PuzzleGeneration::printPuzzleStorageMapsSize() {
    // Dev fcn to verify that the puzzle storage maps were created properly
    cout << "SIZE INFO ABOUT ALL MAPS" << endl;
    cout << "-------------------------------------" << endl;

    // Display the size of all edge maps
    unordered_map<string, unordered_map<int, unordered_set<PuzzlePiece>>> edgeMaps = {
        {"topEdges", topEdges},
        {"leftEdges", leftEdges},
        {"bottomEdges", bottomEdges},
        {"rightEdges", rightEdges}
    };
    for (auto ele : edgeMaps) {
        string name = ele.first;
        unordered_map<int, unordered_set<PuzzlePiece>> map = ele.second;

        std::cout << name << " map: " << map.size() - 1  << " unique edges + " << map[flatEdge].size()
        << " flat edges = " << (map.size() + map[flatEdge].size() - 1) << " edges." << std::endl;
    }

    // Display the size of all color maps
    unordered_map<string, unordered_map<int, unordered_set<PuzzlePiece>>> colorMaps = {
        {"topLeftQuadColors", topLeftQuadColors},
        {"topRightQuadColors", topRightQuadColors},
        {"bottomLeftQuadColors", bottomLeftQuadColors},
        {"bottomRightQuadColors", bottomRightQuadColors}
    };
    for (auto ele : colorMaps) {
        string name = ele.first;
        unordered_map<int, unordered_set<PuzzlePiece>> map = ele.second;

        std::cout << name << " map: " << map.size() << " unique colors." << std::endl;
    }

    cout << "-------------------------------------" << endl;
}

void PuzzleGeneration::savePuzzle() {
    // Use this function to write to 8 seperate files all 8 (4 edge + 4 color) of the unordered maps
    cout << "SAVING THE PUZZLE" << endl;
    cout << "-------------------------------------" << endl;
    // CODE HERE
    cout << "Saved all puzzle maps into seperate files." << endl;
}
