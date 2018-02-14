/* Author: James Qiu
   Feb 10, 2018

   Usage: ./stop-sign-detection /path/to/target/image /path/to/prototype/image
*/

#include <iostream>
#include <ctime>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

// computes mean square error between two n-d matrices.
// lower -> more similar
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

    int maxSim = 50000;
    int maxBox[4] = {0, 0, 0, 0};

    // start time
    int t0 = clock();
    Mat tmpImg;
    Mat window;
    for (int wsize = prototypeImg.cols; wsize > 50; wsize /= 1.5) {
        if (wsize == prototypeImg.cols) {
            tmpImg = prototypeImg.clone();
        } else {
            width /= 1.5;
            height = width;
            resize(tmpImg, tmpImg, Size(width, height));
        }
        if (tmpImg.rows < 50 || tmpImg.cols < 50)
            break;
        if (tmpImg.rows > 1000 || tmpImg.cols > 1000)
            continue;
        for (int y = 0; y < targetImage.rows; y += 2) {
            for (int x = 0; x < targetImage.cols; x += 2) {
            	if (x + tmpImg.cols > targetImage.cols || y + tmpImg.cols > targetImage.rows)
            	    continue;
                Rect R(x, y, tmpImg.cols, tmpImg.cols); // create a rectangle
                window = targetImage(R); // crop the region of interest using above rectangle
                if (window.rows != tmpImg.rows || window.cols != tmpImg.cols)
                    continue;
                double tempSim = meanSquareError(tmpImg, window);
                if (tempSim < maxSim) {
                    maxSim = tempSim;
                    maxBox[0] = x;
                    maxBox[1] = y;
                    maxBox[2] = tmpImg.rows;
                    maxBox[3] = tmpImg.cols;
                }
            }
        }
    }

    // end time
    int t1 = clock();

    cout << "Execution time: " << (t1 - t0)/double(CLOCKS_PER_SEC)*1000 << "s " << endl;
    cout << maxSim << endl;
    cout << maxBox << endl;
    int buff1 = 10;
    int x = maxBox[0];
    int y = maxBox[1];
    int w = maxBox[2];
    int h = maxBox[3];

    // draw the rectangle
    rectangle(targetImage, Point(x-buff1/2, y-buff1/2), Point(x+w+buff1/2, y+h+buff1/2), Scalar(0,255,0), 2);
    // show the image
    imshow("image", targetImage);
    int k = waitKey();
    return 0;
}
