#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QThread>

using namespace std;
using namespace chrono;
using namespace GPIO;

bool motorDone = false;
PID* pidObj = nullptr;
bool motorRunning = false;
vidStream feedObj;      //object for handling the camera
roiRect roiCords;        //gets values from main
float tRPM;
QFuture<void> fut1;
QFuture<void> fut2;
MainWindow* mainWindowptr;

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->stopButton->setEnabled(false);      //disable stop because otherwise the GPIO stuff crashes
    mainWindowptr = this;

    //add the items to the dropdown list
    ui->functionPicker->addItem("Color Classification");
    ui->functionPicker->addItem("Mixing Quantification");
    ui->functionPicker->addItem("Sedimentation Quantification");
    ui->functionPicker->addItem("Flow Tracking Function");
    ui->functionPicker->addItem("None");

    //load the temporary image
    tempImage = new QPixmap();
    tempImage->load("/home/armandt/Desktop/project2021/EPR2021/ERP2021/Images/27.jpg");
    ui->imageLabel->setBackgroundRole(QPalette::Base);
    ui->imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->imageLabel->setScaledContents(true);
    ui->imageLabel->setPixmap(*tempImage);
//    updateImage(1);

    roiSet = false;

    QObject::connect(this, &MainWindow::rpm_change, this, &MainWindow::on_rpm_change);
    QObject::connect(this, &MainWindow::pid_change, this, &MainWindow::on_pid_update);
    QObject::connect(this, &MainWindow::take_picture, this, &MainWindow::on_take_picture);
}

MainWindow::~MainWindow()
{
    delete feedObj.capture;
    delete feedObj.img;
    delete feedObj.output;
    delete ui;
}

// = = = = = = = = GENERAL UTILITY FUNCTIONS = = = = = = = = //
void MainWindow::updateImage(int filename) {
    QString tmp = "/home/armandt/Desktop/project2021/EPR2021/ERP2021/OutputImages/" + QString::number(filename) + ".jpg";
    if(tempImage->load(tmp)) {
        ui->imageLabel->setBackgroundRole(QPalette::Base);
        ui->imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        ui->imageLabel->setScaledContents(true);
        ui->imageLabel->setPixmap(*tempImage);
    } else {
        cout << "The image could not be loaded.\n Filename: " << tmp.QString::toStdString() << '\n';
    }
}

void MainWindow::lightsOn(bool o) {
    if (o) {
        GPIO::output(12, GPIO::HIGH);
    } else {
        GPIO::output(12, GPIO::LOW);
    }
}

void MainWindow::printOutput(QString s) {
    ui->outputTextEdit->append(s);
    
}

void MainWindow::parseRPMs(QString s) {
//    GPIO::setwarnings(false);
    bool done = false;

    if (s == "") {
        return;
    }

    if (s.contains('\n')) {
        done = false;
    } else {
        done = true;
    }

    QString r;
    int i;
    while (!done) {
        r = s.mid(0, s.indexOf('\n'));          //copy the text up to the newline
        i = r.toInt();
        if (abs(i) < 500) {
            if (i > 0) {
                i = 500;
            } else if (i < 0) {
                i = -500;
            }
        }
        if (abs(i) > 4500) {
            if (i > 0) {
                i = 4500;
            } else if (i < 0) {
                i = -4500;
            }
        }
        rpmArray.push_back(i);                          //add onto the list of RPMs
        s = s.mid(s.indexOf('\n') + 1);                 //make s equal to the rest of the string after the \n
//        cout << r.toStdString() << '\n';

        if (s.contains('\n')) {         //update state of doneness
            done = false;
        } else {
            done = true;
        }
    }
    i = s.toInt();
    if (abs(i) < 500) {
        if (i > 0) {
            i = 500;
        } else if (i < 0) {
            i = -500;
        }
        printOutput("Please note that RPM values of less than 500 are not supported.");
    }
    if (abs(i) > 4500) {
        if (i > 0) {
            i = 4500;
        } else if (i < 0) {
            i = -4500;
        }
        printOutput("Please note that RPM values of more than 4500 are not supported.");
    }

    rpmArray.push_back(i);
}

