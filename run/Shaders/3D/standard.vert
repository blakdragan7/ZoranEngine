#version 450                          
uniform mat4 MVP;      
               
in layout(location=0)vec3 vert;
in layout(location=2)vec3 normal;                         
in layout(location=1)vec2 coords;                        
out vec2 coords_c;
out vec3 color;

void main()                           
{
	vec3 light = vec3(0.1,0.1,0.1) + vec3(-10.0,10.0,-10);
	float cosTheta = clamp(dot(normal,light),0,1);
	
	color = /*lightColor*/ vec3(1.0,1.0,1.0) * cosTheta;
	
	coords_c = coords;                                     
	gl_Position = MVP * vec4(vert,1.0);
}