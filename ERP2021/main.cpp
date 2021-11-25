#include "mainwindow.h"
#include <QApplication>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include "./JetsonGPIO/include/JetsonGPIO.h"
#include <chrono>
#include <thread>
#include <map>
#include <string>
//#include "./Camera/Camera.h"
//#include "./Matrix/Matrix.h"

using namespace std;
using namespace cv;
bool setFlag = false;     //is true when the area has been set
int tlX;
int tlY;
int brX;
int brY;

void clickEvent(int event, int x, int y, int flags, void* userdata) {
    if (event == cv::EVENT_LBUTTONDOWN) {
        tlX = x;
        tlY = y;
    }

    if (event == cv::EVENT_LBUTTONUP) {
        brX = x;
        brY = y;
        setFlag = true;
    }
}

void setROIonStartup() {
    int capture_width = 720;
    int capture_height = 480;
    int display_width = 720;
    int display_height = 480;
    int framerate = 17;
    int flip_method = 0;

    string pipeline = Camera::gstreamer_pipeline(capture_width,
                                          capture_height,
                                          display_width,
                                          display_height,
                                          framerate,
                                          flip_method);

//    cout << "Using pipeline: \n\t" << pipeline << "\n";
    cv::VideoCapture* cap = new cv::VideoCapture(pipeline, cv::CAP_GSTREAMER);
    system("i2cset -y 6 0x0c 18 192");      //set the focus to 300 (8cm)
    cout << "Focal distance set.\n";

    if(!cap->isOpened()) {
        string e = "Failed to open camera.\n";
        throw e;
    }

    cv::namedWindow("CSI Camera", cv::WINDOW_AUTOSIZE);
    cv::setMouseCallback("CSI Camera", clickEvent, NULL);
    cv::moveWindow("CSI Camera", 0, 0);

    cout << "Hit ESC to exit" << "\n";
    cv::Mat img;

    while(!setFlag) {
        if (!cap->read(img)) {
            std::cout<<"Capture read error"<<std::endl;
            break;
        } else {
            cv::imshow("CSI Camera", img);
            int keycode = cv::waitKey(1) & 0xff;
        }
    }

    cap->release();             //close the stream
    cv::destroyAllWindows();    //close the window
    delete cap;
    return;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    try {
        GPIO::setmode(GPIO::BOARD);
        GPIO::setup(12, GPIO::OUT, GPIO::HIGH);
        setROIonStartup();
        GPIO::output(12, GPIO::LOW);
        roiRect interestBlock = {tlX, tlY, brX, brY};
        w.roiCoordinates = interestBlock;        //copy values to the mainwindow class
        w.roiSet = true;
    } catch (string e) {
        cout << e;
    }
    cout << "ROI left corner: [" << tlX << "," << tlY << "] to [" << brX << "," << brY << "]\n";
    cout << "Finished taking pictures. \n";


//    GPIO::setmode(GPIO::BOARD);
//    GPIO::setup(33, GPIO::OUT, GPIO::HIGH); //pwm pin
//    GPIO::setup(12, GPIO::OUT, GPIO::HIGH); //led pin
//    GPIO::setup(23, GPIO::OUT, GPIO::HIGH); //standby pin
//    GPIO::setup(21, GPIO::OUT, GPIO::HIGH); //direction pin 1
//    GPIO::setup(19, GPIO::OUT, GPIO::LOW);  //direction pin 2
//    GPIO::PWM p(33, 1000);
//    auto val = 10.0;
//    p.start(val);

//    this_thread::sleep_for(chrono::seconds(4));

//    p.stop();
//    GPIO::output(12, GPIO::LOW);
//    GPIO::cleanup();

    w.show();



    return a.exec();
}
