#include <iostream>
#include <ctime>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

// threshold of detection a stop sign
int THRESHOLD = 7100;

// computes mean square error between two n-d matrices (same size).
// lower mse means higher similarity
static double meanSquareError(const Mat &img1, const Mat &img2) {
    Mat s1;
    absdiff(img1, img2, s1);   // |img1 - img2|
    s1.convertTo(s1, CV_32F);  // cannot make a square on 8 bits
    s1 = s1.mul(s1);           // |img1 - img2|^2
    Scalar s = sum(s1);        // sum elements per channel
    double sse = s.val[0] + s.val[1] + s.val[2];  // sum channels
    double mse = sse / (double)(img1.channels() * img1.total());
    return mse;
}

int main(int argc, char** argv) {
    // read target image
    Mat targetImage = imread(argv[1]);
    cout << "Target Image: " << argv[1] << endl;

    // resize the image
    int width = 500;
    int height = width * targetImage.rows / targetImage.cols;
    resize(targetImage, targetImage, Size(width, height));
    // read prototype image
    Mat prototypeImg = imread(argv[2]);

    cout << "Prototype Image: " << argv[2] << endl;

    int minMSE = INT_MAX;
    int location[4] = {0, 0, 0, 0};

    // start time
    int t0 = clock();

    Mat tmpImg = prototypeImg.clone();
    Mat window;
    cout << tmpImg.rows << " " << tmpImg.cols << endl;
    for (int wsize = tmpImg.cols; wsize > 15; wsize /= 1.3) {
        if (tmpImg.rows < 15 || tmpImg.cols < 15)
        	break;
        if (tmpImg.rows > 900 || tmpImg.cols > 900) {
        	resize(tmpImg, tmpImg, Size(wsize, wsize));
        	continue;
        }
        cout << "Image pyramid width: " << wsize << " height: " << wsize << endl;
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

    cout << "Execution time: " << (t1 - t0)/double(CLOCKS_PER_SEC)*1000 << " ms" << endl;
    cout << "Minimum MSE: " << minMSE << endl;
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
    waitKey(3000);

    return 0;
}
