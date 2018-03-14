/*
 *  routines.cpp
 *
 *  Created on: Mar 8, 2018
 *  Author: lkang
 */

#include <algorithm>
#include "routines.h"
#include "geometry.h"
#define TEST_MODE 0
#define ALGORITHM 1  // 0 - Javis'; 1 - Graham's

namespace Routines {

/* MSE */
// Prototype images which are used for parameters training in command line using cascade
string stopPrototype = FileIO::GetCurrentWorkingDir() + "/dataset/train/stopPrototype.png";
string stopPrototypeFolder = FileIO::GetCurrentWorkingDir() + "/dataset/train//croped_positive_stop/";
    
/* CASCADE */ 
// Tutorial
string train_stopSign_positive = FileIO::GetCurrentWorkingDir() + "/dataset/train/positive_stop/";
string train_stopSign_negtive = FileIO::GetCurrentWorkingDir() + "/dataset/train/negative_stop/";
string test_stopSign_positive = FileIO::GetCurrentWorkingDir() + "/dataset/test/positive_stop/";
string test_stopSign_negtive = FileIO::GetCurrentWorkingDir()+ "/dataset/test/negative_stop/";
    
// Assignment 
string train_trafficLight_positive = FileIO::GetCurrentWorkingDir() + "/dataset/train/positive_light/";
string train_trafficLight_negtive = FileIO::GetCurrentWorkingDir() + "/dataset/train/negative_light/";
string test_trafficLight_positive = FileIO::GetCurrentWorkingDir() + "/dataset/test/positive_light/";
string test_trafficLight_negtive = FileIO::GetCurrentWorkingDir() + "/dataset/test/negative_light/";
 
/* Geometry */
// test cases - input file directories
string test_point_polygon = FileIO::GetCurrentWorkingDir() + "/testcases/geometry/test_point_polygon/";
string test_polygon_polygon = FileIO::GetCurrentWorkingDir() + "/testcases/geometry/test_polygon_polygon/";
string test_convex_hull = FileIO::GetCurrentWorkingDir() + "/testcases/geometry/test_convex_hull/";

// student output file directories
string student_output_point_polygon = FileIO::GetCurrentWorkingDir() + "/putYourOutputHere/point-polygon/";
string student_output_polygon_polygon = FileIO::GetCurrentWorkingDir() + "/putYourOutputHere/polygon-polygon/";
string student_output_convex_hull = FileIO::GetCurrentWorkingDir() + "/putYourOutputHere/convex-hull/";

// test whether a point is inside a polygon or not
void pointInPolygonRoutine() {
    vector<string> fileNameList = FileIO::GetFileName(test_point_polygon);
    for (int j = 0; j < (int)fileNameList.size(); j++) {
        cout << "========== Point-In-Polygon Test #" << j << " ==========" << endl;
        string file = test_point_polygon + fileNameList[j];
        vector<Point> polygon;
        vector<Point> points;
        vector<string> results;
        FileIO::loadPointPolygonInputFile(file, polygon, points); // load test case
        cout << "The polygon: ";
        for (int k = 0; k < polygon.size(); k++)
            cout << "(" << polygon[k].x << ", " << polygon[k].y << ") ";
        cout << endl;
        if (TEST_MODE) {
            results = FileIO::loadPointPolygonOutputFile(student_output_point_polygon); // load result file
        }
        for (int i = 0; i < points.size(); i++) {
            if (Geometry::isInside(polygon, Point(points[i].x, points[i].y))) {
                cout << "Sample answer: Yes - (" << points[i].x << ", " << points[i].y << ")" << endl;
                if (TEST_MODE)
                    (results.size() >= i+1 && results[i] == "Y") ? cout << "Correct answer!\n" : cout << "Wrong answer!\n";
            } else {
                cout << "Sample answer: No - (" << points[i].x << ", " << points[i].y << ")" << endl;
                if (TEST_MODE)
                    (results.size() >= i+1 && results[i] == "N") ? cout << "Correct answer!\n" : cout << "Wrong answer!\n";
            }
        }
        cout << endl;
    }
}

// test whether two polygons overlap or not
void polygonOverlapRoutine() {
    vector<string> fileNameList = FileIO::GetFileName(test_polygon_polygon);
    for (int i = 0; i < (int)fileNameList.size(); i++) {
        cout << "========== Polygon-Overlap Test #" << i << " ==========" << endl; 
        vector<Point> polygon1;
        vector<Point> polygon2;
        vector<string> result;
        string address = test_polygon_polygon + fileNameList[i];
        if (TEST_MODE) {
            result = FileIO::loadPointPolygonOutputFile(student_output_polygon_polygon); // load result files
        }
        FileIO::loadPointPolygonInputFile(address, polygon1, polygon2); // load test cases
        cout << "Polygon 1: ";
        for (int k = 0; k < polygon1.size(); k++)
            cout << "(" << polygon1[k].x << ", " << polygon1[k].y << ") ";
        cout << endl;
        cout << "Polygon 2: ";
        for (int k = 0; k < polygon2.size(); k++)
            cout << "(" << polygon2[k].x << ", " << polygon2[k].y << ") ";
        cout << endl;
        if (Geometry::hasOverlap(polygon1, polygon2)) {
            cout << "Sample answer: Yes \n";
            if (TEST_MODE)
                (result.size() == 1 && result[0] == "Y") ? cout << "Correct answer!\n" : cout << "Wrong answer!\n";
        } else {
            cout << "Sample answer: No \n";
            if (TEST_MODE)
                (result.size() == 1 && result[0] == "N") ? cout << "Correct answer!\n" : cout << "Wrong answer!\n";
        }
        cout << endl;
    }
}

// construct a convex hull over all given points
void convexHullRoutine() {
    vector<string> fileNameList = FileIO::GetFileName(test_convex_hull);
    for (int i = 0; i < (int)fileNameList.size(); i++) {
        cout << "========== Convex-Hull Test #" << i << " ==========" << endl;
        string address = test_convex_hull + fileNameList[i];
        vector<Point> points;
        vector<Point> result;
        vector<Point> hull;
        FileIO::loadConvexHullFile(address, points); // load test cases
        cout << "All points: ";
        for (int k = 0; k < points.size(); k++)
            cout << "(" << points[k].x << ", " << points[k].y << ") ";
        cout << endl;
        if (TEST_MODE) {
            FileIO::loadConvexHullFile(student_output_convex_hull, result); // load result file
            sort(result.begin(), result.end(), Geometry::compare_sort); // sort the hull points
        }
        if (ALGORITHM == 0) {
            // Javis' Algorithm
            hull = Geometry::convexHull_Javis(points);
        } else {
            // Graham's Algorithm
            hull = Geometry::convexHull_Graham(points);
        }
        if (hull.size() == 0) {
            cout << "Sample answer: Impossible" << endl;
            if (TEST_MODE)
                (result.size() == 0) ? cout << "Correct answer!\n" : cout << "Wrong answer!\n";
        } else {
            bool isWrong = false;
            if (hull.size() != result.size())
                isWrong = true;
            cout << "The convex hull: ";
            for (int i = 0; i < hull.size(); i++) {
                if (TEST_MODE && !isWrong && hull[i].x != result[i].x && hull[i].y != result[i].y)
                    isWrong = true;
                cout << "(" << hull[i].x << ", " << hull[i].y << ") ";
            }
            cout << endl;
            if (TEST_MODE && isWrong)    
                cout << "Wrong answer!" << endl;
            else if (TEST_MODE)
                cout << "Correct answer!" << endl;
        }
        cout << endl;
    }
}

void cascadeRoutine() {
  string folder = test_stopSign_positive;
  vector<string> fileNameList = FileIO::GetFileName(folder);
  int cnt = 0;
  /* Advanced method (cascade) to detect objects */
  string model = FileIO::GetCurrentWorkingDir() + "/params/cascade.xml";
  for (int i = 0; i < (int)fileNameList.size(); i++){
    cout<<fileNameList[i]<<endl;
    string file = fileNameList[i];
    cnt += ObjectDetection::detectObjCascade(model, folder, file);
  }
  cout<< cnt << " objects are detected in "<<fileNameList.size()<<" files"<<endl;
}

void processVideo() {
  VideoCapture cap("/home/lkang/Desktop/video.h264"); // open the default camera
  string output{"/home/lkang/Desktop/images/"};
  if(!cap.isOpened()) { // check if we succeeded
    cout<<"not able to open"<<endl;
    return;
  }
  Mat gray;
  for (;;) {
    Mat frame;
    cap >> frame; // get a new frame from camera
    if(frame.empty()) break;

    // int num = processImage(frame, gray);
    // imshow("gray", frame);
    // waitKey(0);
    // sleep(1);
    // cv::imwrite(output + to_string(counter) + ".png", frame);
  }
}

}
