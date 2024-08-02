import QtQuick 2.12
import QtQml 2.12

Item {
    id: guiMonitorDisplay

    //   property var  size: Qt.size(width,height)
    property real hue: 0.0
    property real saturation: 1.0
    property real brightness: 1.0
    property real filterWidth: 10
    property bool colorKiller: true
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
                   adjustZoom (wheel.angleDelta.y / 120);
               }
           }

        function adjustZoom(val) {
            if (val > 0) { myScale = myScale / 1.1 }
            else { myScale = myScale * 1.1 }
            if (myScale < 1.0) { allowScanLines = false } else { allowScanLines = true; }
        }


    }

    Rectangle {

        width: 280*2+16;
        height: 192*2
        color: "white"
        anchors.fill: parent

        Image {
            source: "file:///develop/git/vinace/src/Trinitron3.png"
            anchors.centerIn:  parent

            scale: 0.44 * effect.scale
        }

      //  onWidthChanged: { console.log("Rect width changed to " + width); }

        Image {
            id: img
//            width: (280*2+16)
//            height: 192*2
            smooth: true
visible: false
            anchors.centerIn: parent

            // Assume some Image Provider class, liveImageProvider which provides
            //   image://live/someLiveImage
            //   see: https://forum.qt.io/topic/98284/dynamically-update-qml-image

            //        source: "image://live/someLiveImage"
            //        function reload() {
            //            counter = !counter
            //            source = "image://live/someLiveImage?id="+counter
            //        }

            // This is tuned for a 576x192 or 576x384 image.  The important thing is an 8-pixel
            // buffer either side of the 560 pixel wide Apple outout.

            source:   //"file:///develop/git/vinace/src/buffer2.png"
                      "image://crt/rendered"


            function reload() {
                counter = counter+1
                source = "image://crt/rendered?id=" + counter

            }

            property int counter: 0
            Timer {
                id: refreshTimer
                running: guiMonitorDisplay.displayTimerRunning
                onTriggered: { img.reload();  /*console.log("Timer triggered");*/  }
                interval: 1000/31
                repeat: true
            }
 }
//            layer.enabled: true
//            layer.effect:
                ShaderEffect {
                    anchors.centerIn: parent
visible:true
                id: effect
           //     width: (280*2+16);
            //    height: 384
                width: source.width
                height: source.height
                scale: mouseArea.myScale

                property var  size: Qt.size(width,height)
                property var  source: img
                property real hue: guiMonitorDisplay.hue
                property real saturation: guiMonitorDisplay.saturation
                property real brightness: guiMonitorDisplay.brightness
                property real filterWidth: guiMonitorDisplay.filterWidth
                property bool colorKiller: guiMonitorDisplay.colorKiller
                property bool killBottom: guiMonitorDisplay.killBottom
                property bool showScanLines: mouseArea.allowScanLines ? guiMonitorDisplay.showScanLines : false
                property real monoRed: guiMonitorDisplay.monoRed
                property real monoGreen: guiMonitorDisplay.monoGreen
                property real monoBlue: guiMonitorDisplay.monoBlue
                property real scanLineIntensity: guiMonitorDisplay.scanLineIntensity

                onWidthChanged: { console.log("Image Width changed to " + width); }
                fragmentShader:
                    "//  Simple NTSC Decoder
        //
        //  Decodes composite video signal generated in Buffer A.
        //	Simplified fork of an earlier NTSC decoder shader.
        //
        //  copyright (c) 2017-2019, John Leffingwell
        //  Modifications (c) 2020, Mark Long
        //  license CC BY-SA Attribution-ShareAlike

        const   float PI  = 3.14159265358979323846;
        const   float TAU = PI * 2.0;

        varying highp vec2 qt_TexCoord0;

        uniform vec2 size;
        uniform sampler2D source;
        uniform bool colorKiller;
        uniform bool killBottom;
        uniform bool showScanLines;
        uniform float monoRed;
        uniform float monoGreen;
        uniform float monoBlue;
        uniform float scanLineIntensity;

        //  TV-like adjustments
        uniform float saturation;      //  Saturation / 'Color' (normally 1.0)
        uniform float hue;             //  Hue / 'Tint' (normally 0.0)
        uniform float brightness;      //  Brightness (normally 1.0)

        //  Filter parameters
        uniform float filterWidth;
                int   iFilterWidth = int(filterWidth);   //  Filter Width
                int   M   = iFilterWidth/2;              //  Filter Middle
        const   float FC  = 0.125;                       //  Frequency Cutoff
        const   float SCF = 0.25;                        //  Subcarrier Frequency



        //	Colorspace conversion matrix for YIQ-to-RGB
        const   mat3 YIQ2RGB = mat3(1.0000, 1.0000, 1.0000,
                                    0.9563,-0.2721,-1.1070,
                                    0.6210,-0.6474, 1.7046);


        //	TV-like adjustment matrix for Hue, Saturation, and Brightness
        vec3 adjust(vec3 YIQ, float H, float S, float B) {
                mat3 M = mat3(  B,      0.0,      0.0,
                              0.0, S*cos(H),  -sin(H),
                              0.0,   sin(H), S*cos(H) );
            return M * YIQ;
        }

        //	Hann windowing function
        float hann(float n, float N) {
            return 0.5 * (1.0 - cos((TAU*n)/(N-1.0)));
        }


        //	Sinc function
        float sinc(float x) {
            if (x == 0.0) return 1.0;
            return sin(PI*x) / (PI*x);
        }


        void main()
        {
            vec2 uv = qt_TexCoord0;
            vec2 fragCoord = uv.xy * size;

            vec4 monoVal = vec4(monoRed,monoGreen,monoBlue,1);

            float oddline = mod(fragCoord.y, 2.0);


            if (colorKiller)
            {
                if (showScanLines && oddline > 1.0)
                   gl_FragColor = vec4(scanLineIntensity,scanLineIntensity,scanLineIntensity,1)
                                * vec4(texture2D(source, uv).rgb, 1.0) * monoVal * brightness;
                else
                   gl_FragColor = vec4(texture2D(source, uv).rgb, 1.0) * monoVal * brightness;
            }
            else if (killBottom && uv.y >= 20.0/24.0)
            {
                if (showScanLines && oddline > 1.0)
                   gl_FragColor = vec4(scanLineIntensity,scanLineIntensity,scanLineIntensity,1)
                                * vec4(texture2D(source, uv).rgb, 1.0) * monoVal * brightness;
                else
                   gl_FragColor = vec4(texture2D(source, uv).rgb, 1.0) * monoVal * brightness;
            }
            else
            {
                //  Compute sampling weights
                float weights[256];
                float sum = 0.0;
                for (int n=0; n<iFilterWidth; n++) {
                    weights[n] = hann(float(n), float(iFilterWidth)) * sinc(FC * float(n-M));
                    sum += weights[n];
                }

                //  Normalize sampling weights
                for (int n=0; n<iFilterWidth; n++) {
                    weights[n] /= sum;
                }

                //	Sample composite signal and decode to YIQ
                vec3 YIQ = vec3(0.0);
                for (int n=0; n<iFilterWidth; n++) {
                    vec2 pos = uv + vec2(float(n-M) / size.x, 0.0);
                    float phase = (TAU * SCF * size.x) * pos.x+0.0;
                    YIQ += vec3(1.0, cos(phase), -sin(phase)) * texture2D(source, pos).rgb * weights[n];
                }

                //  Apply TV adjustments to YIQ signal and convert to RGB
                vec4 potentialColor = vec4(YIQ2RGB * adjust(YIQ, hue, saturation, brightness), 1.0) ;

                if (showScanLines && oddline > 1.0)
                {
                    gl_FragColor = vec4(scanLineIntensity,scanLineIntensity,scanLineIntensity,1)
                                 * potentialColor;
                }
                else
                {
                    gl_FragColor = potentialColor;
                }
            }
        }"
            }



    }
}
