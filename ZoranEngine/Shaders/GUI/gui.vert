#version 450                          
uniform mat4 MVP;      
               
in layout(location=0)vec3 vert;         
in layout(location=1)vec2 uv;                        
out vec2 coords;

void main()
{
	gl_Position = MVP * vec4(vert,1.0);
	coords = uv;   
}
