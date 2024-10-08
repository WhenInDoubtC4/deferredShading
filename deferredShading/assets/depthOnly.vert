#version 450

layout(location = 0) in vec3 vPos;

uniform mat4 _view;
uniform mat4 _model;

void main()
{
	gl_Position = _view * _model * vec4(vPos, 1.0);
}