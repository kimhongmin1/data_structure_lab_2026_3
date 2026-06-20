#include <iostream>
#include "WGraphprim.h"

using namespace std;

int main() {
    WGraphMST g;
    g.load("graph.txt");

    g.display("입력 그래프: graph.txt");

    cout << "MST By Kruskal's Algorithm" << endl;
    g.Kruskal();

    cout << endl;
    cout << "MST By Prim's Algorithm" << endl;
    g.Prim();

    return 0;
}
