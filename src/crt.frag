#version 310 es
precision mediump float;

//  Simple NTSC Decoder
//
//  Decodes composite video signal generated in Buffer A.
//  Simplified fork of an earlier NTSC decoder shader.
//
//  copyright (c) 2017-2019, John Leffingwell
//  Modifications (c) 2020, Mark Long
//  license CC BY-SA Attribution-ShareAlike

// Compiled with: qsb --hlsl "100 es,120,150" --hlsl 50 --msl 12 -o crt.qsb crt.frag

const float PI = 3.14159265358979323846;
const float TAU = PI * 2.0;

layout(location=0) in vec2 qt_TexCoord0;

layout(location=0) out vec4 fragColor;

// layout(set = 0, binding = 0) uniform UBO {
//     vec2 size;
//     bool colorKiller;
//     bool killBottom;
//     bool showScanLines;
//     float monoRed;
//     float monoGreen;
//     float monoBlue;
//     float scanLineIntensity;
//     float saturation;
//     float hue;
//     float brightness;
//     float filterWidth;
// } ubo;

layout(binding = 1) uniform sampler2D source;

layout(std140, binding=0) uniform buf {
    mat4 qt_Matrix;
    float qt_Opacity;
} ubuf;


// //  Colorspace conversion matrix for YIQ-to-RGB
// const mat3 YIQ2RGB = mat3(1.0000, 1.0000, 1.0000,
//                           0.9563, -0.2721, -1.1070,
//                           0.6210, -0.6474, 1.7046);

// //  TV-like adjustment matrix for Hue, Saturation, and Brightness
// vec3 adjust(vec3 YIQ, float H, float S, float B) {
//     mat3 M = mat3(B, 0.0, 0.0,
//                   0.0, S * cos(H), -sin(H),
//                   0.0, sin(H), S * cos(H));
//     return M * YIQ;
// }

// //  Hann windowing function
// float hann(float n, float N) {
//     return 0.5 * (1.0 - cos((TAU * n) / (N - 1.0)));
// }

// //  Sinc function
// float sinc(float x) {
//     if (x == 0.0) return 1.0;
//     return sin(PI * x) / (PI * x);
// }

void main() {
    // int iFilterWidth = int(ubo.filterWidth);   //  Filter Width
    // int M = iFilterWidth / 2;                  //  Filter Middle
    // const float FC = 0.125;                    //  Frequency Cutoff
    // const float SCF = 0.25;                    //  Subcarrier Frequency

    // vec2 uv = qt_TexCoord0;
    // vec2 fragCoord = uv.xy * ubo.size;

    // vec4 monoVal = vec4(ubo.monoRed, ubo.monoGreen, ubo.monoBlue, 1);

    // float oddline = mod(fragCoord.y, 2.0);

 fragColor =
         texture(source, qt_TexCoord0) * vec4(1.0, 0.0, 0.0, 1.0) * ubuf.qt_Opacity;
    /*
    if (ubo.colorKiller) {
        if (ubo.showScanLines && oddline > 1.0)
            fragColor = vec4(ubo.scanLineIntensity, ubo.scanLineIntensity, ubo.scanLineIntensity, 1)
                         * vec4(texture(source, uv).rgb, 1.0) * monoVal * ubo.brightness;
        else
            fragColor = vec4(texture(source, uv).rgb, 1.0) * monoVal * ubo.brightness;
    } else if (ubo.killBottom && uv.y >= 20.0 / 24.0) {
        if (ubo.showScanLines && oddline > 1.0)
            fragColor = vec4(ubo.scanLineIntensity, ubo.scanLineIntensity, ubo.scanLineIntensity, 1)
                         * vec4(texture(source, uv).rgb, 1.0) * monoVal * ubo.brightness;
        else
            fragColor = vec4(texture(source, uv).rgb, 1.0) * monoVal * ubo.brightness;
    } else {
        //  Compute sampling weights
        float weights[256];
        float sum = 0.0;
        for (int n = 0; n < iFilterWidth; n++) {
            weights[n] = hann(float(n), float(iFilterWidth)) * sinc(FC * float(n - M));
            sum += weights[n];
        }

        //  Normalize sampling weights
        for (int n = 0; n < iFilterWidth; n++) {
            weights[n] /= sum;
        }

        //  Sample composite signal and decode to YIQ
        vec3 YIQ = vec3(0.0);
        for (int n = 0; n < iFilterWidth; n++) {
            vec2 pos = uv + vec2(float(n - M) / ubo.size.x, 0.0);
            float phase = (TAU * SCF * ubo.size.x) * pos.x + 0.0;
            YIQ += vec3(1.0, cos(phase), -sin(phase)) * texture(source, pos).rgb * weights[n];
        }

        //  Apply TV adjustments to YIQ signal and convert to RGB
        vec4 potentialColor = vec4(YIQ2RGB * adjust(YIQ, ubo.hue, ubo.saturation, ubo.brightness), 1.0);

        if (ubo.showScanLines && oddline > 1.0) {
            fragColor = vec4(ubo.scanLineIntensity, ubo.scanLineIntensity, ubo.scanLineIntensity, 1)
                         * potentialColor;
        } else {
            fragColor = potentialColor;
        }
    }
    */
}
