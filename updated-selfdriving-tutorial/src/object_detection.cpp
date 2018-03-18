/*
 * detectObjCascade.cpp
 *
 *  Created on: Feb 21, 2018
 *      Author: wei
 */

#include "object_detection.h"
#include "utility.h"
#include "fileio.h"

namespace ObjectDetection {

/*Rectangle target object*/
void draw_locations(Mat & img, vector< Rect > &locations, const Scalar & color, string text) {
	if (locations.empty()) return;
	for(size_t i = 0 ; i < locations.size(); ++i){
    rectangle(img, locations[i], color, 3);
    putText(img, text, Point(locations[i].x+1, locations[i].y+8), FONT_HERSHEY_DUPLEX, 0.3, color, 1);
  }
}

vector<Rect> detectObject(const Mat& img, CascadeClassifier& classifer) {
  vector<Rect> objects_found;
  classifer.detectMultiScale(img, objects_found, 1.1, 2, CASCADE_SCALE_IMAGE, Size(10, 10));
  return objects_found;
}

bool detectObjCascade(const string& model_file, const string& folder, const string& file) {
  string path = folder + file;
  cout<<"detect objects in image:"<<path<<endl;
  Mat targetImage = imread(path);
  if (!targetImage.data) {
    cout<<"no image found:"<<path<<endl;
    return false;
  }
  CascadeClassifier stop_sign_classifier;
  stop_sign_classifier.load(model_file);


  double start = (double) getTickCount(); // start time
  vector<Rect> stop_sign_found = detectObject(targetImage, stop_sign_classifier);
  double duration = (double) getTickCount() - start; // time duration

  cout << "Time taken : " << (duration*1000./cv::getTickFrequency()) << " ms" << endl;

  draw_locations(targetImage, stop_sign_found, Scalar(0, 0, 255), "Stop Sign");

  string output_address = FileIO::GetCurrentWorkingDir() + "/output/" + file;
  imwrite(output_address, targetImage);
  cout << "Output image to:" << output_address << endl;
  return stop_sign_found.size() > 0;

}

/* Computes Mean Square Error between two n-d matrices.*/
/* Lower value means more similar*/
double meanSquareError(const Mat &img1, const Mat &img2) {
    Mat s1;
    absdiff(img1, img2, s1);   // |img1 - img2|
    s1.convertTo(s1, CV_32F);  // cannot make a square on 8 bits
    s1 = s1.mul(s1);           // |img1 - img2|^2
    Scalar s = sum(s1);        // sum elements per channel
    double sse = s.val[0] + s.val[1] + s.val[2];  // sum channels
    double mse = sse / (double)(img1.channels() * img1.total());
    return mse;
}

/*Detect stop sign with simple method by comparing MSE*/
void detectStopSignMSE(string model_address,string obj_address, string filename) {
  // set threshold of detection a stop sign
  int THRESHOLD = 7100;
  cout<<"Read image from: "<<obj_address + filename<<endl;
    Mat targetImage = imread(obj_address + filename, IMREAD_COLOR);

    // resize the image
    int width = 500;
    int height = width * targetImage.rows / targetImage.cols;
    resize(targetImage, targetImage, Size(width, height));
    // read prototype image
    Mat prototypeImg = imread(model_address, IMREAD_COLOR);

    int minMSE = INT_MAX;
    int location[4] = {0, 0, 0, 0};
    // start time
    int t0 = clock();
    //Mat tmpImg;
    Mat tmpImg = prototypeImg.clone();
    Mat window;
    for (int wsize = tmpImg.cols; wsize > 15; wsize /= 1.3) {
        if (tmpImg.rows < 15 || tmpImg.cols < 15)
          break;
        if (tmpImg.rows > 900 || tmpImg.cols > 900) {
          resize(tmpImg, tmpImg, Size(wsize, wsize));
          continue;
        }
        //cout << "Image pyramid width: " << wsize << " height: " << wsize << endl;
        for (int y = 0; y < targetImage.rows; y += 5) {
            for (int x = 0; x < targetImage.cols; x += 5) {
              if (x + tmpImg.cols > targetImage.cols || y + tmpImg.cols > targetImage.rows)
                  continue;
                Rect R(x, y, tmpImg.cols, tmpImg.cols); // create a rectangle
                window = targetImage(R); // crop the region of interest using above rectangle
                if (window.rows != tmpImg.rows || window.cols != tmpImg.cols)
                    continue;
                double tempSim = meanSquareError(tmpImg, window);
                if (tempSim < minMSE) {
                    minMSE = tempSim;
                    location[0] = x;
                    location[1] = y;
                    location[2] = tmpImg.rows;
                    location[3] = tmpImg.cols;
                }
            }
        }
        resize(tmpImg, tmpImg, Size(wsize, wsize));
    }

    // end time
    int t1 = clock();

    cout << "Execution time: " << (t1 - t0)/double(CLOCKS_PER_SEC) << " s" << endl;
    cout << "Minimum MSE value: " << minMSE << endl;
    if (minMSE < THRESHOLD) {
      int buff1 = 50;
      int x = location[0];
      int y = location[1];
      int w = location[2];
      int h = location[3];
      // draw the rectangle
      rectangle(targetImage, Point(x-buff1/2,y-buff1/2), Point(x+w+buff1/2,y+h+buff1/2), Scalar(0,255,0), 2);
      cout << "Stop sign found!" << endl;
    } else {
      cout << "Stop sign not found!" << endl;
    }

    // show the image
    imshow("image", targetImage);
    waitKey(200);
    FileIO::creatDir("/obj_detect_output_MSE");
    string output_address = FileIO::GetCurrentWorkingDir() + "/obj_detect_output_MSE/output" + filename;
    imwrite(output_address, targetImage);
    cout << "Detected Images Locate: " << output_address << endl;
}

} /* namespace detectObjCascade */
