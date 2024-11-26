#include "puzzle.h"
using namespace std;

void Algorithms::EdgeAlgorithm(PuzzleGeneration& obj) {
    //Locate top left corner piece: Has two flat edges
    auto set1 = obj.gettopEdges()[obj.getFlatEdge()];
    auto iter1 = set1.begin();

    //Causing problems on my (Kevin's end)

//    string key = "leftEdgeValue";
//    while (iter1 != set1.end()) {
//        auto it = iter1->edges.find(key);
//        if (it->second == obj.getFlatEdge()) {
//            cout << " " << it->second << endl;
//            break;
//        }
//    }
}
