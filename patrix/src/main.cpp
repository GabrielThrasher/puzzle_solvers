#include "puzzle.h"
using namespace std;

int main() {
    Puzzle puzzle;
    puzzle.generate("../patrix/images/BoyInGrass.png");
    puzzle.solve();
}