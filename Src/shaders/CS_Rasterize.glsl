#version 430 core

layout (local_size_x = 8, local_size_y = 8, local_size_z = 1) in;
layout (binding = 0) uniform sampler2D inputImage;
layout (binding = 1, rgba32f) uniform image2D outputImage;

shared vec3 color[64];

void main()
{
	ivec2 outputSize = imageSize(outputImage);
	vec2 texCoord = vec2(gl_GlobalInvocationID.xy) / vec2(8,8) / vec2(outputSize);
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
		vec4 result = vec4(color[0], 1);
		imageStore(outputImage, ivec2(gl_WorkGroupID.xy), result + vec4(0,11111,1111,1111));
	}
}
