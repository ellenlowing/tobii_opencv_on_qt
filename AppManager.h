#ifndef APPMANAGER_H
#define APPMANAGER_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QMetaObject>
#include <QQmlContext>

#include "PointerManager.h"
#include "TobiiListener.h"
#include "GestureClassifier.h"
#include "GazeRecalibration.h"

class AppManager : public QObject
{
    Q_OBJECT
public:
    explicit AppManager(QObject *root);
    ~AppManager();

signals:

public slots:

private:
    QObject *root;
    QObject *pointer;
    PointerManager pointerManager;
    TobiiListener tobiiListener;
    GazeRecalibration recalibration;
    GestureClassifier gestureClassifier;
};

#endif // APPMANAGER_H
