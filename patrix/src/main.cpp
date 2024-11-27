#include "puzzle.h"
#include <iostream>
using namespace std;

int main() {
    Puzzle puzzle;
    puzzle.generateFromImage("images/black.png");

    puzzle.EdgeAlgorithm();
}