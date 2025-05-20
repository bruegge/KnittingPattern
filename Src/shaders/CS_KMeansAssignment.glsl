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
		float minLength = 99999.9;
		int minIndex = 0;
		for(int i = 0; i < u_countColors; ++i)
		{
			float currentLength = length(vec3(color.xyz - ssbo_colorTable[i].xyz));
			if(currentLength < minLength)
			{
				minLength = currentLength;
				minIndex = i;
			}
		}
		ssbo_inputImage[gl_GlobalInvocationID.x].a = minIndex;
	}
}
