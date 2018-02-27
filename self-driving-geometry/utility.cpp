# include <iostream>
# include <stdlib.h>
# include "utility.h"
using namespace std;
 
// given three colinear points p, q, r, the function checks if q lies on line segment 'pr'
bool onSegment(Point p, Point q, Point r) {
    if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
            q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
        return true;
    return false;
}
 
// find orientation of ordered triplet (p, q, r)
// 0 --> p q r colinear
// 1 --> clockwise
// 2 --> counterclockwise
int orientation(Point p, Point q, Point r) {
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
 
    if (val == 0) return 0;  // colinear
    return (val > 0)? 1: 2;  // clock or counterclock wise
}
 
// returns true if line segment 'p1q1' and 'p2q2' intersect.
bool doIntersect(Point p1, Point q1, Point p2, Point q2) {
    // find the four orientations needed for general and special cases
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);
 
    // general case
    if (o1 != o2 && o3 != o4)
        return true;
 
    // Special Cases
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;
 
    // p1, q1 and p2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;
 
    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;
 
     // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;
 
    return false;
}
 
// returns true if the point p lies inside the polygon[] with n vertices
bool isInside(Point polygon[], int n, Point p) {
    // there must be at least 3 vertices in polygon[]
    if (n < 3)  return false;
 
    // create a point for line segment from p to infinite
    Point extreme = {INF, p.y};
 
    // count intersections of the above line with sides of polygon
    int count = 0, i = 0;
    do {
        int next = (i+1)%n;
 
        // check if the line segment from 'p' to 'extreme' intersects
        // with the line segment from 'polygon[i]' to 'polygon[next]'
        if (doIntersect(polygon[i], polygon[next], p, extreme))
        {
            // if the point 'p' is colinear with line segment 'i-next',
            // then check if it lies on segment. If it lies, return true,
            if (orientation(polygon[i], p, polygon[next]) == 0)
               return onSegment(polygon[i], p, polygon[next]);
            count++;
        }
        i = next;
    } while (i != 0);
 
    // return true if count is odd, false otherwise
    return count % 2 == 1;
}

// returns true if x is between p and q
bool between(int x, int p, int q) {
    if (p > q)
        return x >= q && x <= p;
    else
        return x <= q && x >= p;
}

// returns true if the two polygon has intersection, one with n1 vertices the other with n2
// this requires that the points in each polygon is in order (clockwise or anticlockwise)
bool hasOverlap(Point polygon1[], Point polygon2[], int n1, int n2) {
    // check whether there's vertices of one inside another
    for (int i = 0; i < n2; i++) {
        if (isInside(polygon1, n1, polygon2[i])) {
            return true;
        }
    }

    // check for edge intersection
    for (int i = 0; i < n1; i++) {
        int j = i + 1;
        if (i == n1 - 1) j = 0;   

        // general form of the equation: Mx + Ny + Q = 0
        float M = polygon1[i].y - polygon1[j].y;
        float N = polygon1[j].x - polygon1[i].x;
        float Q = 0 - M*polygon1[j].x - N*polygon1[j].y;

        for (int k = 0; k < n2; k++) {
            int l = k + 1;
            if (l == n2) l = 0;
            // general form of the equation: Ax + By + C = 0
            float A = polygon2[k].y - polygon2[l].y;
            float B = polygon2[l].x - polygon2[k].x;
            float C = 0 - A*polygon2[l].x - B*polygon2[k].y;

            // if both edge on the same line
            if (A*N-B*M == 0 && B*Q-C*N == 0) {
                if (onSegment(polygon2[k], polygon1[i], polygon2[l]) || onSegment(polygon2[k], polygon1[j], polygon2[l])) {
                    return true;
                }
            } else if (A*N - B*M != 0) {
                float X = (B*Q-C*N) / (A*N-B*M); // x-Coordinate of the intersection
                float Y = (A*Q-M*C) / (M*B-A*N); // y-Coordinate of the intersection
                if (between(X, polygon2[k].x, polygon2[l].x) && between(X, polygon1[i].x, polygon1[j].x) 
                    && between(Y, polygon2[k].y, polygon2[l].y) && between(Y, polygon1[i].y, polygon1[j].y)) {
                    return true;
                }
            }
        }
    }

    // there's no intersection & there's no completely cover
    return false;
} 