void MainWindow::parseDurations(QString s) {
    bool done = false;

    if (s == "") {
        return;
    }

    if (s.contains('\n')) {
        done = false;
    } else {
        done = true;
    }

    QString r;
    int i;
    while (!done) {
        r = s.mid(0, s.indexOf('\n'));          //copy the text up to the newline
        i = r.toInt();
        if (i < 0) {
            i = 0;
            printOutput("Please note that negative numbers are not allowed in the duration list.");
        }
        if (i > 900) {
            i = 900;
            printOutput("Please note that durations longer than 900 seconds are not allowed.");
        }
        durationArray.push_back(i);                          //add onto the list of RPMs
        s = s.mid(s.indexOf('\n') + 1);                 //make s equal to the rest of the string after the \n
//        cout << r.toStdString() << '\n';

        if (s.contains('\n')) {         //update state of doneness
            done = false;
        } else {
            done = true;
        }
    }
    i = s.toInt();
    if (i < 0) {
        i = 0;
    }
    if (i > 900) {
        i = 900;
    }

    durationArray.push_back(i);
}

// = = = = = = = = MOTOR CONTROL FUNCTIONS = = = = = = = = //
void MainWindow::signalHandler(int t) {
    motorDone = true;
}

void MainWindow::rotationInterruptHandler(int channel) {
    if (pidObj == nullptr) {
        return;
    }

    pidObj->rpm_interrupt_handler();
    mainWindowptr->take_picture();
//    feedObj.filename++;
//    cout << feedObj.filename << '\n';
//    Camera::saveFrame(feedObj);
//    emit(take_picture);
//    QtConcurrent::run(this, &MainWindow::pidControlLoop);
}

void MainWindow::delayMs(int m) {
    this_thread::sleep_for(chrono::milliseconds(m)); //I think this'll have to change
}

void MainWindow::motorControlLoop() {
    //loop through
    //for each item, pop it from the front
    //set the target RPM to the right value
    //delay for the corresponding time
    int targetRPM;
    int delayDuration;
    while(!rpmArray.empty()) {
        motorRunning = true;
        targetRPM = rpmArray.front();
        rpmArray.pop_front();                   //pop target RPM and duration
        delayDuration = durationArray.front();
        durationArray.pop_front();

        if(targetRPM < 0) {
            if (pidObj->clockwise) {
                pidObj->brake(500);             //brake for 500ms
            }
            pidObj->setDirection("ccw");        //set the direction to counter clockwise
        } else {
            if (!pidObj->clockwise) {
                pidObj->brake(500);             //brake for 500ms
            }
            pidObj->setDirection("cw");         //set the direction to clockwise
        }

        emit(rpm_change(targetRPM, delayDuration));        //emit from this thread back to the GUI thread
        pidObj->ChangeDutyCycle(abs(targetRPM) / 42.0);         //update motor target RPM
        std::thread dt(MainWindow::delayMs, 1000);               //delay a little bit to let the motor get close to the target RPM
        dt.join();
        tRPM = targetRPM;
        if (targetRPM != 500) {
            fut2 = QtConcurrent::run(this, &MainWindow::pidControlLoop);
        }
        std::thread delayThread(MainWindow::delayMs, delayDuration * 1000 - 1000);
        delayThread.join();
    }

    printOutput("Centrifugation complete, stopping motor.");
    fut2.pause();
    fut2.cancel();
    motorRunning = false;       //signal the pidControlLoop to stop
    pidObj->rampDown(3);
    pidObj->stop();
}

void MainWindow::pidControlLoop() {
    float Kc = 0.057;
    float Ti = 90.373;
    float Td = 11.707;
    float dtyCycle = 15;
    int refreshTime = 20;
    int runs = 0;
    while(motorRunning) {
//        cout << tRPM << '\n' << flush;
        runs++;
        pidObj->updateError(runs == 1);
        if (runs < 3) {
            dtyCycle = pidObj->pidControl(tRPM, Kc, Ti * 100, Td);  //reduce the effect of the integral that built up during the first portion
        } else {
            dtyCycle = pidObj->pidControl(tRPM, Kc, Ti, Td);
        }
//        if (tRPM < 1000) {
//            refreshtime = 20;
//        }
//        cout << dtyCycle << '\n' << flush;
        pidObj->ChangeDutyCycle(dtyCycle);
        emit pid_change();
        delayMs(refreshTime);
    }
}

