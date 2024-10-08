//#version specified in C++

#if __VERSION__ < 400
precision highp float;
#endif

layout(location = 0) in vec3 vPos;

uniform mat4 _view;
uniform mat4 _model;

void main()
{
	gl_Position = _view * _model * vec4(vPos, 1.0);
}