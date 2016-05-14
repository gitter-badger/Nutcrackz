#version 130
//The fragment shader operates on each pixel in a given polygon

in vec2 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;

//This is the 3 component float vector that gets outputted to the screen
//for each pixel.
out vec4 color;

uniform float intensity;

void main()
{
	float distance = length(fragmentUV);
	color = vec4(fragmentColor.rgb, fragmentColor.a * degrees(acos(dot(fragmentUV, normalize(fragmentUV)))) * (pow(intensity, distance) - 0.01));
}