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
    QQmlContext *context = engine.rootContext();
    engine.load("../SelectionWithNodAndSelection/Target.qml");
    QObject *root = engine.rootObjects()[0];
    QObject *rect = root->findChild<QObject*>("rect");

    qRegisterMetaType<SamplePoint>("SamplePoint");
    qRegisterMetaType<FixationPoint>("FixationPoint");

    AppManager appManager(root);
    return a.exec();
}
