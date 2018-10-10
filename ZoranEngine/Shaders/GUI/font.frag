#version 450

uniform sampler2D MSDF;
uniform float pxRange = 2.0;
uniform float thickness = 0.0;
uniform float border = 0.125;
uniform vec2 shadowVector = vec2(+0.0625, -0.03125);
uniform float shadowSoftness = 0.5;
uniform float shadowOpacity = 0.5;
uniform vec3 bottomColor = vec3(1.0, 1.0, 1.0);
uniform vec3 topColor = vec3(1.0, 1.0, 1.0);
uniform vec3 borderColor = vec3(1.0,0.0,0.0);

in vec2 p;
out vec4 color;

float linearStep(float a, float b, float x) {
    return clamp((x-a)/(b-a), 0.0, 1.0);
}

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void main() {
    vec2 msdfSize = vec2(textureSize(MSDF, 0));
    float pxSize = min(0.5/pxRange*(fwidth(p.x)*msdfSize.x+fwidth(p.y)*msdfSize.y), 0.25);

    vec3 msd = texture(MSDF, p).rgb;
    float sd = 2.0*median(msd.r, msd.g, msd.b)-1.0 + thickness;
    float inside = linearStep(-border-pxSize, -border+pxSize, sd);
    float outsideBorder = border > 0.0 ? linearStep(+border-pxSize, +border+pxSize, sd) : 1.0;

    vec4 fg = vec4(mix(borderColor, mix(bottomColor, topColor, p.y), outsideBorder), inside);

    msd = texture(MSDF, p-shadowVector).rgb;
    sd = 2.0*median(msd.r, msd.g, msd.b)-1.0 + border+thickness;
    float shadow = shadowOpacity*linearStep(-shadowSoftness-pxSize, +shadowSoftness+pxSize, sd);

    color = vec4(mix(vec3(1.0,0.0,0.0), fg.rgb, fg.a), shadow-shadow*fg.a+fg.a);
}