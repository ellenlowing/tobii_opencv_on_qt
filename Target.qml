import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import io.qt.tobiiListener 1.0

ApplicationWindow {
    id: win
    visible: true
    width: 640
    height: 480
    title: qsTr("Gaze target!")

    Rectangle {
        id: rect
        objectName: "rect"
        color: "#FF0000"
        x: 320
        y: 240
        width: 20
        height: 20
        visible: true
        signal destroyok(var a)
        onDestroyok: destroyTarget()
        function destroyTarget(){
            console.log("detected");
            rect.destroy();
        }
    }

}

