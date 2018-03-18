/*
 *  routine.h
 *
 *  Created on: Mar 8, 2018
 *  Author: lkang
 */

#ifndef SRC_ROUTINES_H_
#define SRC_ROUTINES_H_

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "headers.h"
#include "geometry.h"
#include "object_detection.h"
#include "fileio.h"

using namespace std;

using Points = vector<Point>;

namespace Routines {

void cascadeRoutine();
void pointInPolygonRoutine(bool (*pointInPolygon)(Points, Point));
void polygonOverlapRoutine(bool (*polygonOverlap)(Points, Points));
void convexHullRoutine(Points (*convexHull)(Points));
// void convexHullRoutine(const function<Points(Points)>& convexHull);
}

#endif /* SRC_ROUTINES_H_ */
