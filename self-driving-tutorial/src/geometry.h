/*
 * geometry.h
 *
 * Created on: Feb 28, 2018
 * Author: lkang
 */

#ifndef SRC_GEOMETRY_H_
#define SRC_GEOMETRY_H_

#include "utility.h"

namespace Geometry {
    bool isInside(vector<Point> polygon, Point p);
    bool hasOverlap(vector<Point> polygon1, vector<Point> polygon2);
    vector<Point> convexHull_Javis(vector<Point> points);
    vector<Point> convexHull_Graham(vector<Point> points);
    bool compare_sort(Point p1, Point p2); 
}

#endif /* SRC_GEOMETRY_H_ */
