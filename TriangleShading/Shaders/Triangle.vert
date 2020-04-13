#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;

out vec4 vCol;
out vec2 TexCoord;

uniform mat4 mvp;

void main()
{
	gl_Position = mvp * vec4(pos, 1.0);
	vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);

	TexCoord = tex;
}