#version 450
uniform sampler2D tex;
in vec2 coords_c;
out vec4 FragColor;

void main()
{
	FragColor = texture2D(tex,coords_c);
}