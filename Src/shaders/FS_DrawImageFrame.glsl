#version 430 core

layout(std430, binding = 0) buffer WorkBufferSSBO
{	
	vec4 ssbo_workBuffer[];
};

uniform ivec2 u_bufferSize;

uint getBufferIndex(uint x, uint y)
{
	return u_bufferSize.x * y + x;	
}

out vec4 color;
in vec2 uvCoordinates;

void main()
{	
	ivec2 pixelPosition = ivec2(uvCoordinates * vec2(u_bufferSize));
	color = ssbo_workBuffer[getBufferIndex(pixelPosition.x, pixelPosition.y)];
}