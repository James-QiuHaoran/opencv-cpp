/*
 * geometry.h
 *
 *  Created on: Feb 28, 2018
 *      Author: lkang
 */

#ifndef SRC_GEOMETRY_H_
#define SRC_GEOMETRY_H_

#include "utility.h"

namespace Geometry {
    bool isInside(vector<Point> polygon, Point p);
    bool hasOverlap(vector<Point> polygon1, vector<Point> polygon2);
    vector<Point> convexHull(vector<Point> points); 
}

#endif /* SRC_GEOMETRY_H_ */
