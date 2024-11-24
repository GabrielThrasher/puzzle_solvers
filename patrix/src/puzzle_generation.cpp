#include <puzzle_generation.h>

void PuzzleGeneration::generatePuzzle() {
    for (int i = 0; i < rows; i++) {
        puzzleRow = {};

        for (int j = 0; j < cols; j++) {
            PuzzlePiece peice(i, j);
            puzzleRow.push_back(peice);
            generatePiece(i, j);
        }
        puzzle.push_back(puzzleRow);
    }
    cout << "Top Edge: " << puzzle[101][128].edges["topEdge"] << " | Bottom Edge: " << puzzle[100][128].edges["bottomEdge"] << endl;
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
    string octalStr = "";
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

int PuzzleGeneration::getUniqueEdge(unordered_map<int, PuzzlePiece>* map) {
    int edge = getEdge();
    auto iter = map->find(edge);
    while (iter != map->end()) {
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
