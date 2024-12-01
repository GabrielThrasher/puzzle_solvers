#include "puzzle.h"
using namespace std;

// union Edge {
//     struct {
//         unsigned cell1 : 3;
//         unsigned cell2 : 3;
//         unsigned cell3 : 3;
//         unsigned cell4 : 3;
//         unsigned cell5 : 3;
//         unsigned cell6 : 3;
//         unsigned cell7 : 3;
//         unsigned cell8 : 3;
//     } cells;
//     unsigned int value;
// };

class PuzzlePieceTest {
    Edge top;
};

int getEdge() {
    std::random_device rd;  // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator

    std::uniform_int_distribution<> distr(0, 16777215); // define the range

    return distr(gen);
}

int main() {
    // std::cout << getEdge() << std::endl;
    Puzzle puzzle;
    puzzle.generate("../images/CityOverWater.png");
    puzzle.solve();
}