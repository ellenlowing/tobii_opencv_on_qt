#include "MainWindow.h"
#include "GestureClassifier.h"
#include "TobiiListener.h"
#include "AppManager.h"
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QMetaObject>
#include <QQmlContext>
#include <QObject>
#include <QMetaType>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QQmlApplicationEngine engine;
    engine.load("../SelectionWithNodAndSelection/Target.qml");
    QObject *root = engine.rootObjects()[0];

    qRegisterMetaType<SamplePoint>("SamplePoint");
    qRegisterMetaType<FixationPoint>("FixationPoint");

    AppManager appManager(root);
    return a.exec();
}
