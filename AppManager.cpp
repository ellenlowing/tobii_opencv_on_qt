#include "AppManager.h"

AppManager::AppManager(QObject *root) :
    QObject(root),
    root(root),
    pointer(root->findChild<QObject*>("pointer")),
    tobiiListener(root, true),
    gestureClassifier(root),
    pointerManager(root, pointer, recalibration)
{
    connect(&gestureClassifier, SIGNAL(nodSignal()), &gestureClassifier, SLOT(onNod()));
    connect(&gestureClassifier, SIGNAL(shakeSignal()), &gestureClassifier, SLOT(onShake()));
    connect(&tobiiListener, SIGNAL(newGaze(SamplePoint)), &pointerManager, SLOT(updatePointer(SamplePoint)));
    connect(&pointerManager, SIGNAL(newFixation(QPointF,double,double)), &gestureClassifier, SLOT(onNewFixation(QPointF)));
    //connect(&pointerManager, SIGNAL(incompleteFixation(QPointF,double,double)), &gestureClassifier, SLOT(onNewFixation(QPointF)));
    gestureClassifier.StartTracking();
}

AppManager::~AppManager()
{
    //disconnect last two connections for pointerManagers
}

