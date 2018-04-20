#include "MainWindow.h"
#include "GestureClassifier.h"
#include "TobiiListener.h"
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlComponent>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qmlRegisterType<TobiiListener>("io.qt.tobiiListener", 1, 0, "TobiiListener");
    TobiiListener listener(0, true);
    int id = qRegisterMetaType<SamplePoint>();

    GestureClassifier classifier;
    //bool nodConnected = QObject::connect(&classifier, SIGNAL(nodSignal()), &listener, SLOT(onGaze(SamplePoint(0,0))));
    bool nodConnected = QObject::connect(&classifier, SIGNAL(nodSignal()), &classifier, SLOT(printNod()));
    bool shakeConnected = QObject::connect(&classifier, SIGNAL(shakeSignal()), &classifier, SLOT(printShake()));
    classifier.Start();

    return a.exec();
}
