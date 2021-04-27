#version 450 core
out vec4 fragColor;

in vec2 uv;

uniform sampler2D sceneText;
uniform int fx;

const int FX_NONE = 0;
const int FX_GRAYSCALE_AVG = 1;
const int FX_GRAYSCALE_WEIGHTED = 2;
const int FX_NEGATIVE = 3;
const int FX_WAVE = 4;

uniform float time;

void main() {
    if (FX_NONE == fx) {
        fragColor = texture(sceneText, uv);
    } else if (FX_GRAYSCALE_AVG == fx) {
        vec4 color = texture(sceneText, uv);
        float avg = (color.r + color.g + color.b) / 3.0f;
        fragColor = vec4(avg, avg, avg, color.a);
    } else if (FX_GRAYSCALE_WEIGHTED == fx) {
        vec4 color = texture(sceneText, uv);
        float weigth = (0.3f*color.r + 0.59f*color.g + 0.11f*color.b);
        fragColor = vec4(weigth, weigth, weigth, color.a);
    } else if (FX_NEGATIVE == fx) {
        vec4 color = texture(sceneText, uv);
        fragColor = 1.f - color;
    } else if (FX_WAVE == fx) {
        vec2 uvwave = uv;
        uvwave.x += sin(uvwave.y * 20.f + time) / 100.f;
        fragColor = texture(sceneText, uvwave);
    }
}