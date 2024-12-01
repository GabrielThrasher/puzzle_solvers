#include "puzzle_piece.h"

PuzzlePiece::PuzzlePiece(int row, int col, int pieceSize)
    : colors(4, vector<tuple<int, int, int>>(4)) {
    this->col = col;
    this->row = row;
    initColor(pieceSize);
}

PuzzlePiece::PuzzlePiece() {}

bool PuzzlePiece::operator==(const PuzzlePiece &other) const {
    return (this->row == other.row && this->col == other.col);
}

void PuzzlePiece::initColor(int pieceSize) {
    for (int i = 0; i < colors.size(); i++) {
        // std::cout << "i: " << i << std::endl;
        // int a = colors[i].size();
        for (int j = 0; j < colors[i].size(); j++) {
            // row.push_back(arr);
            colors[i][j] = {-1, -1, -1};
        }
    }
}

void PuzzlePiece::write(std::ofstream &out) const {
    // Saving identifier info
    out.write(reinterpret_cast<const char *>(&row), sizeof(row));
    out.write(reinterpret_cast<const char *>(&col), sizeof(col));

    // Saving edge info
    out.write(reinterpret_cast<const char *>(&top), sizeof(top));
    out.write(reinterpret_cast<const char *>(&left), sizeof(left));
    out.write(reinterpret_cast<const char *>(&bottom), sizeof(bottom));
    out.write(reinterpret_cast<const char *>(&right), sizeof(right));

    // Saving color info
    unsigned long long int rows = colors.size();
    out.write(reinterpret_cast<const char *>(&rows), sizeof(rows));

    for (const auto &colorRow : colors) {
        unsigned long long int cols = colorRow.size();
        out.write(reinterpret_cast<const char *>(&cols), sizeof(cols));

        for (const auto &rgb : colorRow) {
            int red = get<0>(rgb);
            int green = get<1>(rgb);
            int blue = get<2>(rgb);

            out.write(reinterpret_cast<const char *>(&red), sizeof(red));
            out.write(reinterpret_cast<const char *>(&green), sizeof(green));
            out.write(reinterpret_cast<const char *>(&blue), sizeof(blue));
        }
    }
}

int PuzzlePiece::read(const std::vector<char> &buffer, int offset) {
    // Helper lambda to read from the buffer
    auto readFromBuffer = [&](void *dest, int size) {
        std::memcpy(dest, buffer.data() + offset, size);
        offset += size;
    };

    readFromBuffer(&row, sizeof(row));
    readFromBuffer(&col, sizeof(col));
    readFromBuffer(&top, sizeof(top));
    readFromBuffer(&left, sizeof(left));
    readFromBuffer(&bottom, sizeof(bottom));
    readFromBuffer(&right, sizeof(right));

    // Loading color info
    unsigned long long rows;
    readFromBuffer(&rows, sizeof(rows));

    colors.clear();
    colors.reserve(rows);

    for (unsigned long long i = 0; i < rows; i++) {
        unsigned long long cols;
        readFromBuffer(&cols, sizeof(cols));

        std::vector<std::tuple<int, int, int>> colorRow;
        colorRow.reserve(cols);

        for (unsigned long long j = 0; j < cols; j++) {
            int colorData[3]; // Batching red, green, and blue
            readFromBuffer(colorData, sizeof(colorData));
            colorRow.emplace_back(colorData[0], colorData[1], colorData[2]);
        }

        colors.push_back(std::move(colorRow));
    }

    return offset;
}
