#include "puzzle.h"
using namespace std;

int main() {
    Puzzle puzzle;
    puzzle.generate("../images/gator3.png");
    puzzle.solve();
}