# tobii_opencv_on_qt
Integration of Tobii eye tracking and OpenCV head gesture recognition implemented on Qt platform

# Setup guide
1. [Set up QT creator that is compatible with OpenCV](https://www.youtube.com/watch?v=ox6NFWE8xg4&lc=Ughv6GFWxv8XR3gCoAEC)
2. Clone this repository.
3. Open project file (.pro) in Qt Creator.
4. Update include path for OpenCV to corresponding absolute paths.

Note that this build is based on Qt 5.4.1, so some Qt newer functions cannot be used. If running the project results in a large number of errors, it's likely that 'CONFIG += c++11' is not included in the project file.

# Explanation of interactions between C++ classes and Qml interface objects
main.cpp:
- engine (QQmlApplicationEngine) loads the interface through Target.qml
- root (QObject*) gets the engine's root object (read more online about root object)
- declares AppManager with root, which gets passed into other classes so they can get access to Qml interface objects

AppManager: 
- instantiates GestureClassifier, TobiiListener and PointerManager with root (see declaration in main.cpp)
- connects signals (nodSignal, shakeSignal, newGaze, newFixation) with their slots

GestureClassifier:
- GestureClassifier constructor: declares Qml interface objects with their **objectName** (not id)
- StartDetectingNodAndShake(): detects nod or shake and emits corresponding signals (nodSignal, shakeSignal)
- onNod(): public slot that is connected to nodSignal; invokes function of Qml interface objects defined in Target.qml
- onShake(): public slot that is connected to shakeSignal; invokes function of Qml interface objects defined in Target.qml
- onNewFixation(): public slot that is connected to newFixation signal; sets property of Qml interface objects

PointerManager:
- updatePointer(): emits newFixation signal if enough newGaze signals are gathered

TobiiListener:
- OnGazeDataEvent(): emits newGaze signal
- OnFixationDataEvent(): emits newGaze signal
- note that the onGaze(gaze) slot is not connected to any signals here (see connection in Andrew's version)
