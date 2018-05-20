#version 450
uniform sampler2D tex;
in vec2 coords_c;
out vec4 FragColor;

void main()
{
	FragColor = vec4(0.0,0.0,1.0,1.0);//texture2D(tex,coords_c);
}