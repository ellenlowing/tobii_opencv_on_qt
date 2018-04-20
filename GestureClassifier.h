#ifndef GESTURECLASSIFIER_H
#define GESTURECLASSIFIER_H

#include <QObject>
#include <QCloseEvent>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QMetaObject>
#include <QDebug>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <time.h>

class GestureClassifier : public QObject
{
    Q_OBJECT
public:
    explicit GestureClassifier(QObject *parent = 0);
    const int WIDTH = 640;
    const int HEIGHT = 480;
    const std::string imageWindow;
    cv::String face_cascade_name = "C:/Users/IVC-HCI/Desktop/Ellen/GestureClassifierTemplate/haarcascade_frontalface_alt.xml";
    cv::String nose_cascade_name = "C:/Users/IVC-HCI/Desktop/Ellen/GestureClassifierTemplate/haarcascade_mcs_nose.xml";
    cv::CascadeClassifier face_cascade;
    cv::CascadeClassifier nose_cascade;
    cv::Mat frame;
    cv::Mat tmpl;
    cv::Mat fullTmpl;
    clock_t now;
    clock_t nodTime0;
    clock_t shakeTime0;
    double nodTicks;
    double shakeTicks;
    cv::VideoCapture capture;
    cv::Point prevLoc = cv::Point(0,0);
    cv::Point searchCenter = cv::Point(0,0);
    cv::Size searchSize = cv::Size(0,0);
    int frameCount = 0;
    bool initialized = false;
    bool nodInitialized = false;
    bool shakeInitialized = false;
    std::vector<double> Ypts;
    std::vector<double> Xpts;
    void Start();
    void detectAndDisplay(cv::Mat &frame, cv::CascadeClassifier face_cascade, cv::CascadeClassifier nose_cascade, cv::Point &searchCenter, cv::Size &searchSize);
    bool detectNod(std::vector<double> pts);
    bool detectShake(std::vector<double> pts);
    bool setTrackPoint(cv::Mat &frame, cv::Point point, cv::Point & prevLoc, cv::Mat &templ, cv::Mat &fullTempl);
    cv::Point track(cv::Mat &frame, cv::Point prevLoc, cv::Mat templ, cv::Mat fullTempl);
    cv::Point match(cv::Mat &frame, cv::Mat &tmpl, cv::Size limits, cv::Point searchCenter, cv::Size searchSize);
    cv::Mat workingFrame(cv::Mat &frame);
    cv::Point adjustPoint(cv::Point point, cv::Size limits);
    int adjustPosition(int pos, int limit);
    int myMax(int a, int b, int c);
    int myMin(int a, int b, int c);
    void mySkinDetect(cv::Mat& src, cv::Mat& dst);
    void closeEvent(QCloseEvent *event);
    bool breakLoop = false;
    ~GestureClassifier();

signals:
    void nodSignal();
    void shakeSignal();

public slots:
    void printNod();
    void printShake();

private:
    QQmlEngine engine;
    QQmlComponent *component;
    QObject *object;
};

#endif // GESTURECLASSIFIER_H
