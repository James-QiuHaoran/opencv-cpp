#include "headers.h"
#include "routines.h"
#include "fileio.h"

using namespace cv;
using namespace std;

bool point_in_polygon(Points polygon, Point p);
bool polygon_overlap(Points polygon1, Points polygon2);
Points convex_hull(Points points);

int main(int argc, char** argv) {
  cout << "Hello Self-Driving Tutorial" << endl;
  cout << "OpenCV Version: " << CV_VERSION << endl << endl;

  // Routines::cascadeRoutine();           // stop sign detection
  Routines::pointInPolygonRoutine(point_in_polygon); // point in polygon detection
  Routines::polygonOverlapRoutine(polygon_overlap);  // polygon overlap detection
  Routines::convexHullRoutine(convex_hull);          // convex hull construction

  return 0;
}

bool point_in_polygon(Points polygon, Point p) {
    return Geometry::isInside(polygon, p);
}

bool polygon_overlap(Points polygon1, Points polygon2) {
    return Geometry::hasOverlap(polygon1, polygon2);
}

Points convex_hull(Points points) {
    return Geometry::convexHull_Javis(points);
}
