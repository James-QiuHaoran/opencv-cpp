/*
 *  fileio.cpp
 *
 *  Created on: Mar 8, 2018
 *  Author: lkang
 */

#include "fileio.h"
#include "dirent.h"
using namespace std;

namespace FileIO {

void loadPointPolygonInputFile(const string &address, vector<Point> &points1, vector<Point> &points2) {
    int numPoints = 0;
    ifstream inFile;
    inFile.open(address);
    cout << "Test input file: " << address << endl;
    inFile >> numPoints;
    for (int i = 0; i < numPoints; i++) {
        double x, y;
        inFile >> x;
        inFile >> y;
        points1.push_back(Point(x, y));
    }
    inFile >> numPoints;
    for (int i = 0; i < numPoints; i++) {
        double x, y;
        inFile >> x;
        inFile >> y;
        points2.push_back(Point(x, y));
    }
    inFile.close();
}

vector<string> loadPointPolygonOutputFile(const string &address) {
    vector<string> results;

    ifstream inFile;
    inFile.open(address);
    cout << "Test output file: " << address << endl;
    string result;
    while (inFile >> result)
        results.push_back(result);
    return results;
}

// input / output file have the same format
void loadConvexHullFile(const string& address, vector<Point> &points) {
  int numPoints = 0;
  ifstream inFile;
  inFile.open(address);
  cout << "File address: " << address << endl;
  inFile >> numPoints;
  for (int i = 0; i < numPoints; i++) {
    double x, y;
    inFile >> x;
    inFile >> y;
    points.push_back(Point(x, y));
  }
  inFile.close();
}

vector<string> GetFileName(string directory) {
    DIR *dir;
    struct dirent *ent;
    vector<string> nameList;
    if ((dir = opendir (directory.c_str())) != NULL) {
      /* print all the files and directories within directory */
      while ((ent = readdir (dir)) != NULL) {
         //printf ("%s\n", ent->d_name);
         if (ent->d_type == DT_REG)
             nameList.push_back(ent->d_name);
      }
      return nameList;
      closedir (dir);
    } else {
      /* could not open directory */
      perror ("");
    }
}

//get the local file path
std::string GetCurrentWorkingDir() {
  char buff[FILENAME_MAX];
  GetCurrentDir( buff, FILENAME_MAX );
  std::string current_working_dir(buff);
  return current_working_dir;
}

//creat a new directory with name file_path
void creatDir(string file_path){
    std::string dir = "mkdir -p " + GetCurrentWorkingDir() + file_path;
    const int dir_err = system(dir.c_str());
    if (-1 == dir_err) {
        printf("Error creating directory!n");
        exit(1);
    }
}

}
