#include "puzzle_generation.h"
#include <iostream>
using namespace std;

int main() {
    cout << "Started puzzle generation..." << std::endl;
    PuzzleGeneration puzzle_generation;
    puzzle_generation.generatePuzzle();
    cout << "Finsihed puzzle generation..." << endl;
}