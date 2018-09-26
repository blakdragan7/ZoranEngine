#version 450                          
uniform mat4 MVP;      
               
in layout(location=0)vec3 vert;                         
in layout(location=1)vec2 coords;                        
out vec2 coords_c;

void main()
{
	coords_c = coords;                                     
	gl_Position = MVP * vec4(vert,1.0);
}
