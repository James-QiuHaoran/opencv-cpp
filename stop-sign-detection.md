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
