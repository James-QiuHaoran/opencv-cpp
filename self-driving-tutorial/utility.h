/*
 * utility
 *
 *  Created on: Sep 6, 2017
 *      Author: lkang
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "headers.h"

using namespace cv;
using namespace std;

namespace utility {

static std::random_device rd;
static std::mt19937 randomNumberGenerator(rd());
static std::uniform_int_distribution<uint32_t> uint_dist100(1,100);

vector<string> GetFileName(string directory);
std::string GetCurrentWorkingDir();
void creatDir(string file_path);

int getRandomNumber();


double getPSNR(const Mat& I1, const Mat& I2);

void convertFileToVideo(string file, double loss_percent = 0.0);
void convertFileToVideoFEC(string file, double loss_percent = 0.0);
/**
 * @param src the input MAT image, with color
 * @return the blur value, use a threshold to detect blurness
 */
double blurDetection(cv::Mat& src);

/**
 * Canny threshold test
 */
void thresh_callback(int thresh, void* gray);
int adjustTest(Mat& src);

}



#endif /* UTILITY_H_ */
