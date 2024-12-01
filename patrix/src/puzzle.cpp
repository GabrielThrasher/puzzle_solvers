#include "puzzle.h"
using namespace std;

Puzzle::Puzzle() : puzzle(rows, vector<PuzzlePiece *>(cols)) {
    Utilities::displayHeader("Patrix 🧩");
}

void Puzzle::generate(string imagePath) {
    Utilities::startSectionTime("generation");
    Utilities::displayHeader("PUZZLE GENERATION");
    Utilities::displayText("Starting puzzle generation...");

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

    int duration = Utilities::endSectionTime("generation");
    Utilities::displayText("Finished puzzle generation (" +
                           to_string(duration) + " ms).");
    Utilities::displaySectionDivder();
    printPuzzleStorageMapsSize();
}

void Puzzle::solve() {
    Utilities::startSectionTime("algoSection");
    Utilities::displayHeader("SOLVE PUZZLE");

    edgeAlgorithm("edgeAlgoSteps.bin");
    colorAlgorithm("colorAlgoSteps.bin");

    int duration = Utilities::endSectionTime("algoSection");
    Utilities::displayText("Finished algorithm implementations (" +
                           to_string(duration) + " ms).");
    Utilities::displaySectionDivder();
}

void Puzzle::addEdges(int row, int col, PuzzlePiece *piece) {
    int minColIdx = 0;
    int minRowIdx = 0;
    int maxColIdx = cols - 1;
    int maxRowIdx = rows - 1;

    // Top left corner piece
    if (col == minColIdx && row == minRowIdx) {
        piece->top.value = flatEdge;
        piece->left.value = flatEdge;
        piece->bottom = getUniqueEdge(bottomEdges);
        piece->right = getUniqueEdge(rightEdges);
    }
    // Top right corner piece
    else if (col == maxColIdx && row == minRowIdx) {
        piece->top.value = flatEdge;
        piece->right.value = flatEdge;
        piece->left = getComplementEdge(puzzle[row][col - 1]->right);
        piece->bottom = getUniqueEdge(bottomEdges);
    }
    // Bottom left corner piece
    else if (col == minColIdx && row == maxRowIdx) {
        piece->left.value = flatEdge;
        piece->bottom.value = flatEdge;
        piece->top = getComplementEdge(puzzle[row - 1][col]->bottom);
        piece->right = getUniqueEdge(rightEdges);
    }
    // Bottom right corner piece
    else if (col == maxColIdx && row == maxRowIdx) {
        piece->bottom.value = flatEdge;
        piece->right.value = flatEdge;
        piece->top = getComplementEdge(puzzle[row - 1][col]->bottom);
        piece->left = getComplementEdge(puzzle[row][col - 1]->right);
    }
    // Top edge piece
    else if (row == minRowIdx) {
        piece->top.value = flatEdge;
        piece->left = getComplementEdge(puzzle[row][col - 1]->right);
        piece->bottom = getUniqueEdge(bottomEdges);
        piece->right = getUniqueEdge(rightEdges);
    }
    // Left edge piece
    else if (col == minColIdx) {
        piece->top = getComplementEdge(puzzle[row - 1][col]->bottom);
        piece->left.value = flatEdge;
        piece->bottom = getUniqueEdge(bottomEdges);
        piece->right = getUniqueEdge(rightEdges);
    }
    // Bottom edge piece
    else if (row == maxRowIdx) {
        piece->top = getComplementEdge(puzzle[row - 1][col]->bottom);
        piece->left = getComplementEdge(puzzle[row][col - 1]->right);
        piece->bottom.value = flatEdge;
        piece->right = getUniqueEdge(rightEdges);
    }
    // Right edge piece
    else if (col == maxColIdx) {
        piece->top = getComplementEdge(puzzle[row - 1][col]->bottom);
        piece->left = getComplementEdge(puzzle[row][col - 1]->right);
        piece->bottom = getUniqueEdge(bottomEdges);
        piece->right.value = flatEdge;
    }
    // Middle piece
    else {
        piece->top = getComplementEdge(puzzle[row - 1][col]->bottom);
        piece->left = getComplementEdge(puzzle[row][col - 1]->right);
        piece->bottom = getUniqueEdge(bottomEdges);
        piece->right = getUniqueEdge(rightEdges);
    }
}

