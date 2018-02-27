# include <iostream>
# include <stdlib.h>
# include "utility.h"
using namespace std;
 
int main(int argc, char *argv[]) {
    int numVertices = 0;
    Point* vertices;
    if (argc > 1) {
        numVertices = (argc - 1) / 2;
        vertices = new Point[numVertices];
        for (int i = 0; i < numVertices; i++) {
            vertices[i].x = atoi(argv[i*2 + 1]);
            vertices[i].y = atoi(argv[i*2 + 2]);
        }
    } else {
        cout << "How many vertices does the polygon have?" << endl;
        cin >> numVertices;
        vertices = new Point[numVertices];
        for (int i = 0; i < numVertices; i++) {
            cout << "Enter the x coordinate of vertex " << i << ": ";
            cin >> vertices[i].x;
            cout << "Enter the y coordinate of vertex " << i << ": ";
            cin >> vertices[i].y;
        }
    }
    
    int numTest;
    Point p;
    cout << "How many points do you want to test?" << endl;
    cin >> numTest;
    for (int i = 0; i < numTest; i++) {
        cout << "Enter the x coordinate of point " << i << ": ";
        cin >> p.x;
        cout << "Enter the y coordinate of point " << i << ": ";
        cin >> p.y;
        isInside(vertices, numVertices, p)? cout << "Yes \n": cout << "No \n";
    }
     
    return 0;
}
