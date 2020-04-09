#version 330

layout (location = 0) in vec3 pos;

uniform mat4 model;

out vec4 vCol;

void main()
{
	gl_Position = model * vec4(pos, 1.0);
	vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);
}