#version 450

uniform sampler2D image;
uniform vec4 tint = vec4(0.0,0.0,0.0,0.0);
in vec2 coords;
out vec4 FragColor;

void main()
{
	FragColor = texture2D(image,coords) * (1.0 + tint); 
}