Edge Puzzle::getEdge() {
    std::random_device rd;  // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator

    // std::uniform_int_distribution<> distr(0, 16777215 * 2); // define the
    // range
    std::uniform_int_distribution<> distr(1, 7); // define the range

    // return distr(gen) % 16777215;
    Edge edge;
    edge.cells.cell1 = distr(gen);
    edge.cells.cell2 = distr(gen);
    edge.cells.cell3 = distr(gen);
    edge.cells.cell4 = distr(gen);
    edge.cells.cell5 = distr(gen);
    edge.cells.cell6 = distr(gen);
    edge.cells.cell7 = distr(gen);
    edge.cells.cell8 = distr(gen);

    return edge;
    // string octalStr;
    // random_device rd;
    // unsigned long seed = rd();
    // mt19937 engine(seed);

    // discrete_distribution<> dist{
    //     {0, 1, 1, 0, 1, 1,
    //      1}}; // Distribution of possible octal values: {1, 2, 4, 5, 6}
    // auto rng = bind(dist, ref(engine));

    // for (int sideIdx = 0; sideIdx < 8; sideIdx++) {
    //     int randNum = rng();
    //     octalStr += to_string(randNum);
    // }

    // return stoi(octalStr);
}

Edge Puzzle::getUniqueEdge(EdgeMap &map) {
    // Utilities::startSectionTime("get unique edge");
    Edge edge = getEdge();
    auto iter = map.find(edge.value);

    // Keep generating a new edge octal value for as long as it is already being
    // used in a same-side edge OR it is the same as the flat edge value
    while (iter != map.end() || edge.value == flatEdge) {
        edge = getEdge();
        iter = map.find(edge.value);
    }
    // int duration = Utilities::endSectionTime("get unique edge");
    // Utilities::displayText("get unique (" +
    //                        to_string(duration) + " ms).");
    // Utilities::displaySectionDivder();

    return edge;
}

Edge Puzzle::getComplementEdge(Edge &edge) {
    int mask = 4;
    Edge newEdge;
    newEdge.value = edge.value;
    if (edge.cells.cell1 != 4)
        newEdge.cells.cell1 = edge.cells.cell1 ^ mask;
    if (edge.cells.cell2 != 4)
        newEdge.cells.cell2 = edge.cells.cell2 ^ mask;
    if (edge.cells.cell3 != 4)
        newEdge.cells.cell3 = edge.cells.cell3 ^ mask;
    if (edge.cells.cell4 != 4)
        newEdge.cells.cell4 = edge.cells.cell4 ^ mask;
    if (edge.cells.cell5 != 4)
        newEdge.cells.cell5 = edge.cells.cell5 ^ mask;
    if (edge.cells.cell6 != 4)
        newEdge.cells.cell6 = edge.cells.cell6 ^ mask;
    if (edge.cells.cell7 != 4)
        newEdge.cells.cell7 = edge.cells.cell7 ^ mask;
    if (edge.cells.cell8 != 4)
        newEdge.cells.cell8 = edge.cells.cell8 ^ mask;
    return newEdge;
    // k/ string str = to_string(num);
    // string comp = "";
    // unordered_map<char, char> compMap = {
    //     {'1', '5'}, {'2', '6'}, {'4', '4'}, {'6', '2'}, {'5', '1'},
    // };

    // for (auto chr : str) {
    //     comp += compMap[chr];
    // }

    // return stoi(comp);
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
    topEdges[piece->top.value].insert(piece);
    leftEdges[piece->left.value].insert(piece);
    bottomEdges[piece->bottom.value].insert(piece);
    rightEdges[piece->right.value].insert(piece);

    // Update color storage
    topLeftQuadTopEdge[hashRGBValues(piece->colors[1][1])][piece->top.value] =
        piece;
    topLeftQuadLeftEdge[hashRGBValues(piece->colors[1][1])][piece->left.value] =
        piece;
    topRightQuadRightEdge[hashRGBValues(piece->colors[1][2])]
                         [piece->right.value] = piece;
    bottomLeftQuadBottomEdge[hashRGBValues(piece->colors[2][1])]
                            [piece->bottom.value] = piece;
}

