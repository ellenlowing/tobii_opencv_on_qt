#include <QDebug>
#include <QThread>
#include <QMetaObject>
#include <cmath>
#include <iostream>
#include "PointerManager.h"
#include "Timer.h"

using namespace std;

PointerManager::PointerManager(QObject *root, QObject *pointer, GazeRecalibration &recalibration) :
    QObject(root), root(root), pointer(pointer), recalibration(recalibration), dist(0, 50)
{}

PointerManager::~PointerManager()
{}

void PointerManager::updatePointer(SamplePoint newPosition)
{
    //newPosition.value = mapToWindow(newPosition.value);
    //emit newUnfilteredSample(newPosition.value, newPosition.tstamp);
//    if (realPointer->property("active").toBool())
//    {
//        realPointer->setProperty("centerX", newPosition.value.x());
//        realPointer->setProperty("centerY", newPosition.value.y());
//    }
//    if (false)
//    {
//        //newPosition.value += QPointF(dist(generator), dist(generator));
//        double t = Timer::timestamp();
//        newPosition.value += QPointF(0.5, 0.5)  * floor(t / 1000);
//        if (uncalibPointer->property("active").toBool())
//        {
//            uncalibPointer->setProperty("centerX", newPosition.value.x());
//            uncalibPointer->setProperty("centerY", newPosition.value.y());
//        }
//    }
//    if (filtering)
//    {
//        /*samples.push_back(newPosition);
//        QMutableListIterator<SamplePoint> it(samples);
//        while (it.hasNext())
//        {
//            if (newPosition.tstamp - it.next().tstamp > 50) it.remove();
//        }
//        newPosition.value = recalibration.remap(filteredSample());*/
//        newPosition.value = recalibration.remap(newPosition.value);
//    }
    bool added = currentFixation.addSample(newPosition);
    if (!added)
    {
        if (currentFixation.isCompleted()) {
            emit newFixation(currentFixation.getCenter(), currentFixation.getStartTstamp(), currentFixation.duration());
            //cout << "newFixation signal emitted" << endl;
            latestFixations.append(currentFixation);
        }
        currentFixation = FixationPoint(newPosition);
    }
    if (currentFixation.duration() > 0) emit incompleteFixation(currentFixation.getCenter(), currentFixation.getEndTstamp(), currentFixation.duration());
    emit newSample(currentFixation.getCenter(), currentFixation.getEndTstamp());
}

//void PointerManager::setIsMouse(bool isMouse)
//{
//    filtering = !isMouse;
//}

QList<QVariant> PointerManager::latestCompletedFixations(double interval)
{
    QList<QVariant> completedFixations;
    double curtstamp = Timer::timestamp();
    QMutableListIterator<FixationPoint> it(latestFixations);
    while (it.hasNext())
    {
        FixationPoint fixation = it.next();
        double timediff = curtstamp - fixation.getEndTstamp();
        if (timediff < interval)
        {
            completedFixations.append(QVariant::fromValue(fixation.getCenter()));
        }
        else if (timediff > 10000) it.remove();
    }
    completedFixations.append(QVariant::fromValue(currentFixation.getCenter()));
    return completedFixations;
}

//QPointF PointerManager::mapToWindow(QVariant globalPosition)
//{
//    QVariant windowCursor;
//    QMetaObject::invokeMethod(root, "mapToWindow", Q_RETURN_ARG(QVariant, windowCursor), Q_ARG(QVariant, globalPosition));
//    return windowCursor.toPointF();
//}

//QPointF PointerManager::filteredSample()
//{
//    double x = 0;
//    double y = 0;
//    foreach (SamplePoint p, samples)
//    {
//        x += p.value.x();
//        y += p.value.y();
//    }
//    return QPointF(x / samples.size(), y / samples.size());
//}
