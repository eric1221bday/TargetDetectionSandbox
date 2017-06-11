#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc/imgproc.hpp>

int main() {
    std::cout << "Hello, World!" << std::endl;

    cv::VideoCapture cap;
    int thresh = 50;
    int fontFace = CV_FONT_NORMAL;
    cap.open(0);

    if (!cap.isOpened()) {
        std::cerr << "***Could not initialize capturing...***\n";
        std::cerr << "Current parameter's value: \n";
        return -1;
    }

    cv::Mat frame;
    cv::Mat frame_gray;
    while(1){
        cap >> frame;
        if(frame.empty()){
            std::cerr<<"frame is empty"<<std::endl;
            break;
        }

        cv::cvtColor(frame, frame_gray, CV_BGR2GRAY);
        cv::GaussianBlur(frame_gray, frame_gray, cv::Size(9, 9), 2, 2);

        cv::Mat canny_output;
        std::vector<std::vector<cv::Point>> contours;

        cv::Canny( frame_gray, canny_output, thresh, thresh*2, 3 );
        findContours( canny_output, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

        std::vector<std::vector<cv::Point>> circles;
        std::vector<cv::Rect> circle_bounds;
        for (int i = 0; i < contours.size(); i++) {
            double area = cv::contourArea(contours[i]);
            cv::Rect bounds = cv::boundingRect(contours[i]);
            if ( std::abs(area - (CV_PI*(bounds.width/2)*(bounds.height/2))) < 10) {
                circles.push_back(contours[i]);
                circle_bounds.push_back(bounds);
            }
        }

        std::vector<cv::Point> concentrics;
        for (auto bound : circle_bounds) {
            for (auto other : circle_bounds) {
                if (bound != other) {
                    cv::Point center1 = cv::Point(bound.x + bound.width/2, bound.y + bound.height/2);
                    cv::Point center2 = cv::Point(other.x + other.width/2, other.y + other.height/2);
                    double dist = cv::norm(center1 - center2);
                    if (dist < std::min(bound.width/50, other.width/50)) {
                        concentrics.push_back(center1);
                        std::cout << bound.width*bound.height << std::endl;
                    }
                }
            }
        }

        for (auto concentric : concentrics) {
            cv::circle(frame, concentric, 10, cv::Scalar(255, 0, 0), 3);
            std::ostringstream stringStream;
            stringStream << "x: " << concentric.x << ", y: " << concentric.y;
            cv::putText(frame, stringStream.str(), concentric + cv::Point(-100, -20), fontFace, 0.5, cv::Scalar(0, 0, 0));
        }

//        for (auto bound : circle_bounds) {
//            cv::circle(frame, cv::Point(bound.x + bound.width/2, bound.y + bound.height/2), 5, cv::Scalar(255, 0, 0), 3);
//        }

//        for (int i = 0; i < circles.size(); i++) {
//            cv::drawContours(frame, circles, i, cv::Scalar(0, 0, 255));
//        }

        cv::imshow("", frame);
        cv::waitKey(10);
    }

    return 1;

    return 0;
}