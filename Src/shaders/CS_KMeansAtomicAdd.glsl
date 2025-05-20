#version 430 core

layout (local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

layout(std430, binding = 0) buffer AllColorsSSBO
{	
	ivec4 ssbo_inputImage[];
};

layout(std430, binding = 1) buffer ColorTableSSBO
{	
	ivec4 ssbo_colorTable[];
};

uniform int u_countAllColors;
uniform int u_countColors;

void main()
{
	if(gl_GlobalInvocationID.x < u_countAllColors)
	{
		ivec4 color = ssbo_inputImage[gl_GlobalInvocationID.x];
		if(color.a < u_countColors)
		{
			atomicAdd(ssbo_colorTable[color.a].x, color.x);
			atomicAdd(ssbo_colorTable[color.a].y, color.y);
			atomicAdd(ssbo_colorTable[color.a].z, color.z);
			atomicAdd(ssbo_colorTable[color.a].w, 1);
		}
	}
}
