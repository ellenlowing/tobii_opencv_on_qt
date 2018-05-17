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

    Text {
        text: "Gaze matches!"
        objectName: "textObject"
        font.family: "Helvetica"
        font.pointSize: 16
        color: "#000000"
        visible: false
        x: 800
        y: 400

    }

    Rectangle {
        id: rect
        objectName: "rectObject"
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