int Puzzle::hashRGBValues(tuple<int, int, int> rgb) {
    std::hash<int> hasher;

    // Combine the hashes of individual numbers
    size_t hash_value = hasher(get<0>(rgb));
    hash_value ^= hasher(get<1>(rgb)) + 0x9e3779b9 + (hash_value << 6) +
                  (hash_value >> 2);
    hash_value ^= hasher(get<2>(rgb)) + 0x9e3779b9 + (hash_value << 6) +
                  (hash_value >> 2);
    return hash_value;
    // string hashValue = to_string(get<0>(rgb)) + to_string(get<1>(rgb)) +
    //                    to_string(get<2>(rgb));

    // return stoi(hashValue);
}

void Puzzle::printPuzzleStorageMapsSize() {
    // Dev fcn to verify that the puzzle storage maps were created properly
    Utilities::startSectionTime("printStorage");
    Utilities::displayHeader("STORAGE INFO");

    // Display the size of all edge maps
    unordered_map<string, EdgeMap> edgeMaps = {{"topEdges", topEdges},
                                               {"leftEdges", leftEdges},
                                               {"bottomEdges", bottomEdges},
                                               {"rightEdges", rightEdges}};

    for (auto ele : edgeMaps) {
        string name = ele.first;
        unordered_map<int, unordered_set<PuzzlePiece *>> map = ele.second;

        string text =
            name + " map: " + to_string(map.size() - 1) + " unique edges + " +
            to_string(map[flatEdge].size()) + " flat edges = " +
            to_string(map.size() + map[flatEdge].size() - 1) + " edges.";
        Utilities::displayText(text);
    }

    // Display the size of all color maps
    unordered_map<string, ColorMap> colorMaps = {
        {"topLeftQuadTopEdge", topLeftQuadTopEdge},
        {"topLeftQuadLeftEdge", topLeftQuadLeftEdge},
        {"topRightQuadRightEdge", topRightQuadRightEdge},
        {"bottomLeftQuadBottomEdge", bottomLeftQuadBottomEdge}};

    for (auto ele : colorMaps) {
        string name = ele.first;
        ColorMap map = ele.second;

        string text =
            name + " map: " + to_string(map.size()) + " unique colors.";
        Utilities::displayText(text);
    }

    int duration = Utilities::endSectionTime("printStorage");
    Utilities::displayText("Finished displaying storage info (" +
                           to_string(duration) + " ms).");
    Utilities::displaySectionDivder();
    ;
}

bool Puzzle::isValidColorIdx(int row, int col) {
    return ((row >= 0 && row < pieceSize * rows) &&
            (col >= 0 && col < pieceSize * cols));
}

bool Puzzle::isValidMatirxIdx(int row, int col) {
    return ((row >= 0 || col >= 0) && (row < pieceSize || col < pieceSize));
}

