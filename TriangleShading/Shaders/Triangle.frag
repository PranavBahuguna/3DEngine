#version 330

in vec4 vCol;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D thisTexture;

void main()
{
	color = texture(thisTexture, TexCoord) * vCol;
}