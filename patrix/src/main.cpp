#include "puzzle.h"
#include <iostream>
using namespace std;

int main() {
    Puzzle puzzle;
    puzzle.generateFromImage("images/chicken.jpg");

    puzzle.ColorAlgorithm("color_al.bin");
    // puzzle.ColorAlgorithm();
}