#version 450

uniform sampler2D image;
uniform vec4 tint = vec4(0.0,0.0,0.0,0.0);
in vec2 coords;
out vec4 FragColor;
uniform int hasTexture = 1;
void main()
{
	FragColor = mix(vec4(0,0,0,1.0),texture2D(image,coords),hasTexture) + tint; 
}