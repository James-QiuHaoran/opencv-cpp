# Documentation - stop-sign-detection.cpp

## Overview

The code takes an image of stop sign as "prototype" and could detect another image of stop sign by OpenCV. It creates a pyramid out of the "prototype" by downsampling (read more [here](https://en.wikipedia.org/wiki/Pyramid_(image_processing))). Then it slides the pyramid one slice at a time over the target image by putting them in each location, computing the mean square error. The cropping with the smallest mean square error is determined to be the stop sign.

The code is converted from its Python version [here](https://github.com/mbasilyan/Stop-Sign-Detection).

## Steps to Run the Program

The following steps show how to run this program in Eclipse.

1. Download the code. In Eclipse, create a new project and import the code into the source file of the project.
2. In Project -> Properties, include OpenCV libraries and use `-std=c++11`.
3. Build the project and a binary file will be created in the project directory.
4. Enter the directory consisting the binary file (stop-sign-detection). Execute `./stop-sign-detection /path/to/target/image /path/to/prototype/image` and the output image will show up in a while.

In case you want to test the correctness of this program, you could download the data set which contains 100 pictures with stop signs and 100 pictures without stop signs. Execute the following one line command in terminal:

```
for i in {1..200}; do ./stop-sign-detection stop_sign_dataset/$i.jpg stop_sign_prototype.png; done
```

After execution, the execution time, minimum MSE, and the detection result will be printed in the terminal. If found, a rectangle will be draw in the image, otherwise, "Stop sign not found!" will be printed.

## How It Works?

```
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
```

This function takes two images in `Mat` format and computes the mean square error of the two. Simply speaking, it represents the difference between two images. The lower the `mse`, the more similar the first image is compared with the second. We keep a threshold to distinguish images with a stop-sign and without any stop sign.

- `absdiff(Mat img1, Mat img2, Mat dest)` in opencv2/core.hpp library computes the per-element absolute difference between two Mat arrays. See documentation [here](https://docs.opencv.org/java/2.4.9/org/opencv/core/Core.html#absdiff(org.opencv.core.Mat,%20org.opencv.core.Mat,%20org.opencv.core.Mat)).
- `convertTo(Mat img, int rtype)` converts an array to another type. By default, the type of Mat would be `CV_8U` (unsigned 8bit/pixel - i.e. a pixel can have values 0-255, this is the normal range for most image and video formats). However, we cannot multiply on one byte large values. Thus we need to convert to type `CV_32F` (the pixel can have any value between 0-1.0, this is useful for some sets of calculations on data, e.g. multiplication).
- `Mat::mul(Mat img)` gives us the per-element product of two array and `sum(Mat img)` gives us the summation of all the elements respective to three channels. 
- Mean square error will be the summation of square difference in three channels divided by the number of pixels in three channels.

```
    // resize the image
    int width = 500;
    int height = width * targetImage.rows / targetImage.cols;
    resize(targetImage, targetImage, Size(width, height));
```

After get the prototype image and the target image, we first resize the target image to have width 500 in order to relax our computation.

```
    int minMSE = INT_MAX;
    int location[4] = {0, 0, 0, 0};
```

We first initialize the minimum MSE to be `INT_MAX`. Then we iteratively resize the "tmpImg" to be prototype image in descending size with scale 1.3. In each iteration, we take a embedded loop to search the cropped region of target image, updating the location with higher similarity with the prototype image.

```
    Mat tmpImg = prototypeImg.clone();
    Mat window;
    for (int wsize = prototypeImg.cols; wsize > 15; wsize /= 1.3) {
        if (tmpImg.rows < 15 || tmpImg.cols < 15)
            break;
        if (tmpImg.rows > 900 || tmpImg.cols > 900) {
            resize(tmpImg, tmpImg, Size(wsize, wsize)); 
            continue;
        }
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
        resize(tmpImg, tmpImg, Size(wsize, wsize));  // continue to create the next prototype pyramid
    }
```

After getting the location of the stop sign, we draw a rectangle to mark it. Here, `x` and `y` stand for the location of the point at left-top corner, `w` and `h` represent width and height respectively.

```
    int buff1 = 50;
    int x = maxBox[0];
    int y = maxBox[1];
    int w = maxBox[2];
    int h = maxBox[3];

    // draw the rectangle
    rectangle(targetImage, Point(x-buff1/2, y-buff1/2), Point(x+w+buff1/2, y+h+buff1/2), Scalar(0,255,0), 2);
    // show the image
    imshow("image", targetImage);
```

## Tuning Parameters

There are several critical parameters in this program detecting stop signs:

- `THRESHOLD` - given the algorithm runs well, this parameter should be the threshold of distinguishing images with stop signs and images without stop signs. It can be set after we run a huge amount of tests and choose a suitable value that do not yield much precision.
- The descending scale - it determines the execution time. The larger the scale is, the faster the program runs but the result would not be good if scale is larger than some value.
- The minimum pyramid value - it determines when the for-loop generating pyramids of prototype image will stop. Similar to the descending scale, it should be cautiously chosen by tuning with the test results.
- Iterative step of searching match in the target image - similar to descending scale, the result will be worse if step is too large but if step is too small the program will take a lot of time.

It's believed that these parameters should be tuned with the help of machine learning and large amounts of test cases. Currently the program will give a decision in about 1-2 seconds with correct rate around 70%.
