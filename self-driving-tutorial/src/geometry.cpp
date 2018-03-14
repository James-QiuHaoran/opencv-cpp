# include <iostream>
# include <stdlib.h>
# include <algorithm>
# include "geometry.h"
using namespace std;

namespace Geometry {

// given three colinear points p, q, r, the function checks if q lies on line segment 'pr'
bool onSegment(Point p, Point q, Point r) {
    if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
            q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
        return true;
    return false;
}

/*
 * find orientation of ordered triplet (p, q, r)
 *   0 --> p q r colinear
 *   1 --> clockwise
 *   2 --> counterclockwise
 */
int orientation(Point p, Point q, Point r) {
    double val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

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
bool isInside(vector<Point> polygon, Point p) {
    // there must be at least 3 vertices in polygon[]
    int n = polygon.size();
    if (n < 3)  return false;

    // get the MAX
    double maxXCoordinate = -1;
    for (int i = 0; i < n; i++) {
        if (polygon[i].x > maxXCoordinate)
            maxXCoordinate = polygon[i].x;
    }

    // create a point for line segment from p to infinite
    Point extreme = Point(maxXCoordinate + 1, p.y);

    // count intersections of the above line with sides of polygon
    int count = 0, i = 0;
    do {
        int next = (i + 1) % n;

        // check if the line segment from 'p' to 'extreme' intersects
        if (doIntersect(polygon[i], polygon[next], p, extreme))
        {
            // if the point 'p' is colinear with line segment 'i-next', then check if it lies on segment
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
bool between(double x, double p, double q) {
    if (p > q)
        return x >= q && x <= p;
    else
        return x <= q && x >= p;
}

// returns true if the two polygon has intersection, one with n1 vertices the other with n2
// Note: this requires that the points in each polygon is in order (clockwise or anticlockwise)
bool hasOverlap(vector<Point> polygon1, vector<Point> polygon2) {
    // check whether there's vertices of one inside another
    int n1 = polygon1.size();
    int n2 = polygon2.size();
    for (int i = 0; i < n2; i++) {
        if (isInside(polygon1, polygon2[i])) {
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

// prints convex hull of a set of n points - Javis' Algorithm
vector<Point> convexHull_Javis(vector<Point> points) {
    int n = points.size();
    vector<Point> hull;
    if (n < 3) return hull;
 
    // find the leftmost point
    int l = 0;
    for (int i = 1; i < n; i++)
        if (points[i].x < points[l].x)
            l = i;
 
    // start from leftmost point, keep moving counterclockwise
    // until reach the start point again.  This loop runs O(h)
    // where h is number of points in result or output.
    int p = l, q;
    do {
        // add current point to result
        hull.push_back(points[p]);
 
        // search for a point 'q' such that orientation(p, x, q) 
        // is counterclockwise for all points 'x'. The idea
        // is to keep track of last visited most counterclock-
        // wise point in q.
        q = (p + 1) % n;
        for (int i = 0; i < n; i++) {
           // If i is more counterclockwise than current q, then
           // update q
           if (orientation(points[p], points[i], points[q]) == 2)
               q = i;
        }
 
        // q is the most counterclockwise with respect to p
        // set p as q for next iteration
        p = q;
    } while (p != l);  // while we don't come back to first point

    for (int i = 0; i < hull.size(); i++) {
        if (orientation(hull[i], hull[(i+1)%hull.size()], hull[(i+2)%hull.size()]) == 0)
            hull.erase(hull.begin() + (i+1)%hull.size());
    }

    // sort the result based on x-coordinates in ascending order
    sort(hull.begin(), hull.end(), compare_sort);
 
    return hull;
}
 
// swap two points
int swap(Point &p1, Point &p2) {
    Point temp = p1;
    p1 = p2;
    p2 = temp;
}
 
// return square of distance between p1 and p2
int distSq(Point p1, Point p2) {
    return (p1.x - p2.x)*(p1.x - p2.x) +
          (p1.y - p2.y)*(p1.y - p2.y);
}

// global pivot point
Point p0;
 
// compare points with respect to the first point
int compare_qsort(const void *vp1, const void *vp2) {
    Point *p1 = (Point *)vp1;
    Point *p2 = (Point *)vp2;
 
    int o = orientation(p0, *p1, *p2);
    if (o == 0)
        return (distSq(p0, *p2) >= distSq(p0, *p1))? -1 : 1;
 
    return (o == 2)? -1: 1;
}

// compare points based on the x-coordinates
bool compare_sort(Point p1, Point p2) {
    return (p1.x < p2.x) || (p1.x == p2.x && p1.y < p2.y);
}

// prints convex hull of a set of n points - Graham's Scan Algorithm
vector<Point> convexHull_Graham(vector<Point> points) {
    int n = points.size();

    // find the bottom-most point
    int ymin = points[0].y, min = 0;
    for (int i = 1; i < n; i++) {
        int y = points[i].y;
 
        // pick the bottom-most or chose the left most point in case of tie
        if ((y < ymin) || (ymin == y && points[i].x < points[min].x))
            ymin = points[i].y, min = i;
    }
 
    // place the bottom-most point at first position
    swap(points[0], points[min]);
 
    // sort n-1 points with respect to the first point.
    // p1 comes before p2 in sorted ouput if p2 has larger polar angle (in counterclockwise direction) than p1
    p0 = points[0];
    qsort(&points[1], n-1, sizeof(Point), compare_qsort);
 
    // If two or more points make same angle with p0,
    // Remove all but the one that is farthest from p0
    // Remember that, in above sorting, our criteria was
    // to keep the farthest point at the end when more than
    // one points have same angle.
    int m = 1;
    for (int i = 1; i < n; i++) {
        // Keep removing i while angle of i and i+1 is same
        // with respect to p0
        while (i < n-1 && orientation(p0, points[i], points[i+1]) == 0)
            i++;
        points[m] = points[i];
        m++; // update size of modified array
    }
 
    vector<Point> S;

    // If modified array of points has less than 3 points,
    // convex hull is not possible
    if (m < 3) return S;
 
    S.push_back(points[0]);
    S.push_back(points[1]);
    S.push_back(points[2]);
 
    // process remaining n-3 points
    for (int i = 3; i < m; i++) {
        // Keep removing top while the angle formed by
        // points next-to-top, top, and points[i] makes
        // a non-left turn
        while (orientation(S[S.size()-2], S.back(), points[i]) != 2)
            S.pop_back();
        S.push_back(points[i]);
    }
    
    // sort the result based on x-coordinates in ascending order
    sort(S.begin(), S.end(), compare_sort);
    return S; 
}

}
