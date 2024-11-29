#include "puzzle.h"
#include <iostream>
using namespace std;

int main() {
    Puzzle puzzle;
    puzzle.generateFromImage("PixelArt.png");

    puzzle.EdgeAlgorithm("example.bin");
    //puzzle.ColorAlgorithm();
}