#-------------------------------------------------
#
# Project created by QtCreator 2018-04-19T12:08:37
#
#-------------------------------------------------

QT       += core gui quick qml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SelectionWithNodAndSelection
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += $$PWD/include

INCLUDEPATH += C:\\OpenCV-2.4.9\\OpenCV-2.4.9\\include

LIBS += -LC:\\OpenCV-2.4.9\\OpenCV-2.4.9\\build-qt\\lib \
    -lopencv_calib3d249d \
    -lopencv_contrib249d \
    -lopencv_core249d \
    -lopencv_features2d249d \
    -lopencv_flann249d \
    -lopencv_gpu249d \
    -lopencv_highgui249d \
    -lopencv_imgproc249d \
    -lopencv_legacy249d \
    -lopencv_ml249d \
    -lopencv_nonfree249d \
    -lopencv_objdetect249d \
    -lopencv_ocl249d \
    -lopencv_photo249d \
    -lopencv_stitching249d \
    -lopencv_superres249d \
    -lopencv_ts249d \
    -lopencv_video249d \
    -lopencv_videostab249d

CONFIG += c++11
SOURCES += main.cpp\
        MainWindow.cpp \
    GestureClassifier.cpp \
    QPointFUtil.cpp \
    SamplePoint.cpp \
    Timer.cpp \
    TobiiListener.cpp \
    PointerManager.cpp \
    GazeRecalibration.cpp \
    AppManager.cpp
HEADERS  += MainWindow.h \
    GestureClassifier.h \
    QPointFUtil.h \
    SamplePoint.h \
    Timer.h \
    TobiiListener.h \
    PointerManager.h \
    GazeRecalibration.h \
    AppManager.h
win32 {
    # Copy DLLs
    DEST = $${OUT_PWD}

    !contains(QMAKE_TARGET.arch, x86_64) {
        ## Windows x86 (32bit)
        message("x86 build")
        SRC = $$PWD/lib/x86/Tobii.EyeX.Client.dll
        LIBS += -L$$PWD/lib/x86/ -lTobii.EyeX.Client
        #SRC = $$PWD\\lib\\x86\Tobii.EyeX.Client.dll
        #LIBS += -L$$PWD\\lib\\x86\-lTobii.EyeX.Client
    } else {
        ## Windows x64 (64bit)
        message("x86_64 build")
        SRC = $$PWD/lib/x64/Tobii.EyeX.Client.dll
        LIBS += -L$$PWD/lib/x64/ -lTobii.EyeX.Client
        #SRC = $$PWD\\lib\\x64\Tobii.EyeX.Client.dll
        #LIBS += -L$$PWD\\lib\\x64\-lTobii.EyeX.Client
    }

    SRC ~= s,/,\\,g
    DEST ~= s,/,\\,g

    copydata.commands = $(COPY_DIR) $$SRC $$DEST
    first.depends = $(first) copydata
    export(first.depends)
    export(copydata.commands)
    QMAKE_EXTRA_TARGETS += first copydata
}


FORMS    += MainWindow.ui

DISTFILES += \
    Target.qml
