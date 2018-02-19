# Documentation - video-cascade.cpp

## Steps to Run the Program

The following steps show how to run this program in Eclipse.

1. Download the code and three cascade classifier files (.xml). In Eclipse, create a new project and import the code into the source file of the project.
2. In Project -> Properties, include OpenCV libraries and use `-std=c++11`.
3. Specify the path to three xml files in the code.
4. Build the project and a binary file will be created in the project directory.
5. Enter the directory consisting the binary file. Execute `./binary /path/to/test/video` and the output video will show up.

## How It Works?

```
#define STOP_SIGN_CASCADE_NAME "/home/jamesqiu/Desktop/640-Project/stopsign_classifier2.xml"
#define TRAFFIC_LIGHT_CASCADE_NAME "/home/jamesqiu/Desktop/640-Project/trafficlight_classifier2.xml"
#define VEHICLE_CASCADE_NAME "/home/jamesqiu/Desktop/640-Project/cars3.xml"
#define TARGET_VIDEO "/home/jamesqiu/Desktop/640-Project/test.mp4"
```

This block of codes defines the paths to xml files, the target video file and our window name.

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

Function `draw_locations` takes the Mat image object, a vector containing the locations of detected objects, and the color of the boundary box, and the text you want to include in the box. If the locations is not empty, it will iterate through the locations and for each location, it draws a rectangle according to the `location[i]` (a `Rect` object) with border color `color` and thickness `1`. At the same time, it puts text inside the box using `putText()`.

```
    string address = TARGET_VIDEO;
    if (argc > 1) {
        address = (string)(argv[1]);
    }
    VideoCapture cap(address);
    if(!cap.isOpened())
        throw runtime_error(string("can't open video file: " + address));
    else
        cout << "Target Video: " << address << endl;
```

Open the test video.

``` 
    cars.load(VEHICLE_CASCADE_NAME);
    traffic_light.load(TRAFFIC_LIGHT_CASCADE_NAME);
    stop_sign.load(STOP_SIGN_CASCADE_NAME);
```

Load cascade classifiers with the paths to those xml files.

```    
for(;;) {
    Mat frame;
    cap >> frame; // get a new frame from camera
    resize(frame, frame, Size(frame.size().width/2, frame.size().height/2), 0, 0, INTER_LINEAR);

    double t = (double) getTickCount(); // start time

    cars.detectMultiScale(frame, cars_found, 1.1, 5, 0 | CASCADE_SCALE_IMAGE, Size(10, 10));
    traffic_light.detectMultiScale(frame, traffic_light_found, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(0, 0), Size(50, 50));
    stop_sign.detectMultiScale(frame, stop_sign_found, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(0, 0), Size(50, 50));

    t = (double) getTickCount() - t; // time duration
    cout << "Time taken : " << (t*1000./cv::getTickFrequency()) << " ms" << endl;

    draw_locations(frame, cars_found, Scalar(0, 255, 0), "Car");
    draw_locations(frame, traffic_light_found, Scalar(0, 255, 255), "Traffic Light");
    draw_locations(frame, stop_sign_found, Scalar(0, 0, 255), "Stop Sign");

    imshow("Real Time Detection", frame);
    if(waitKey(30) >= 0) {
        break;
    }
}
```

In the above for loop, we iterate through the video frame by frame. For each frame, we store it in Mat format and resize it to its half size (relax the computation). And then we call `detectMultiScale` to detect target objects in this frame. Here,

- `targetImage` is the input image;
- `traffic_light_found` is the vector of `Rect` which will contain the locations of detected objects in the image after the return of this function;
- `1.1` is the scale factor which specifies how much the image size will be reduced at each image scale;
- `2` is the minNeighbor which specifies how many neighbors each candidate rectangle should have to retain it. (E.g. 2 means each candidate location contains at least two overlapped rectangles)
- `0 | CASCADE_SCALE_IMAGE` is the flag we want the classifying process to know: OpenCV documentation suggests using `CASCADE_SCALE_IMAGE` when performing a detectMultiScale with LBP features. It is not used for new cascade classifiers.
- `Size(0, 0)` - the first Size() is the minimum size a detected object can be of;
- `Size(50, 50)` - the second Size() is the maximum size a detected object can be of;

Although scale factor, minNeighbor, flag, minSize, and maxSize are all optional, it’s better tune them to suit your project and situation carefully.

In the end, draw rectangles and show the frame. After all frames are shown, break the loop.

