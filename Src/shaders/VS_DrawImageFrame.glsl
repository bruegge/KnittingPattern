#version 430 core

layout (location = 0) in vec3 position;

out vec2 uvCoordinates;

void main()
{
	gl_Position = vec4(position,1);
	uvCoordinates = vec2(0,0);
	if(gl_VertexID == 0)
		uvCoordinates = vec2(0,1);
	if(gl_VertexID == 1)
		uvCoordinates = vec2(1,1);
	if(gl_VertexID == 2)
		uvCoordinates = vec2(1,0);

	if(gl_VertexID == 3)
		uvCoordinates = vec2(0,1);
	if(gl_VertexID == 4)
		uvCoordinates = vec2(1,0);
	if(gl_VertexID == 5)
		uvCoordinates = vec2(0,0);
	
}
