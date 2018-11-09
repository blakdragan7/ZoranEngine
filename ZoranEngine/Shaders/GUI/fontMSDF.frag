#version 450

in vec2 p;
out vec4 color;
uniform sampler2D MSDF;
uniform float pxRange = 8.0;
uniform vec4 bgColor = vec4(1.0,0.0,0.0,0.0);
uniform vec4 fgColor = vec4(1.0,1.0,1.0,1.0);

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void main()
{
    vec2 msdfUnit = pxRange/vec2(textureSize(MSDF, 0));
    vec3 sampleR = texture(MSDF, p).rgb;
    float sigDist = median(sampleR.r, sampleR.g, sampleR.b) - 0.5;
    sigDist *= dot(msdfUnit, 0.5/fwidth(p));
    float opacity = clamp(sigDist + 0.5, 0.0, 1.0);
    color =  mix(bgColor, fgColor, opacity);
}

/*uniform sampler2D MSDF;
uniform float pxRange;
uniform float thickness;
uniform float border;
uniform vec3 shadowColor;
uniform vec2 shadowVector;
uniform float shadowSoftness;
uniform float shadowOpacity;
uniform vec3 bottomColor;
uniform vec3 topColor;
uniform vec3 borderColor;

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

    color = vec4(mix(shadowColor, fg.rgb, fg.a), shadow-shadow*fg.a+fg.a);
}*/
