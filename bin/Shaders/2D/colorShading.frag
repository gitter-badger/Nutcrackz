#version 130

in vec2 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;

out vec4 color;

uniform sampler2D _texture;

void main()
{
	vec4 texColor = texture(_texture, fragmentUV);

	color = texColor * fragmentColor;
}