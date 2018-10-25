#version 450

uniform sampler2D image;
in vec2 coords;
out vec4 FragColor;

void main()
{
	FragColor = texture2D(image,coords);
}