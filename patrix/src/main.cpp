#include "puzzle_generation.h"
#include "puzzle.h"
#include "read_in_image.h"
#include <iostream>
using namespace std;

int main() {
    PuzzleGeneration puzzle_generation;
    puzzle_generation.generatePuzzle();

    //Testing algorithm here
    Algorithms alg;
    alg.EdgeAlgorithm(puzzle_generation);
}