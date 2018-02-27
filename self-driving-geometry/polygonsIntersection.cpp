# include <iostream>
# include <stdlib.h>
# include "utility.h"
using namespace std;

int main(int argc, char *argv[]) {
    int numVertices1 = 0, numVertices2 = 0;
    Point *vertices1, *vertices2;

    cout << "How many vertices does the first polygon have?" << endl;
    cin >> numVertices1;
    vertices1 = new Point[numVertices1];
    cout << "Remember to enter all vertices in clockwise order or anticlockwise order!" << endl;
    for (int i = 0; i < numVertices1; i++) {
        cout << "Enter the x coordinate of vertex " << i+1 << ": ";
        cin >> vertices1[i].x;
        cout << "Enter the y coordinate of vertex " << i+1 << ": ";
        cin >> vertices1[i].y;
    }

    cout << "How many vertices does the second polygon have?" << endl;
    cin >> numVertices2;
    vertices2 = new Point[numVertices2];
    cout << "Rember to enter all vertices in clockwise order or anticlockwise order!" << endl;
    for (int i = 0; i < numVertices2; i++) {
        cout << "Enter the x coordinate of vertex " << i+1 << ": ";
        cin >> vertices2[i].x;
        cout << "Enter the y coordinate of vertex " << i+1 << ": ";
        cin >> vertices2[i].y;
    }

    if (hasOverlap(vertices1, vertices2, numVertices1, numVertices2))
        cout << "The two polygon has overlap!" << endl;
    else
        cout << "The two polygon does not have any overlap!" << endl;

    return 0;
}