void Puzzle::writePieceToFile(PuzzlePiece *element, ofstream &file) {
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

void Puzzle::edgeAlgorithm(string filename) {
    Utilities::startSectionTime("edgeAlgo");
    Utilities::displayText("Started edge algorithm.");

    ofstream file(filename, ios::binary);

    Edge complement;
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
                for (auto piece : topEdges[flatEdge]) {
                    if (leftEdges[flatEdge].find(piece) !=
                        leftEdges[flatEdge].end()) {
                        // Write to file location + rgb values
                        writePieceToFile(piece, file);
                        complement = getComplementEdge(piece->right);
                        isStarting = false;
                    }
                }
                continue;
            }
            // Index leftEdges map with the complement
            auto leftSet = leftEdges[complement.value];
            auto piece = leftSet.begin();

            // Write to file location + rgb values
            writePieceToFile(*piece, file);

            complement = getComplementEdge((*piece)->right);
            if (i == rightIdx - 1) {
                // Get the complement of the bottom edge to prepare for
                // top-to-down portion of algorithm
                complement = getComplementEdge((*piece)->bottom);
            }
        }

        // Start at row = 1
        for (int i = topIdx; i < bottomIdx; i++) {
            // Index topEdges map with the complement-> returns set of all
            // possible pieces
            auto topSet = topEdges[complement.value];
            auto element = topSet.begin();

            // Write to file location + rgb values
            writePieceToFile(*element, file);
            complement = getComplementEdge((*element)->bottom);

            if (i == bottomIdx - 1) {
                // Get complement of left edge to prepare for right to left
                // portion of algorithm
                complement = getComplementEdge((*element)->left);
            }
        }

        // Start at col = cols - 2
        for (int i = rightIdx - 2; i >= leftIdx; i--) {
            // Index rightEdges map with the complement
            auto rightSet = rightEdges[complement.value];
            auto element = rightSet.begin();

            // Write to file location + rgb values
            writePieceToFile(*element, file);
            complement = getComplementEdge((*element)->left);

            if (i == leftIdx) {
                // Get complement of top edge to prepare for bottom to top
                // portion of algorithm
                complement = getComplementEdge((*element)->top);
            }
        }

        // Start at row = rows - 2
        for (int i = bottomIdx - 2; i >= topIdx; i--) {
            // Index bottomEdges map with the complement-> returns set of all
            // possible pieces
            auto bottomSet = bottomEdges[complement.value];
            auto element = bottomSet.begin();

            // Write to file location + rgb values
            writePieceToFile(*element, file);
            complement = getComplementEdge((*element)->top);

            if (i == topIdx) {
                // Get complement of right edge to prepare for another left to
                // right portion of algorithm
                complement = getComplementEdge((*element)->right);
            }
        }

        // Change the idx values to simuation the clockwise spiral that the algo
        // does to solve the problem
        leftIdx++;
        rightIdx--;
        topIdx++;
        bottomIdx--;
        counter--;
    }

    if (isOdd) {
        auto lastSet = leftEdges[complement.value];
        auto element = lastSet.begin();
        // Write to file location + rgb values
        writePieceToFile(*element, file);
    }

    int duration = Utilities::endSectionTime("edgeAlgo");
    Utilities::displayText("Finished edge algorithm (" + to_string(duration) +
                           " ms).");

    file.write(reinterpret_cast<const char *>(&duration), 2);
    file.close();
}

tuple<int, int, int> Puzzle::getPixelRGB(PuzzlePiece *piece, int idx) {
    const auto &colors = piece->colors;

    if (idx == 0)
        return colors[0][1];
    if (idx == 1)
        return colors[colors[0].size() - 1][1];
    if (idx == 2)
        return colors[1][0];
    if (idx == 3)
        return colors[1][colors[0].size() - 1];
}

PuzzlePiece *Puzzle::getColorPiece(PuzzlePiece *piece, int idx, Edge edgeValue,
                                   int startingHashRGBValue) {
    tuple<int, int, int> pixelRGB = getPixelRGB(piece, idx);
    int hashRGBValue = hashRGBValues(pixelRGB);

    if (hashRGBValue != startingHashRGBValue)
        return nullptr;

    unordered_map<int, unordered_map<int, PuzzlePiece *>> *colorMap;
    switch (idx) {
    case 0:
        colorMap = &bottomLeftQuadBottomEdge;
        break;
    case 1:
        colorMap = &topLeftQuadTopEdge;
        break;
    case 2:
        colorMap = &topRightQuadRightEdge;
        break;
    case 3:
        colorMap = &topLeftQuadLeftEdge;
        break;
    default:
        return nullptr;
    }

    auto it = colorMap->find(hashRGBValue);
    if (it != colorMap->end()) {
        auto &sameColorPieces = it->second;
        Edge compEdge = getComplementEdge(edgeValue);
        auto compIt = sameColorPieces.find(compEdge.value);

        if (compIt != sameColorPieces.end()) {
            return compIt->second;
        }
    }

    return nullptr;
}

