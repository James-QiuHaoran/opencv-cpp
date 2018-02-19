#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/video.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

#define STOP_SIGN_CASCADE_NAME "/home/jamesqiu/Desktop/640-Project/stopsign_classifier2.xml"
#define TRAFFIC_LIGHT_CASCADE_NAME "/home/jamesqiu/Desktop/640-Project/trafficlight_classifier2.xml"
#define VEHICLE_CASCADE_NAME "/home/jamesqiu/Desktop/640-Project/cars3.xml"
#define TARGET_VIDEO "/home/jamesqiu/Desktop/640-Project/test.mp4"

void draw_locations(Mat & img, vector< Rect > & locations, const Scalar & color,string text);

int main(int argc, char** argv) {
    cout << "OpenCV Version: " << CV_VERSION << endl;

    CascadeClassifier cars, traffic_light, stop_sign;
	cars.load(VEHICLE_CASCADE_NAME);
	traffic_light.load(TRAFFIC_LIGHT_CASCADE_NAME);
	stop_sign.load(STOP_SIGN_CASCADE_NAME);

	vector<Rect> cars_found, traffic_light_found, stop_sign_found;
    string address = TARGET_VIDEO;
    if (argc > 1) {
    	address = (string)(argv[1]);
    }
    VideoCapture cap(address);
    if(!cap.isOpened())
    	throw runtime_error(string("can't open video file: " + address));
    else
    	cout << "Target Video: " << address << endl;

    namedWindow("Real Time Detection", 1);
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
	return 0;
}

void draw_locations(Mat & img, vector< Rect > &locations, const Scalar & color, string text) {
	if (!locations.empty()) {
        for(size_t i = 0 ; i < locations.size(); ++i){
        	if (text == "Traffic Light" && locations[i].y < img.size().height/4)
        		continue;
        	else if (text == "Car" && (locations[i].y > 3.8*img.size().height/5 || locations[i].y < img.size().height/3.5))
        		continue;
            rectangle(img, locations[i], color, 3);
            cout << locations[i] << endl;
            putText(img, text, Point(locations[i].x+1, locations[i].y+8), FONT_HERSHEY_DUPLEX, 0.3, color, 1);
        }
	}
}
