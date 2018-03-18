/*
 *  routines.cpp
 *
 *  Created on: Mar 8, 2018
 *  Author: lkang
 */

#include <algorithm>
#include "routines.h"
#include "geometry.h"
#include <fstream>

#define TEST_MODE 1  // 1 - test your answer; 0 - sample answer
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

// sample answer file directories
string sample_answer_point_polygon = FileIO::GetCurrentWorkingDir() + "/testcases/geometry/sample_outputs/test_point_polygon/";
string sample_answer_polygon_polygon = FileIO::GetCurrentWorkingDir() + "/testcases/geometry/sample_outputs/test_polygon_polygon/";
string sample_answer_convex_hull = FileIO::GetCurrentWorkingDir() + "/testcases/geometry/sample_outputs/test_convex_hull/";

// test whether a point is inside a polygon or not
void pointInPolygonRoutine(bool (*point_in_polygon)(Points, Point)) {
    vector<string> fileNameList = FileIO::GetFileName(test_point_polygon);
    int numPass = 0;
    int numTest = (int)fileNameList.size();

    for (int j = 0; j < numTest; j++) {
        cout << "========== Point-In-Polygon Test #" << j << " ==========" << endl;
        string file = test_point_polygon + fileNameList[j];
        vector<Point> polygon;
        vector<Point> points;
        vector<int> results;
        FileIO::loadPointPolygonInputFile(file, polygon, points); // load test case
        file = sample_answer_point_polygon + fileNameList[j];
        results = FileIO::loadPointPolygonOutputFile(file);       // load sample answer

        cout << "The polygon: ";
        for (int k = 0; k < polygon.size(); k++)
            cout << "(" << polygon[k].x << ", " << polygon[k].y << ") ";
        cout << endl;

        bool isCorrect;
        for (int i = 0; i < points.size(); i++) {
            isCorrect = true;
            cout << "Point (" << points[i].x << ", " << points[i].y << "): ";
            if (results[i] == 0)
                cout << "Sample answer - NO";
            else
                cout << "Sample answer - YES";
            if (TEST_MODE) {
                bool studentAnswer = point_in_polygon(polygon, Point(points[i].x, points[i].y));
                if ((studentAnswer && results[i] == 1) || (!studentAnswer && results[i] == 0))
                    cout << " [Correct]";
                else {
                    cout << " [Wrong]";
                    isCorrect = false;
                }
            }
            cout << endl;
        }
        if (isCorrect) numPass++;
        cout << endl;
    }

    if (TEST_MODE) 
        cout << "========== Overall performance: " << numPass << " / " << numTest << " passed! ==========" << endl;
    cout << endl;
}

// test whether two polygons overlap or not
void polygonOverlapRoutine(bool (*polygon_overlap)(Points, Points)) {
    vector<string> fileNameList = FileIO::GetFileName(test_polygon_polygon);
    int numPass = 0;
    int numTest = (int) fileNameList.size();

    for (int i = 0; i < numTest; i++) {
        cout << "========== Polygon-Overlap Test #" << i << " ==========" << endl; 
        vector<Point> polygon1;
        vector<Point> polygon2;
        vector<int> result;
        string address = test_polygon_polygon + fileNameList[i];
        string file = sample_answer_polygon_polygon + fileNameList[i];
        result = FileIO::loadPointPolygonOutputFile(file);              // load result file
        FileIO::loadPointPolygonInputFile(address, polygon1, polygon2); // load test cases

        cout << "Polygon 1: ";
        for (int k = 0; k < polygon1.size(); k++)
            cout << "(" << polygon1[k].x << ", " << polygon1[k].y << ") ";
        cout << endl;
        cout << "Polygon 2: ";
        for (int k = 0; k < polygon2.size(); k++)
            cout << "(" << polygon2[k].x << ", " << polygon2[k].y << ") ";
        cout << endl;

        if (result[0] == 0) {
            cout << "Sample answer: NO \n";
            if (TEST_MODE) {
                if (polygon_overlap(polygon1, polygon2)) {
                    cout << "Wrong answer!\n";
                } else {
                    cout << "Correct answer!\n";
                    numPass++;
                }
            }
        } else {
            cout << "Sample answer: YES \n";
            if (TEST_MODE) {
                if (polygon_overlap(polygon1, polygon2)) {
                    cout << "Correct answer!\n";
                    numPass++;
                } else
                    cout << "Wrong answer!\n";
            }
        } 
        cout << endl;
    }
    
    if (TEST_MODE)
        cout << "========== Overall performance: " << numPass << " / " << numTest << " passed! ==========" << endl;
    cout << endl;
}

