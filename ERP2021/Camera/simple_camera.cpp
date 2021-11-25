// simple_camera.cpp
// MIT License
// Copyright (c) 2019 JetsonHacks
// See LICENSE for OpenCV license and additional information
// Using a CSI camera (such as the Raspberry Pi Version 2) connected to a 
// NVIDIA Jetson Nano Developer Kit using OpenCV
// Drivers for the camera and OpenCV are included in the base image

#include <iostream>
#include <iomanip>
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
// #include <opencv2/videoio.hpp>
// #include <opencv2/highgui.hpp>

using namespace std;

std::string gstreamer_pipeline (int capture_width, int capture_height, int display_width, int display_height, int framerate, int flip_method) {
    return "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)" + std::to_string(capture_width) + ", height=(int)" +
           std::to_string(capture_height) + ", format=(string)NV12, framerate=(fraction)" + std::to_string(framerate) +
           "/1 ! nvvidconv flip-method=" + std::to_string(flip_method) + " ! video/x-raw, width=(int)" + std::to_string(display_width) + 
           ", height=(int)" + std::to_string(display_height) + ", format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
}

std::string my_pipeline(int capture_width, int capture_height, int display_width, int display_height, int framerate, int flip_method) {
    return "gst-launch-1.0 nvarguscamerasrc sensor_id=0 sensor-mode=5 ! appsink";
}

bool saveFrame(cv::VideoCapture& cap, cv::Mat& img, cv::Mat& output, int a) {
    if (!cap.read(img)) {
        std::cout<<"Capture read error"<<std::endl;
        return false;
    }

    cap.grab();
    cap.retrieve(output);
    std::string path = "/home/armandt/Desktop/project2021/nvgst/Images/";
    bool result = cv::imwrite(path + to_string(a) + ".jpg", output);
    cv::imshow("CSI Camera",img);

    return result;
}

int main()
{
    int capture_width = 1280 ;
    int capture_height = 720 ;
    int display_width = 1280 ;
    int display_height = 720 ;
    int framerate = 60 ;
    int flip_method = 0 ;

    cv::Mat outputImage(1280, 720, CV_8UC3);

    std::string pipeline = gstreamer_pipeline(capture_width,
                                              capture_height,
                                              display_width,
                                              display_height,
                                              framerate,
                                              flip_method);
    // pipeline = "nvarguscamerasrc sensor_id=0 sensor-mode=5 ! appsink";
    std::cout << "Using pipeline: \n\t" << pipeline << "\n";
 
    cv::VideoCapture cap(pipeline, cv::CAP_GSTREAMER);
    system("i2cset -y 6 0x0c 18 192"); 
    
    if(!cap.isOpened()) {
        std::cout<<"Failed to open camera."<<std::endl;
        return (-1);
    }

    cv::namedWindow("CSI Camera", cv::WINDOW_AUTOSIZE);
    cv::Mat img;

    std::cout << "Hit ESC to exit" << "\n" ;

    int runs = 0;
    std::string path = "/home/armandt/Desktop/project2021/nvgst/Images/";
    while(true)
    {
        runs++;

        saveFrame(cap, img, outputImage, runs);

    	// if (!cap.read(img)) {
        //     std::cout<<"Capture read error"<<std::endl;
        //     break;
	    // }

        // cap.grab();
        // cap.retrieve(outputImage);
        // cv::imwrite(path + to_string(runs) + ".jpg", outputImage);
        // cv::imshow("CSI Camera",img);

        int keycode = cv::waitKey(1) & 0xff ; 
            if (keycode == 27) break ;
        
        if (runs > 160) {
            break;
        }
    }

    cap.release();
    cv::destroyAllWindows() ;
    return 0;
}