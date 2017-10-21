#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 view;
uniform mat4 projection;

void main()
{
	TexCoords = aPos;

	vec4 pos = projection * view * vec4(aPos, 1.0);
	//the depth of the skybox is 1.0, which means that we're going to render the skybox last
	gl_Position = pos.xyww;
}