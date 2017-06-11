//
// Created by Eric Fang on 6/11/17.
//

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv/cv.hpp>

int main(int argc, char *argv[]) {

    if (argc != 2) {
        std::cout << "specify video file!" << std::endl;
        return -1;
    }
    const std::string videoPath = argv[1];

    int frameNum = -1;
    int thresh = 50;

    std::cout << "openning video at: " << videoPath << std::endl;

    cv::VideoCapture video(videoPath);
    cv::namedWindow( "Display window", CV_WINDOW_AUTOSIZE);
    cv::namedWindow( "Original window", CV_WINDOW_AUTOSIZE);

    if (!video.isOpened()) {
        std::cout << "could not open video!" << std::endl;
        return -1;
    }

    CvSize videoDims = CvSize((int) video.get(CV_CAP_PROP_FRAME_WIDTH), (int) video.get(CV_CAP_PROP_FRAME_HEIGHT));
    cv::Mat frame;

    std::cout << "opened video of size " << videoDims.width << "x" << videoDims.height << std::endl;

//    cv::Ptr<cv::SimpleBlobDetector> ptrBlob = cv::SimpleBlobDetector::create();
//    std::vector<cv::KeyPoint> keypoints;

    while(1) {
        video >> frame;
        if (frame.empty()) {
            std::cout << "video ended!" << std::endl;
            break;
        }

        ++frameNum;

        cv::resize(frame, frame, cvSize(videoDims.width/4, videoDims.height/4));
        cv::GaussianBlur(frame, frame, cv::Size(9, 9), 2, 2);

        cv::Mat hsvFrame;
        cv::cvtColor(frame, hsvFrame, cv::COLOR_BGR2HSV);

        cv::Mat segmentedFrame;
        cv::inRange(hsvFrame, cv::Scalar(160, 20, 250), cv::Scalar(175, 255, 255), segmentedFrame);

        std::vector<std::vector<cv::Point>> contours;
        cv::Mat canny_output;
        cv::Canny( segmentedFrame, canny_output, thresh, thresh*2, 3 );
        findContours( canny_output, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );
        for (int i = 0; i < contours.size(); i++) {
            cv::drawContours(frame, contours, i, cvScalar(0, 0, 255));
        }

//        ptrBlob->detect(frame, keypoints);

//        cv::Mat drawnFrame;
//        cv::drawKeypoints(frame, keypoints, drawnFrame, cvScalar(0, 0, 255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

//        keypoints.clear();

        cv::imshow("Display window", segmentedFrame);
        cv::imshow("Original window", frame);
//        std::cout << frameNum << std::endl;
        cv::waitKey(1);
    }

    return 0;
}