// construct a convex hull over all given points
void convexHullRoutine(Points (*convex_hull)(Points)) {
    int numPass = 0;
    vector<string> fileNameList = FileIO::GetFileName(test_convex_hull);
    int numTest = (int) fileNameList.size();
    for (int i = 0; i < numTest; i++) {
        cout << "========== Convex-Hull Test #" << i << " ==========" << endl;
        string address = test_convex_hull + fileNameList[i];
        vector<Point> points;
        vector<Point> result;
        vector<Point> hull;
        
        FileIO::loadConvexHullFile(address, points); // load test cases
        string file = sample_answer_convex_hull + fileNameList[i];
        FileIO::loadConvexHullFile(file, result);    // load sample answer

        cout << "All points: ";
        for (int k = 0; k < points.size(); k++)
            cout << "(" << points[k].x << ", " << points[k].y << ") ";
        cout << endl;
        
        if (!TEST_MODE) {
            if (result.size() == 0)
                cout << "Sample answer: Impossible" << endl;
            else {
                cout << "Sample answer - the convex hull: ";
                for (int j = 0; j < result.size(); j++)
                    cout << "(" << result[j].x << ", " << result[j].y << ") ";
                cout << endl;
            }
            cout << endl;
            continue;
        }

        hull = convex_hull(points);                             // student's answer
        sort(hull.begin(), hull.end(), Geometry::compare_sort); // sort the hull points
        
        if (result.size() == 0) {
            if (hull.size() == 0) {
                cout << "Correct answer!\n";
                numPass++;
            } else
                cout << "Wrong answer!\n";
        } else {
            bool isWrong = false;
            if (hull.size() != result.size())
                isWrong = true;
            for (int i = 0; i < hull.size(); i++) {
                if (!isWrong && hull[i].x != result[i].x && hull[i].y != result[i].y)
                    isWrong = true;
                cout << "(" << hull[i].x << ", " << hull[i].y << ") ";
            }
            cout << endl;
            if (isWrong)
                cout << "Wrong answer!" << endl;
            else {
                numPass++;
                cout << "Correct answer!" << endl;
            }
        }
        cout << endl;
    }

    if (TEST_MODE)
        cout << "========== Overall performance: " << numPass << " / " << numTest << " passed! ==========" << endl;
    cout << endl;
}

void cascadeRoutine() {
    string folder = test_stopSign_positive;
    vector<string> fileNameList = FileIO::GetFileName(folder);
    int cnt = 0;
 
    /* Advanced method (cascade) to detect objects */
    string model = FileIO::GetCurrentWorkingDir() + "/params/cascade.xml";
    for (int i = 0; i < (int)fileNameList.size(); i++) {
        cout << fileNameList[i] << endl;
        string file = fileNameList[i];
        cnt += ObjectDetection::detectObjCascade(model, folder, file);
    }
    cout<< cnt << " objects are detected in "<<fileNameList.size()<<" files"<<endl;
}

void processVideo() {
    VideoCapture cap("/home/lkang/Desktop/video.h264"); // open the default camera
    string output{"/home/lkang/Desktop/images/"};
    if(!cap.isOpened()) { // check if we succeeded
        cout << "not able to open" << endl;
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