void Puzzle::colorAlgorithm(string filename) {
    Utilities::startSectionTime("colorAlgo");
    Utilities::displayText("Started color algorithm.");

    ofstream file(filename, ios::binary);
    int numPieces = rows * cols;
    float bufferMultiple = 1.25;

    unordered_set<PuzzlePiece *> unsolvedPieces;
    unsolvedPieces.reserve(bufferMultiple * numPieces);
    unsolvedPieces.rehash(unsolvedPieces.bucket_count());

    unordered_set<PuzzlePiece *> SolvedPuzzlePieces;
    SolvedPuzzlePieces.reserve(bufferMultiple * numPieces);

    queue<PuzzlePiece *> NewPieceQueue;

    // Convert unordered map to unordered set
    for (auto &elem : topEdges) {
        for (auto piece : elem.second) {
            unsolvedPieces.insert(piece);
        }
    }

    while (!unsolvedPieces.empty()) {
        PuzzlePiece *initPiece = *unsolvedPieces.begin();

        SolvedPuzzlePieces.insert(initPiece);
        unsolvedPieces.erase(initPiece);
        NewPieceQueue.push(initPiece);
        writePieceToFile(initPiece, file);
        int startingHashRGBValue = hashRGBValues(initPiece->colors[3][1]);

        while (!NewPieceQueue.empty()) {
            PuzzlePiece *piece = NewPieceQueue.front();
            NewPieceQueue.pop();

            vector<Edge> vect = {piece->top, piece->bottom, piece->left,
                                 piece->right};
            for (int i = 0; i < vect.size(); i++) {
                Edge edgeValue = vect[i];

                // We want to avoid flat edges as the cluster will not expand in
                // the direction of the flat edge
                if (edgeValue.value == flatEdge)
                    continue;

                // Get the connecting piece
                PuzzlePiece *connectingPiece =
                    getColorPiece(piece, i, edgeValue, startingHashRGBValue);

                if (connectingPiece != nullptr) {
                    if (SolvedPuzzlePieces.find(connectingPiece) ==
                        SolvedPuzzlePieces.end()) {
                        SolvedPuzzlePieces.insert(connectingPiece);
                        unsolvedPieces.erase(connectingPiece);
                        NewPieceQueue.push(connectingPiece);
                        writePieceToFile(connectingPiece, file);
                    }
                }
            }
        }
    }

    int duration = Utilities::endSectionTime("colorAlgo");
    Utilities::displayText("Finished color algorithm (" + to_string(duration) +
                           " ms).");

    file.write(reinterpret_cast<const char *>(&duration), 2);
    file.close();
}

cv::Mat Puzzle::readImage(string imagePath, int newWidth, int newHeight) {
    cv::Mat img = cv::imread(imagePath, cv::IMREAD_COLOR);

    cv::Size newSize(newWidth, newHeight);
    cv::resize(img, img, newSize, 0, 0, cv::INTER_LINEAR);

    return img;
}

void Puzzle::displayImage(string imagePath, cv::Mat &img) {
    imshow("Puzzle Image", img);
}

