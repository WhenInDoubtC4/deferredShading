#version 450

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vUV;
layout(location = 3) in vec3 vTangent;
layout(location = 4) in vec3 vBitangent;

uniform mat4 _model;
uniform mat4 _view;
uniform mat4 _lightViewProjection;

out Surface
{
	vec3 pos;
	mat3 tbn;
	vec2 UV;
} vs_out;

void main()
{
	vec3 t = normalize(vec3(_model * vec4(vTangent, 0.0)));
	vec3 b = normalize(vec3(_model * vec4(vBitangent, 0.0)));
	vec3 n = normalize(vec3(_model * vec4(vNormal, 0.0)));
	mat3 tbn = mat3(t, b, n);

	vs_out.pos = vec3(_model * vec4(vPos, 1.0));
	vs_out.tbn = tbn;
	vs_out.UV = vUV;
	gl_Position = _view * _model * vec4(vPos, 1.0);
}