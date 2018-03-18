/*
 * detectObjCascade.h
 *
 *  Created on: Feb 21, 2018
 *      Author: wei
 */
#include "headers.h"
#include <stdlib.h>
#ifndef SRC_ObjectDetection_H_
#define SRC_ObjectDetection_H_

using namespace cv;
using namespace std;

namespace ObjectDetection {

  vector<Rect> detectObject(const Mat& img, CascadeClassifier& classifer);

	void draw_locations(Mat & img, vector< Rect > &locations, const Scalar & color, string text);
	bool detectObjCascade(const string& model, const string& folder, const string& file_name);

	double meanSquareError(const Mat &img1, const Mat &img2);
	void detectStopSignMSE(string model_address,string obj_address, string filename) ;

} /* namespace detectObjCascade */

#endif /* SRC_DETECTOBJCASCADE_H_ */