void MainWindow::foo() {
    cout << "Foo called.\n" << flush;
}


// = = = = = = = = SIGNALS AND SLOTS = = = = = = = = //

void MainWindow::on_startButton_clicked()
{
    roiCords = roiCoordinates;

    ui->outputTextEdit->clear();
    QString enteredRPMs = ui->rpmTextEdit->document()->toPlainText();
    parseRPMs(enteredRPMs);
    QString enteredDurations = ui->durationTextEdit->document()->toPlainText();
    parseDurations(enteredDurations);

    //check whether the RPMs and Durations match
    if (rpmArray.size() != durationArray.size()) {
        printOutput("PLEASE ENSURE THAT THERE ARE AN EQUAL NUMBER OF RPMS AND DURATIONS ENTERED.");
        rpmArray.clear();
        durationArray.clear();
        return;
    } else {
        printOutput("RPMs and Durations successfully captured.\n");
    }

    ui->rpmTextEdit->setReadOnly(true);             //user isn't allowed to edit the instructions after things have been started.
    ui->durationTextEdit->setReadOnly(true);
    printOutput("Starting rotation and processing functions.");

    //start the camera
//    feedObj = Camera::openStream(false);
    static int picsTaken = 0;
    try {
        feedObj = Camera::openStream(false);
        for (int a = 0; a < 60; a++) {
            feedObj.filename = ++picsTaken;
            if (roiSet) {
                lightsOn(true);
                delayMs(1);
//                Camera::saveFrame(feedObj, roiCoordinates);         //try to save only the ROI
                mainWindowptr->take_picture();
                QtConcurrent::run(this, &MainWindow::updateImage, picsTaken);
//                updateImage(picsTaken);
                lightsOn(false);
            } else {
                lightsOn(true);
                delayMs(1);
                Camera::saveFrame(feedObj);
//                    lightsOn(false);
            }
            delayMs(19);
        }
    } catch (string e) {
        cout << e;
    }

    //start the motor
    //setup the motor control things
    delayMs(350);       //allow the camera to focus etc
    GPIO::setmode(GPIO::BOARD);
    pidObj = new PID();
    pidObj->start(10);
    GPIO::add_event_detect(18, GPIO::Edge::FALLING, rotationInterruptHandler, 10);      //this is the interrupt for each rotation

    pidObj->stop();
    GPIO::output(pidObj->led_control_pin, GPIO::LOW);        //make sure the lights are off

//    feedObj = Camera::openStream(false);                        //start up the camera stream
    lightsOn(true);
    fut1 = (QtConcurrent::run(this, &MainWindow::motorControlLoop));     //start spinning the motor

    //disable this button
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(true);
}

void MainWindow::on_stopButton_clicked()
{
//    fut1.pause();
//    GPIO::output(pidObj->standby_pin, GPIO::LOW);                //disable the motor
//    GPIO::output(pidObj->led_control_pin, GPIO::LOW);
//    pidObj->stop();
//    pidObj->dumpText();

    feedObj.capture->release();

    lightsOn(false);

    rpmArray.clear();
    durationArray.clear();

    ui->rpmTextEdit->setReadOnly(false);        //If the whole thing is stopped, re-enable the text box.
    ui->durationTextEdit->setReadOnly(false);
//    ui->outputTextEdit->clear();
    ui->outputTextEdit->append("\nStopping rotation and processing functions.");

    //disable this button
    ui->startButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
//    GPIO::cleanup();          //this should be un-commented!!!

    //close the camera stream
//    Camera::closeStream(feedObj);
}

void MainWindow::on_rpm_change(float rpm, int s) {     //this is a slot
    QString str = "Setting target RPM to " + QString::number(rpm) + " RPM. Maintaining this speed for " + QString::number(s) + " seconds.";
    printOutput(str);
}

void MainWindow::on_pid_update() {
    if (motorRunning) {
        ui->rpmLabel->setText(QString::number(pidObj->currentRPM));
    }
}

void MainWindow::on_take_picture() {
    feedObj.filename++;
    Camera::saveFrame(feedObj);
    updateImage(feedObj.filename);
}
