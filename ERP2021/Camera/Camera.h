#ifndef SIMPLE_CAMERA_H
#define SIMPLE_CAMERA_H

#include <iostream>
#include <iomanip>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include "./Utility/Utility.h"

using namespace std;

class Camera {
private:

public:
    Camera();
    ~Camera();

    static string gstreamer_pipeline (int capture_width, int capture_height, int display_width, int display_height, int framerate, int flip_method) {
        return "nvarguscamerasrc exposuretimerange='72000 80000' tnr-mode=2 tnr-strength=1 saturation=1.0 ! video/x-raw(memory:NVMM), width=(int)" + std::to_string(capture_width) + ", height=(int)" +
               std::to_string(capture_height) + ", format=(string)NV12, framerate=(fraction)" + std::to_string(framerate) +
               "/1 ! nvvidconv flip-method=" + std::to_string(flip_method) + " ! video/x-raw, width=(int)" + std::to_string(display_width) +
               ", height=(int)" + std::to_string(display_height) + ", format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink drop=1";
    }

    static bool saveFrame(vidStream& videoFeed) {
        if (!videoFeed.capture->read(*videoFeed.img)) {
            std::cout<<"Capture read error"<<std::endl;
            return false;
        }

        if (videoFeed.showWindow) {
            cv::imshow("CSI Camera",*videoFeed.img);
            return true;
        } else {
            videoFeed.capture->grab();
            videoFeed.capture->retrieve(*videoFeed.output);
            bool result = cv::imwrite(videoFeed.path + to_string(videoFeed.filename) + ".jpg", *videoFeed.output);
            return result;
        }
    }

    static bool saveFrame(vidStream &videoFeed, roiRect rCords) {
        if (!videoFeed.capture->read(*videoFeed.img)) {
            std::cout<<"Capture read error"<<std::endl;
            return false;
        }

        if (videoFeed.showWindow) {
            cv::imshow("CSI Camera",*videoFeed.img);
            return true;
        } else {
            videoFeed.capture->grab();
            videoFeed.capture->retrieve(*videoFeed.output);

            cv::Rect roiRectangle(rCords.topLeftX, rCords.topLeftY, rCords.btmRightX, rCords.btmRightY);
            *videoFeed.output = cv::Mat(*videoFeed.img, roiRectangle);         //the output is equal to only the region of interest

            bool result = cv::imwrite(videoFeed.path + to_string(videoFeed.filename) + ".jpg", *videoFeed.output);
            return result;
        }
    }

    static vidStream openStream(bool showViewfinder) {
        int capture_width = 720;
        int capture_height = 480;
        int display_width = 720;
        int display_height = 480;
        int framerate = 17;
        int flip_method = 0;

        //this mat object holds the frame before its saved
        cv::Mat* outputImage = new cv::Mat(720, 480, CV_8UC3);

        string pipeline = gstreamer_pipeline(capture_width,
                                              capture_height,
                                              display_width,
                                              display_height,
                                              framerate,
                                              flip_method);

        // pipeline = "nvarguscamerasrc sensor_id=0 sensor-mode=5 ! appsink";
//        cout << "Using pipeline: \n\t" << pipeline << "\n";
        cv::VideoCapture* cap = new cv::VideoCapture(pipeline, cv::CAP_GSTREAMER);
        system("i2cset -y 6 0x0c 18 192");
        cout << "Focal distance set.\n";

        if(!cap->isOpened()) {
            std::cout<<"Failed to open camera."<<std::endl;
            string e = "Failed to open camera.\n";
            throw e;
        }

        if (showViewfinder) {
            cv::namedWindow("CSI Camera", cv::WINDOW_AUTOSIZE);
            cv::moveWindow("CSI Camera", 0, 0);
        }

        //this Mat is used in the read() function
        cv::Mat* img = new cv::Mat;

        int runs = 0;
        std::string path = "/home/armandt/Desktop/project2021/EPR2021/ERP2021/OutputImages/";
        vidStream streamer = {cap, img, outputImage, runs, path};
        return streamer;
    }

    static void closeStream(vidStream& feed) {
        feed.capture->release();
    }

    static void openCamera() {
        bool showViewfinder = false;

        int capture_width = 1280 ;
        int capture_height = 720 ;
        int display_width = 1280 ;
        int display_height = 720 ;
        int framerate = 60 ;
        int flip_method = 0 ;

        //this mat object holds the frame before its saved
        cv::Mat* outputImage = new cv::Mat(1280, 720, CV_8UC3);

        string pipeline = gstreamer_pipeline(capture_width,
                                                  capture_height,
                                                  display_width,
                                                  display_height,
                                                  framerate,
                                                  flip_method);

        // pipeline = "nvarguscamerasrc sensor_id=0 sensor-mode=5 ! appsink";
        cout << "Using pipeline: \n\t" << pipeline << "\n";
        cv::VideoCapture* cap = new cv::VideoCapture(pipeline, cv::CAP_GSTREAMER);
        system("i2cset -y 6 0x0c 18 192");
        cout << "Focal distance set.\n";

        if(!cap->isOpened()) {
            std::cout<<"Failed to open camera."<<std::endl;
            return;
        }

        if (showViewfinder) {
            cv::namedWindow("CSI Camera", cv::WINDOW_AUTOSIZE);
        }

        //this Mat is used in the read() function
        cv::Mat* img = new cv::Mat;

        std::cout << "Hit ESC to exit" << "\n";

        int runs = 0;
        std::string path = "/home/armandt/Desktop/project2021/EPR2021/ERP2021/OutputImages/";
        vidStream streamer = {cap, img, outputImage, runs, path, showViewfinder};
        while(true)
        {
            runs++;
            streamer.filename = runs;       //update the number so the filename works out
            saveFrame(streamer);
            // if (!cap.read(img)) {
            //     std::cout<<"Capture read error"<<std::endl;
            //     break;
            // }

            // cap.grab();
            // cap.retrieve(outputImage);
            // cv::imwrite(path + to_string(runs) + ".jpg", outputImage);
            if (showViewfinder) {
//                cv::imshow("CSI Camera",img);
            }

            int keycode = cv::waitKey(1) & 0xff ;
            if (keycode == 27) {
                break;
            }
        }

        cap->release();
        if (showViewfinder) {
            cv::destroyAllWindows();
        }
        return;
    }
};

#endif // SIMPLE_CAMERA_H
