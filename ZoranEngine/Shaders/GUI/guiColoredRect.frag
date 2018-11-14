#version 450

uniform vec4 ColorA = vec4(0.0,0.0,0.0,0.0);
uniform vec4 ColorB = vec4(0.0,0.0,0.0,0.0);

in vec4 alpha;
out vec4 FragColor;

void main()
{
	FragColor = mix(ColorA,ColorB,vec4(alpha)); 
}