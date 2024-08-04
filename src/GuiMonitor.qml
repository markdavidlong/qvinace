import QtQuick 2.12
import QtQml 2.12

Item {
    id: guiMonitorDisplay

    property real hue: 0.0
    property real saturation: 1.0
    property real brightness: 0.4
    property real filterWidth: 10
    property bool colorKiller: false
    property bool killBottom: false
    property bool showScanLines: true
    property real monoRed: 1.0
    property real monoGreen: 1.0
    property real monoBlue: 1.0
    property real scanLineIntensity: 0.5

    property bool displayTimerRunning: true

    signal reset

    onReset: { }

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        property real myScale: 1.0
        property bool allowScanLines: true
        onWheel: {
            if (wheel.modifiers & Qt.ControlModifier) {
                adjustZoom(wheel.angleDelta.y / 120);
            }
        }

        function adjustZoom(val) {
            if (val > 0) { myScale = myScale / 1.1 }
            else { myScale = myScale * 1.1 }
            if (myScale < 1.0) { allowScanLines = false } else { allowScanLines = true; }
        }
    }

    Rectangle {
        width: 280 * 2 + 16
        height: 192 * 2
        color: "white"
        anchors.fill: parent

        Image {
            source: "qrc:/Trinitron3.png"
            anchors.centerIn: parent
            scale: 0.44 //* effect.scale
        }

        Image {
            id: img
            smooth: true
            visible: false
            anchors.centerIn: parent
            source: "file:///git/qvinace/src/buffer2.png"

            // function reload() {
            //     counter = counter + 1
            //   //  source = "image://crt/rendered?id=" + counter
            // }

            // property int counter: 0
            // // Timer {
            // //     id: refreshTimer
            // //     running: guiMonitorDisplay.displayTimerRunning
            // //     onTriggered: { img.reload(); }
            // //     interval: 1000 / 31
            // //     repeat: true
            // // }
        }

        ShaderEffectSource {
            anchors.centerIn: parent

            id: shaderSource
            sourceItem: img
            hideSource: true
            live: true
            width: img.width
            height: img.height
        }

        ShaderEffect {
            id: effect
            width: shaderSource.width
            height: shaderSource.height
            scale: mouseArea.myScale
            anchors.centerIn: parent


            // property var size: Qt.size(width, height)
            // property var source: shaderSource
            // property real hue: guiMonitorDisplay.hue
            // property real saturation: guiMonitorDisplay.saturation
            // property real brightness: guiMonitorDisplay.brightness
            // property real filterWidth: guiMonitorDisplay.filterWidth
            // property bool colorKiller: guiMonitorDisplay.colorKiller
            // property bool killBottom: guiMonitorDisplay.killBottom
            // property bool showScanLines: mouseArea.allowScanLines ? guiMonitorDisplay.showScanLines : false
            // property real monoRed: guiMonitorDisplay.monoRed
            // property real monoGreen: guiMonitorDisplay.monoGreen
            // property real monoBlue: guiMonitorDisplay.monoBlue
            // property real scanLineIntensity: guiMonitorDisplay.scanLineIntensity

           // onWidthChanged: { console.log("Image Width changed to " + width); }
            fragmentShader: "qrc:/crt.qsb"
        }
    }
}