void Puzzle::savePuzzleEdgeMap(string file, EdgeMap &map) {
    std::ofstream out(file, std::ios::binary);

    // Write size of the map
    int mapSize = map.size();
    out.write(reinterpret_cast<const char *>(&mapSize), sizeof(mapSize));

    // Write key-value pairs
    for (const auto &keyValuePair : map) {
        int key = keyValuePair.first;
        out.write(reinterpret_cast<const char *>(&key), sizeof(key));

        unordered_set<PuzzlePiece *> set = keyValuePair.second;
        int set_size = set.size();
        out.write(reinterpret_cast<const char *>(&set_size), sizeof(set_size));

        // Write each piece in the set
        for (const auto &piece : set) {
            piece->write(out);
        }
    }
    out.close();
}

void Puzzle::savePuzzleColorMap(string file, ColorMap &map) {
    std::ofstream out(file, std::ios::binary);

    // Write size of the map
    int mapSize = map.size();
    out.write(reinterpret_cast<const char *>(&mapSize), sizeof(mapSize));

    // Write key-value pairs
    for (const auto &keyValuePair : map) {
        int key = keyValuePair.first;
        out.write(reinterpret_cast<const char *>(&key), sizeof(key));

        const unordered_map<int, PuzzlePiece *> nestedMap = keyValuePair.second;

        // Write size of the map
        int nestedMapSize = nestedMap.size();
        out.write(reinterpret_cast<const char *>(&nestedMapSize),
                  sizeof(nestedMapSize));

        // Write each edge and piece in the key-value pair in the map
        for (const auto &keyValuePair : nestedMap) {
            key = keyValuePair.first;
            out.write(reinterpret_cast<const char *>(&key), sizeof(key));

            PuzzlePiece *piece = keyValuePair.second;
            piece->write(out);
        }
    }
    out.close();
}

void Puzzle::loadPuzzleEdgeMap(string file, EdgeMap &map) {
    std::ifstream in(file, std::ios::binary);

    in.seekg(0, std::ios::end);
    int fileSize = in.tellg();
    in.seekg(0, std::ios::beg);

    std::vector<char> buffer(fileSize);
    in.read(buffer.data(), fileSize);
    in.close();

    int offset = 0; // Keeps track of the current position in the buffer

    // Helper lambda to read data and advance the offset
    auto readFromBuffer = [&](auto &variable) {
        std::memcpy(&variable, buffer.data() + offset, sizeof(variable));
        offset += sizeof(variable);
    };

    // Start deserializing the map
    int mapSize;
    readFromBuffer(mapSize); // Read the size of the map

    for (int i = 0; i < mapSize; i++) {
        // Deserialize the key (int)
        int key;
        readFromBuffer(key);

        // Deserialize the unordered_set
        int setSize;
        readFromBuffer(setSize); // Read the size of the set

        std::unordered_set<PuzzlePiece *> pieces;
        for (int j = 0; j < setSize; j++) {
            PuzzlePiece *piece = new PuzzlePiece;
            offset = piece->read(buffer, offset);
            pieces.insert(piece);
        }

        map[key] = std::move(pieces);
    }
}

void Puzzle::loadPuzzleColorMap(string file, ColorMap &map) {
    std::ifstream in(file, std::ios::binary);

    in.seekg(0, std::ios::end);
    int fileSize = in.tellg();
    in.seekg(0, std::ios::beg);

    std::vector<char> buffer(fileSize);
    in.read(buffer.data(), fileSize);
    in.close();

    int offset = 0; // Keeps track of the current position in the buffer

    // Helper lambda to read data and advance the offset
    auto readFromBuffer = [&](auto &variable) {
        std::memcpy(&variable, buffer.data() + offset, sizeof(variable));
        offset += sizeof(variable);
    };

    // Start deserializing the map
    int mapSize;
    readFromBuffer(mapSize); // Read the size of the map

    for (int i = 0; i < mapSize; i++) {
        // Deserialize the key (int)
        int key;
        readFromBuffer(key);

        // Start deserializing the map
        int nestedMapSize;
        readFromBuffer(nestedMapSize);

        unordered_map<int, PuzzlePiece *> nestedMap;
        for (int j = 0; j < nestedMapSize; j++) {
            int nestedKey;
            readFromBuffer(nestedKey);

            PuzzlePiece *piece = new PuzzlePiece;
            offset = piece->read(buffer, offset);
            nestedMap.insert({nestedKey, piece});
        }

        map[key] = std::move(nestedMap);
    }
}

