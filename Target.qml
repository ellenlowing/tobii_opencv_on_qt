import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
//import io.qt.tobiiListener 1.0

ApplicationWindow {
    id: win
    visible: true
    width: 1600
    height: 900
    title: qsTr("Gaze target!")

    Rectangle {
        id: rect
        objectName: "rect"
        color: "#0000FF"
        x: 800
        y: 450
        width: 100
        height: 100
        visible: true

        function moveWithGaze(gazePointX, gazePointY){
            rect.x = gazePointX;
            rect.y = gazePointY;

        }

        function turnGreen(){
            rect.color= "#00FF00";
        }

        function turnRed(){
            rect.color = "#FF0000";
        }
    }



}

