#version 450

in vec2 p;
out vec4 color;

uniform sampler2D SDF;
uniform float pxRange = 8.0;
uniform vec4 fontColor = vec4(1.0,1.0,1.0,1.0);
uniform vec3 borderColor = vec3(0.0,0.0,0.0);
uniform vec3 shadowColor = vec3(0.0,0.0,0.0);
uniform vec2 shadowVector;
uniform float shadowSoftness;
uniform float shadowOpacity;
uniform float thickness;
uniform float border;

float linearStep(float a, float b, float x) {
    return clamp((x-a)/(b-a), 0.0, 1.0);
}

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void main()
{
    vec2 msdfSize = vec2(textureSize(SDF, 0));
    float pxSize = min(0.5/pxRange*(fwidth(p.x)*msdfSize.x+fwidth(p.y)*msdfSize.y), 0.25);
    vec3 msd = texture(SDF, p).rgb;
    float sd = (2.0 * msd.r) - 1.0 + thickness;

	// border
	float inside = linearStep(-border-pxSize, -border+pxSize, sd);
    float outsideBorder = border > 0.0 ? linearStep(+border-pxSize, +border+pxSize, sd) : 1.0;
	vec4 fg = vec4(mix(borderColor, fontColor.rgb, outsideBorder), inside);

	// shadow
	msd = texture(SDF, p-(shadowVector * (1.0 / textureSize(SDF, 0)))).rgb;
    sd = (2.0*msd.r)-1.0 + border+thickness;
    float shadow = shadowOpacity*linearStep(-shadowSoftness-pxSize, +shadowSoftness+pxSize, sd);

	fg.a *= fontColor.a;

    color = vec4(mix(shadowColor, fg.rgb, fg.a), shadow-shadow*fg.a+fg.a);
}

/*uniform sampler2D MSDF;

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
