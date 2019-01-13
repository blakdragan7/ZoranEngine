#version 450                          
uniform mat4 MVP;      
               
in layout(location=0)vec3 vert;         
in layout(location=1)vec4 blend;                        
out vec4 alpha;

void main()
{
	gl_Position = MVP * vec4(vert,1.0);
	alpha = blend;   
}
