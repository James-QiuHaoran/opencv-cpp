# Documentation - image-detection-cascade.cpp

## Steps to Run the Program

The following steps show how to run this program in Eclipse.

1. Download the code and two cascade classifier files (.xml). In Eclipse, create a new project and import the code into the source file of the project.
2. In Project -> Properties, include OpenCV libraries and use `-std=c++11`.
3. Specify the path to two xml files in the code.
4. Build the project and a binary file will be created in the project directory.
5. Enter the directory consisting the binary file. Execute `./binary /path/to/target/image` and the output image will show up in a while.

In case you want to test the correctness of this program, you could download the test data set and uncomment the codes storing detected images (don’t to change the storage path). Go to the directory of test images and execute the following one line command in terminal:

```
for i in `ls`; do /path/to/binary $i; done
```

After execution, the execution time, and the detection result will be printed in the terminal. If found, a rectangle will be draw in the image, otherwise, nothing will be drawn or printed.

## How It Works?

```
#define STOP_SIGN_CASCADE_NAME "/home/jamesqiu/Desktop/640-Project/stopsign_classifier2.xml"
#define TRAFFIC_LIGHT_CASCADE_NAME "/home/jamesqiu/Desktop/640-Project/trafficlight_classifier2.xml"
#define TARGET_IMAGE "/home/jamesqiu/Desktop/640-Project/test.jpg"
#define WINDOW_NAME "WINDOW - Detection Results"
```

This block of codes defines the paths to xml files, the target file and our window name.

```
void draw_locations(Mat & img, vector< Rect > &locations, const Scalar & color, string text) {
    if (!locations.empty()) {
        for(size_t i = 0 ; i < locations.size(); ++i){
            rectangle(img, locations[i], color, 3);
            putText(img, text, Point(locations[i].x+1, locations[i].y+8), FONT_HERSHEY_DUPLEX, 0.3, color, 1);
        }
    }
}
```

Function `draw_locations` takes the Mat image object, a vector containing the locations of detected objects, and the color of the boundary box, and the text you want to include in the box. If the locations is not empty, it will iterate through the locations and for each location, it draws a rectangle according to the `location[i]` (a Rect object) with border color `color` and thickness `1`. At the same time, it puts text inside the box using `putText`.

```
    string address = TARGET_IMAGE;
    Mat targetImage;
    if (argc > 1) {
        address = (string)(argv[1]);
    }
    targetImage = imread(address);
    cout << "Target Image: " << address << endl;
```

Read the target image.
   
``` 
    traffic_light.load(TRAFFIC_LIGHT_CASCADE_NAME);
    stop_sign.load(STOP_SIGN_CASCADE_NAME);
```

Load cascade classifiers with the paths to those xml files.

``` 
    traffic_light.detectMultiScale(targetImage, traffic_light_found, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(10, 10));
    stop_sign.detectMultiScale(targetImage, stop_sign_found, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(10, 10));
```

Detect target objects in the target image by calling “detectMultiScale()” function. Here,

- `targetImage` is the input image;
- `traffic_light_found` is the vector of “Rect” which will contain the locations of detected objects in the image after the return of this function;
- `1.1` is the scale factor which specifies how much the image size will be reduced at each image scale;
- `2` is the minNeighbor which specifies how many neighbors each candidate rectangle should have to retain it. (E.g. 2 means each candidate location contains at least two overlapped rectangles)
- `0 | CASCADE_SCALE_IMAGE` is the flag we want the classifying process to know: OpenCV documentation suggests using `CASCADE_SCALE_IMAGE` when performing a detectMultiScale with LBP features. It is not used for new cascade classifiers.

```
    draw_locations(targetImage, traffic_light_found, Scalar(0, 255, 255), "Traffic Light");
    draw_locations(targetImage, stop_sign_found, Scalar(0, 0, 255), "Stop Sign");
    imshow(WINDOW_NAME, targetImage);
    waitKey(3000);
```

Draw rectangles and show the image.

