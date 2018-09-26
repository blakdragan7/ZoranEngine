#version 450
uniform sampler2D tex;
in vec2 coords_c;
in vec3 color;
out vec4 FragColor;

void main()
{
	FragColor = vec4(color,1.0);//texture2D(tex,coords_c);
}