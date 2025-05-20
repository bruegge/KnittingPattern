#version 430 core

layout (local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

layout(std430, binding = 0) buffer InputBufferSSBO
{	
	ivec4 ssbo_inputBuffer[];
};


layout(std430, binding = 1) buffer WorkBufferSSBO
{	
	ivec4 ssbo_workBuffer[];
};

uniform ivec2 u_workBufferSize;
uniform ivec2 u_inputBufferSize;
uniform ivec2 u_mirror;

shared ivec3 color[64];

uint getInputBufferIndex(uint x, uint y)
{
	return u_inputBufferSize.x * y + x;	
}

uint getWorkBufferIndex(uint x, uint y)
{
	return u_workBufferSize.x * y + x;	
}

uint getWorkBufferIndex()
{
	uint x = gl_WorkGroupID.x;
	uint y = gl_WorkGroupID.y;
	if(u_mirror.x == 1)
	{
		x = gl_NumWorkGroups.x - x - 1;
	}
	if(u_mirror.y == 1)
	{
		y = gl_NumWorkGroups.y - y - 1;
	}
	return getWorkBufferIndex(x, y);
}

void main()
{
	ivec2 texCoord = ivec2(vec2(gl_GlobalInvocationID.xy) / vec2(8,8) / vec2(u_workBufferSize) * u_inputBufferSize);

	color[gl_LocalInvocationIndex] = ssbo_inputBuffer[getInputBufferIndex(texCoord.x, texCoord.y)].xyz;
		
	barrier();

	if(gl_LocalInvocationIndex == 0)
	{
		ivec3 colorOut = ivec3(0,0,0);
		for(uint i = 0; i < 64; ++i)
		{
			colorOut += color[i];
		}
		colorOut = colorOut / 64;
		ssbo_workBuffer[getWorkBufferIndex()] = ivec4(color[0], 1);
	}
}
