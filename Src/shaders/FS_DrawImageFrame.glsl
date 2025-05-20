#version 450
layout(std430, binding = 0) buffer InputBufferSSBO
{	
	ivec4 ssbo_workBuffer[];
};

layout(std430, binding = 1) buffer ColorTableSSBO
{	
	ivec4 ssbo_colorTable[];
};

uniform ivec2 u_bufferSize;
uniform int u_countColors;
uniform vec4 u_gridColor;
uniform vec4 u_gridHighlightColor;

uint getBufferIndex(uint x, uint y)
{
	return u_bufferSize.x * y + x;
}

out vec4 color;
in vec2 uvCoordinates;

void main()
{
	vec2 pixelPositionFloat = uvCoordinates * vec2(u_bufferSize);
	ivec2 pixelPosition = ivec2(pixelPositionFloat);
	
	float diffX = dFdx(pixelPositionFloat.x);
	float diffY = dFdy(pixelPositionFloat.y);
	
	ivec2 pixelPositionDiff = ivec2(pixelPositionFloat + vec2(diffX, diffY));
		
	{
		uint workBufferIndex = getBufferIndex(pixelPosition.x, pixelPosition.y);
		vec4 colorWorkBuffer = vec4(ssbo_workBuffer[workBufferIndex]);
		float minDistance = 9999;
		vec3 nearestColor = vec3(ssbo_colorTable[0].xyz);
		int nearestColorIndex = 0;
		for(int i = 0; i < u_countColors; ++i)
		{
			vec3 currentColor = vec3(ssbo_colorTable[i].xyz);
			float currentDistance = length(colorWorkBuffer.xyz - currentColor);

			if(currentDistance < minDistance)
			{
				minDistance = currentDistance;
				nearestColor = currentColor;
				nearestColorIndex = i;
			}
		}
		ssbo_workBuffer[workBufferIndex].a = nearestColorIndex;
		color = vec4(nearestColor / 255.0, 1);
	}

	if(pixelPositionDiff != pixelPosition) //drawing raster
	{
		if(mod(pixelPositionDiff.x,10) == 0 || mod(pixelPositionDiff.y,10) == 0)
		{
			color = (1- u_gridHighlightColor.a) * color + u_gridHighlightColor.a * u_gridHighlightColor;
		}
		else
		{
			color = (1- u_gridColor.a) * color + u_gridColor.a * u_gridColor;
		}
	}

}