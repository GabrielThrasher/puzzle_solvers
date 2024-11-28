#include "puzzle_piece.h"
PuzzlePiece::PuzzlePiece() {}

PuzzlePiece::PuzzlePiece(int row, int col, int pieceSize) {
    this->col = col;
    this->row = row;
    initColor(pieceSize);
}

bool PuzzlePiece::operator==(const PuzzlePiece &other) const {
    return (this->row == other.row && this->col == other.col);
}

void PuzzlePiece::initColor(int pieceSize) {
    std::tuple<int, int, int> arr = {-1, -1, -1};

    for (int i = 0; i < pieceSize + 2; i++) {
        vector<tuple<int, int, int>> row;
        for (int j = 0; j < pieceSize + 2; j++) {
            row.push_back(arr);
        }
        colors.push_back(row);
    }
}

// Serialization
void PuzzlePiece::write(std::ofstream& out) const {
    // Saving identifier info
    out.write(reinterpret_cast<const char*>(&row), sizeof(row));
    out.write(reinterpret_cast<const char*>(&col), sizeof(col));

    // Saving edge info
    out.write(reinterpret_cast<const char*>(&top), sizeof(top));
    out.write(reinterpret_cast<const char*>(&left), sizeof(left));
    out.write(reinterpret_cast<const char*>(&bottom), sizeof(bottom));
    out.write(reinterpret_cast<const char*>(&right), sizeof(right));

    // Saving color info
    unsigned long long int rows = colors.size();
    out.write(reinterpret_cast<const char*>(&rows), sizeof(rows));

    for (const auto& color_row : colors) {
        unsigned long long int cols = color_row.size();
        out.write(reinterpret_cast<const char*>(&cols), sizeof(cols));

        for (const auto& rgb : color_row) {
            int red = get<0>(rgb);
            int green = get<1>(rgb);
            int blue = get<2>(rgb);

            out.write(reinterpret_cast<const char*>(&red), sizeof(red));
            out.write(reinterpret_cast<const char*>(&green), sizeof(green));
            out.write(reinterpret_cast<const char*>(&blue), sizeof(blue));
        }
    }
}

// Deserialization
int PuzzlePiece::read(const std::vector<char>& buffer, int offset) {
    // Helper lambda to read from the buffer
    auto readFromBuffer = [&](void* dest, int size) {
        std::memcpy(dest, buffer.data() + offset, size);
        offset += size;
    };

    // Loading identifier info
    readFromBuffer(&row, sizeof(row));
    //cout << "row: " << row << endl;
    readFromBuffer(&col, sizeof(col));
    //cout << "col: " << col << endl;

    // Loading edge info
    readFromBuffer(&top, sizeof(top));
    //cout << "top: " << top << endl;
    readFromBuffer(&left, sizeof(left));
    //cout << "left: " << left << endl;
    readFromBuffer(&bottom, sizeof(bottom));
    //cout << "bottom: " << bottom << endl;
    readFromBuffer(&right, sizeof(right));
    //cout << "right: " << right << endl;

    // Loading color info
    unsigned long long rows;
    readFromBuffer(&rows, sizeof(rows));
    //cout << "rows: " << rows << endl;

    colors.clear();
    colors.reserve(rows); // Preallocate rows in the colors vector

    for (unsigned long long i = 0; i < rows; i++) {
        unsigned long long cols;
        readFromBuffer(&cols, sizeof(cols));
        //cout << "cols: " << cols << endl;

        std::vector<std::tuple<int, int, int>> colorRow;
        colorRow.reserve(cols); // Preallocate columns in the colorRow vector

        for (unsigned long long j = 0; j < cols; j++) {
            int colorData[3]; // Array to batch read red, green, and blue
            readFromBuffer(colorData, sizeof(colorData));
            //cout << "red, green, blue: " << colorData[0] << " " << colorData[1] << " " << colorData[2] << endl;
            colorRow.emplace_back(colorData[0], colorData[1], colorData[2]);
        }

        colors.push_back(std::move(colorRow)); // Move the row to avoid copying
    }

    return offset;
}
