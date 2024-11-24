#include <puzzle_generation.h>

void PuzzleGeneration::generatePuzzle() {
    cout << "PUZZLE GENERATION" << endl;
    cout << "-------------------------------------" << endl;
    cout << "Starting puzzle generation..." << std::endl;
    for (int i = 0; i < rows; i++) {
        puzzleRow = {};

        for (int j = 0; j < cols; j++) {
            PuzzlePiece piece(i, j);
            puzzleRow.push_back(piece);
            generatePiece(i, j);
            // put add color function here
            updatePuzzleStorageMaps(j);
        }
        puzzle.push_back(puzzleRow);
    }
    cout << "Finsihed puzzle generation." << endl;
    cout << "-------------------------------------" << endl;
    printPuzzleStorageMapsSize();
}

void PuzzleGeneration::generatePiece(int row, int col) {
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

void PuzzleGeneration::updatePuzzleStorageMaps(int col) {
    PuzzlePiece& piece = puzzleRow[col];

    topEdges[piece.edges[topEdgeName]].insert(piece);
    leftEdges[piece.edges[leftEdgeName]].insert(piece);
    bottomEdges[piece.edges[bottomEdgeName]].insert(piece);
    rightEdges[piece.edges[rightEdgeName]].insert(piece);

    // Add the 4 color maps updates here with the *exact same* logIC to that above
    // CODE HERE
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

    // Add the 4 color maps prints here with *similar* logic to that above
    // CODE HERE
    cout << "-------------------------------------" << endl;
}

void PuzzleGeneration::savePuzzle() {
    // Use this function to write to 8 seperate files all 8 (4 edge + 4 color) of the unordered maps
    cout << "SAVING THE PUZZLE" << endl;
    cout << "-------------------------------------" << endl;
    // CODE HERE
    cout << "Saved all puzzle maps into seperate files." << endl;
}
