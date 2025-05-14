#version 430 core

layout (local_size_x = 8, local_size_y = 8, local_size_z = 1) in;
layout (binding = 0) uniform sampler2D inputImage;

layout(std430, binding = 1) buffer WorkBufferSSBO
{	
	vec4 ssbo_workBuffer[];
};

uniform ivec2 u_bufferSize;

shared vec3 color[64];


uint getBufferIndex(uint x, uint y)
{
	return u_bufferSize.x * y + x;	
}

uint getBufferIndex()
{
	return getBufferIndex(gl_WorkGroupID.x, gl_WorkGroupID.y);
}

void main()
{
	vec2 texCoord = vec2(gl_GlobalInvocationID.xy) / vec2(8,8) / vec2(u_bufferSize);
	color[gl_LocalInvocationIndex] = texture(inputImage, texCoord).xyz;
		
	barrier();

	if(gl_LocalInvocationIndex == 0)
	{
		vec3 colorOut = vec3(0,0,0);
		for(uint i = 0; i < 64; ++i)
		{
			colorOut += color[i];
		}
		colorOut = (colorOut / 64);
		vec4 result = vec4(colorOut, 1);
		ssbo_workBuffer[getBufferIndex()] = result;
	}
}
