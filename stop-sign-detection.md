# Documentation - stop-sign-detection.cpp

## Overview

The code takes an image of stop sign as "prototype" and could detect another image of stop sign by OpenCV. It creates a pyramid out of the "prototype" by downsampling (read more [here](https://en.wikipedia.org/wiki/Pyramid_(image_processing))). Then it slides the pyramid one slice at a time over the target image, computing the mean square error. The slice with the smallest mean square error is determined to be the stop sign.

The code is converted from its Python version [here](https://github.com/mbasilyan/Stop-Sign-Detection).

## Steps to Run the Program

The following steps show how to run this program in Eclipse.

1. Download the code. In Eclipse, create a new project and import the code into the source file of the project.
2. In Project -> Properties, include OpenCV libraries and use `-std=c++11`.
3. Build the project and a binary file will be created in the project directory.
4. Enter the directory consisting the binary file (stop-sign-detection). Execute `./stop-sign-detection /path/to/target/image /path/to/prototype/image` and the output image will show up in a while.

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

This function takes two images in `Mat` format and computes the mean square error of the two. Simply speaking, it represents the difference between two images. The lower the `mse`, the more similar the first image is compared to the second.

```
    // resize the image
    int width = 500;
    int height = width * targetImage.rows / targetImage.cols;
    resize(targetImage, targetImage, Size(width, height));
```

After get the prototype image and the target image, we first resize the target image to have width 500.

```
    int maxSim = 50000;
    int maxBox[4] = {0, 0, 0, 0};
```

We first initialize the maximum similarity to be 50000 (large enough). Then we iteratively resize the "tmpImg" to be prototype image in descending size with scale 1.5. In each iteration, we take a embedded loop to search the cropped region of target image, updating the location with maximum similarity with the prototype image.

```
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
```

After get the location of the stop sign, we draw a rectangle to mark it. Here, `x` and `y` stand for the location of the point at left-top corner, `w` and `h` represent width and height respectively.

```
    int buff1 = 10;
    int x = maxBox[0];
    int y = maxBox[1];
    int w = maxBox[2];
    int h = maxBox[3];

    // draw the rectangle
    rectangle(targetImage, Point(x-buff1/2, y-buff1/2), Point(x+w+buff1/2, y+h+buff1/2), Scalar(0,255,0), 2);
    // show the image
    imshow("image", targetImage);
```
