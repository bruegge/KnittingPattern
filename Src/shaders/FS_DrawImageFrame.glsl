#version 450
layout(std430, binding = 0) buffer WorkBufferSSBO
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

vec3 LABtoXYZ(in vec3 lab)
{
	float var_Y = (lab.x +16.0f) / 116.0f;
	float var_X = lab.y / 500.0f + var_Y;
	float var_Z = var_Y - lab.z / 200.0f;

	if (pow(var_Y, 3.0f) > 0.008856f)
		var_Y = pow(var_Y, 3.0f);
	else
		var_Y = (var_Y - 16.0f / 116.0f) / 7.787f;
	if (pow(var_X, 3.0f) > 0.008856f)
		var_X = pow(var_X, 3.0f);
	else
		var_X = (var_X - 16.0f / 116.0f) / 7.787f;
	if (pow(var_Z, 3.0f) > 0.008856f)
		var_Z = pow(var_Z, 3.0f);
	else
		var_Z = (var_Z - 16.0f / 116.0f) / 7.787f;

	return vec3(95.047f * var_X,
		100.000f * var_Y,
		108.883f * var_Z);
}

vec3 XYZtoRGB(in vec3 xyz)
{
	float var_X = xyz.x / 100.0f;      //X from 0 to  95.047 
	float var_Y = xyz.y / 100.0f;      //Y from 0 to 100.000
	float var_Z = xyz.z / 100.0f;      //Z from 0 to 108.883

	float var_R = var_X * 3.2406f + var_Y * -1.5372f + var_Z * -0.4986f;
	float var_G = var_X * -0.9689f + var_Y * 1.8758f + var_Z * 0.0415f;
	float var_B = var_X * 0.0557f + var_Y * -0.2040f + var_Z * 1.0570f;

	if (var_R > 0.0031308f)
		var_R = 1.055f * pow(var_R,(1.0f / 2.4f)) - 0.055f;
	else
		var_R = 12.92f * var_R;
	if (var_G > 0.0031308f)
		var_G = 1.055f * pow(var_G, (1.0f / 2.4f)) - 0.055f;
	else
		var_G = 12.92f * var_G;
	if (var_B > 0.0031308f)
		var_B = 1.055f * pow(var_B, (1.0f / 2.4f)) - 0.055f;
	else
		var_B = 12.92f * var_B;

	return vec3(var_R, var_G, var_B);
}

ivec4 LABtoRGB(in ivec4 lab)
{
	vec3 xyz = LABtoXYZ(vec3(lab.xyz));
	vec3 rgb = XYZtoRGB(xyz);
	return ivec4(rgb * 255, 0);
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
			vec3 currentColor = vec3(LABtoRGB(ssbo_colorTable[i]));
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