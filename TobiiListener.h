#ifndef TOBIILISTENER_H
#define TOBIILISTENER_H

#include <QObject>
#include <QList>
//#include <QQmlEngine>
//#include <QQmlComponent>
//#include <QMetaObject>

#include "SamplePoint.h"

class GazeEmitter : public QObject
{
    Q_OBJECT
public:
    GazeEmitter(QObject *parent) : QObject(parent) {}

signals:
    void newGaze(SamplePoint gaze);
};

class TobiiListener : public QObject
{
    Q_OBJECT
    Q_PROPERTY(NOTIFY newGaze)
public:
    explicit TobiiListener();
    TobiiListener(QObject *root, bool controlling = true);
    ~TobiiListener();
    void setControlling(bool controlling);
    bool gazed = false;

signals:
    void newGaze(SamplePoint gaze);

public slots:
    void controlToggled(bool notControlling);
    void onGaze(SamplePoint gaze);

private:
    bool controlling;
//    QQmlEngine engine;
//    QQmlComponent *component;
//    QObject *object;
};

#endif // TOBIILISTENER_H
