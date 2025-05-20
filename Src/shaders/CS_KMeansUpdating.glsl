#version 430 core

layout (local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

layout(std430, binding = 0) buffer ColorTableSSBO
{	
	ivec4 ssbo_colorTable[];
};

uniform int u_countColors;

void main()
{
	if(gl_GlobalInvocationID.x < u_countColors)
	{
		ivec4 color = ssbo_colorTable[gl_GlobalInvocationID.x];
		color.xyz = ivec3(vec3(color.xyz) / float(color.w));
		color.w = 0;
		ssbo_colorTable[gl_GlobalInvocationID.x] = color;	
	}
}
