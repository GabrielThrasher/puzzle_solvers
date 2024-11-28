#include "puzzle.h"
#include <iostream>
using namespace std;

int main() {
    Puzzle puzzle;
    puzzle.generate("../images/test.webp");
    puzzle.save();
    puzzle.load();
}