void Puzzle::save() {
    Utilities::startSectionTime("save");

    Utilities::displayHeader("SAVE GENERATED PUZZLE");
    Utilities::displayText("Saving...");

    savePuzzleEdgeMap("../patrix/saved puzzle/topEdges.bin", topEdges);
    savePuzzleEdgeMap("../patrix/saved puzzle/leftEdges.bin", leftEdges);
    savePuzzleEdgeMap("../patrix/saved puzzle/bottomEdges.bin", bottomEdges);
    savePuzzleEdgeMap("../patrix/saved puzzle/rightEdges.bin", rightEdges);
    savePuzzleColorMap("../patrix/saved puzzle/topLeftQuadTopEdge.bin",
                       topLeftQuadTopEdge);
    savePuzzleColorMap("../patrix/saved puzzle/topLeftQuadLeftEdge.bin",
                       topLeftQuadLeftEdge);
    savePuzzleColorMap("../patrix/saved puzzle/topRightQuadRightEdge.bin",
                       topRightQuadRightEdge);
    savePuzzleColorMap("../patrix/saved puzzle/bottomLeftQuadBottomEdge.bin",
                       bottomLeftQuadBottomEdge);

    int duration = Utilities::endSectionTime("save");
    Utilities::displayText("Finished saving (" + to_string(duration) + " ms).");
    Utilities::displaySectionDivder();
}

void Puzzle::load() {
    Utilities::startSectionTime("load");

    Utilities::displayHeader("LOAD IN PUZZLE");
    Utilities::displayText("Loading...");

    loadPuzzleEdgeMap("../patrix/saved puzzle/topEdges.bin", topEdges);
    loadPuzzleEdgeMap("../patrix/saved puzzle/leftEdges.bin", leftEdges);
    loadPuzzleEdgeMap("../patrix/saved puzzle/bottomEdges.bin", bottomEdges);
    loadPuzzleEdgeMap("../patrix/saved puzzle/rightEdges.bin", rightEdges);
    loadPuzzleColorMap("../patrix/saved puzzle/topLeftQuadTopEdge.bin",
                       topLeftQuadTopEdge);
    loadPuzzleColorMap("../patrix/saved puzzle/topLeftQuadLeftEdge.bin",
                       topLeftQuadLeftEdge);
    loadPuzzleColorMap("../patrix/saved puzzle/topRightQuadRightEdge.bin",
                       topRightQuadRightEdge);
    loadPuzzleColorMap("../patrix/saved puzzle/bottomLeftQuadBottomEdge.bin",
                       bottomLeftQuadBottomEdge);

    int duration = Utilities::endSectionTime("load");
    Utilities::displayText("Finished loading (" + to_string(duration) +
                           " ms).");
    Utilities::displaySectionDivder();
}

void Puzzle::deleteLoadedPieces() {
    // This function MUST be called after the load() function was used; you do
    // need to call solve() after load() and before deleteLoadedPieces() though.

    // Delete puzzle pieces that were on the heap in edge storage
    vector<EdgeMap> edgeMaps = {topEdges, leftEdges, bottomEdges, rightEdges};
    for (auto &map : edgeMaps) {
        for (auto &ele : map) {
            for (PuzzlePiece *piece : ele.second) {
                delete piece;
            }
        }
    }

    // Delete puzzle pieces that were on the heap in color storage
    vector<ColorMap> colorMaps = {topLeftQuadTopEdge, topLeftQuadLeftEdge,
                                  topRightQuadRightEdge,
                                  bottomLeftQuadBottomEdge};
    for (auto &map : colorMaps) {
        for (auto &outerPair : map) {
            for (auto &ele : outerPair.second) {
                delete ele.second;
            }
        }
    }
}