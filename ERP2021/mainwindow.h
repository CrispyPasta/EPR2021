#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <deque>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip>
#include <signal.h>
#include <fstream>
#include <opencv2/core.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <qtconcurrentrun.h>
#include "JetsonGPIO.h"
#include "./Camera/Camera.h"
#include "./PID/PID.h"
#include "./Utility/Utility.h"
#include "./Utility/worker.h"

using namespace std;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    deque<int> rpmArray;
    deque<int> durationArray;
    roiRect roiCoordinates;        //gets values from main
    bool roiSet = false;

    void parseRPMs(QString s);

    void parseDurations(QString s);

    void printOutput(QString s);


private slots:
    void on_startButton_clicked();

    void on_stopButton_clicked();

    void on_rpm_change(float rpm, int s);

    void on_pid_update();

    void on_take_picture();

signals:
    Q_SIGNAL void rpm_change(float rpm, int s);

    Q_SIGNAL void pid_change();

    Q_SIGNAL void take_picture();

private:    
    void updateImage(int filename);

    static void rotationInterruptHandler(int channel);

    static void delayMs(int m);

    static void foo();

    void lightsOn(bool o);

    static void signalHandler(int t);

    void motorControlLoop();

    void pidControlLoop();


    QPixmap* tempImage;  //this is the image that gets displayed (the photo)

    const QString projectBase = "/home/armandt/Desktop/project2021/EPR2021/EPR2021";

    int picsTaken;          //counter



//    PID* pidObj;            //motor control object

    bool PIDmode;

    int dutyCycle;

//    bool motorDone;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
