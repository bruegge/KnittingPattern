#version 430 core

uniform sampler2D inputImage;


out vec4 color;
in vec2 uvCoordinates;

void main()
{	
	vec4 colorImage = texture(inputImage, uvCoordinates);
	ivec2 size = textureSize(inputImage, 0);
	if(size.x > 50)
		color = vec4(colorImage.rgb * 1000, 1);	
	else
		color = vec4(1,0,0,1);
}