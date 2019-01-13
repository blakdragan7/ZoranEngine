#version 450
uniform vec3 matColor;

in vec2 coords_c;
in vec3 lightColor;
out vec4 FragColor;

void main()
{
	FragColor = vec4(lightColor,1.0) * vec4(matColor,1.0);
}