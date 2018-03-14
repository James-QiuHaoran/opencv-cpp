/*
 * fileio.h
 *
 * Created on: Mar 8, 2018
 * Author: lkang
 */

#ifndef SRC_FILEIO_H_
#define SRC_FILEIO_H_

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <fstream>

#include "headers.h"
#include "utility.h"

using namespace std;

namespace FileIO {

void loadPointPolygonInputFile(const string &address, vector<Point> &points1, vector<Point> &points2);
vector<string> loadPointPolygonOutputFile(const string &address);
void loadConvexHullFile(const string& address, vector<Point> &points);

vector<string> GetFileName(string directory);
std::string GetCurrentWorkingDir();
void creatDir(string file_path);

}

#endif /* SRC_FILEIO_H_ */
