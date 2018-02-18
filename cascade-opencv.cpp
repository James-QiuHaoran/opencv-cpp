#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

#define STOP_SIGN_CASCADE_NAME "/home/jamesqiu/Desktop/640-Project/stopsign_classifier2.xml"
#define TRAFFIC_LIGHT_CASCADE_NAME "/home/jamesqiu/Desktop/640-Project/trafficlight_classifier2.xml"
#define VEHICLE_CASCADE_NAME "/home/jamesqiu/Desktop/640-Project/cars3.xml"
#define PEDESTRIAN_CASCADE_NAME "/home/jamesqiu/Desktop/640-Project/pedestrian_classifier4.xml"
#define TARGET_IMAGE "/home/jamesqiu/Desktop/640-Project/test.jpg"
#define CAR_IMAGE "/home/jamesqiu/Desktop/640-Project/car.png"
#define WINDOW_NAME "WINDOW - Detection Results"

void draw_locations(Mat & img, vector< Rect > & locations, const Scalar & color,string text);

int main(int argc, char** argv) {
    cout << "OpenCV Version: " << CV_VERSION << endl;
    CascadeClassifier cars, traffic_light, stop_sign, pedestrian;
    vector<Rect> cars_found, traffic_light_found, stop_sign_found , pedestrian_found, cars_tracking;
    vector<Mat> cars_tracking_img;
    vector<int> car_timer;
    string address = TARGET_IMAGE;
    Mat targetImage;
    if (argc > 1) {
    	address = (string)(argv[1]);
    }
    targetImage = imread(address);
    cout << "Target Image: " << address << endl;

    cars.load(VEHICLE_CASCADE_NAME);
    traffic_light.load(TRAFFIC_LIGHT_CASCADE_NAME);
    stop_sign.load(STOP_SIGN_CASCADE_NAME);
    pedestrian.load(PEDESTRIAN_CASCADE_NAME);

    // Start and end times
    time_t start, end;
    time(&start);

    // cars cascade
    cars.detectMultiScale(targetImage, cars_found, 1.1, 5, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
    // traffic lights cascade
    traffic_light.detectMultiScale(targetImage, traffic_light_found, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
    //pedestrian cascade
    pedestrian.detectMultiScale(targetImage, pedestrian_found, 1.1, 1, 0 | CASCADE_SCALE_IMAGE, Size(20,50));
    //stop sign cascade
    stop_sign.detectMultiScale(targetImage, stop_sign_found, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

    // End Time
    time(&end);
    // Time elapsed
    double seconds = difftime(end, start);
    cout << "Time taken : " << seconds << " seconds" << endl;

    // draw_locations(targetImage, cars_found, Scalar(0, 255, 0),"Car");
    draw_locations(targetImage, traffic_light_found, Scalar(0, 255, 255),"Traffic Light");
    draw_locations(targetImage, stop_sign_found, Scalar(0, 0, 255), "Stop Sign");
    draw_locations(targetImage, pedestrian_found, Scalar(255, 0, 0),"Pedestrian");

    imshow(WINDOW_NAME, targetImage);
    waitKey(3000);
    address = "/home/jamesqiu/Desktop/output-images/" + address;
    imwrite(address, targetImage);

    return 0;
}

void draw_locations(Mat & img, vector< Rect > &locations, const Scalar & color, string text) {
    Mat img1, car, carMask ,carMaskInv,car1;
    img.copyTo(img1);

    if (!locations.empty()) {
        // double distance;
        for(size_t i = 0 ; i < locations.size(); ++i) {
            rectangle(img, locations[i], color, -1);
        }
        addWeighted(img1, 0.8, img, 0.2, 0, img);

        for(size_t i = 0 ; i < locations.size() ; ++i) {
            rectangle(img, locations[i], color, 3);
            putText(img, text, Point(locations[i].x+1, locations[i].y+8), FONT_HERSHEY_DUPLEX, 0.3, color, 1);
            if (text == "Car") {
                locations[i].y = locations[i].y - img.rows/2; // shift the bounding box
            }
        }
    }
}
