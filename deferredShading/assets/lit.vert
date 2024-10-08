//#version specified in C++

#if __VERSION__ < 400
precision highp float;
#endif

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vUV;
layout(location = 3) in vec3 vTangent;
layout(location = 4) in vec3 vBitangent;

uniform mat4 _model;
uniform mat4 _view;
uniform mat4 _lightViewProjection;

out vec3 atr_pos;
out mat3 atr_tbn;
out vec2 atr_UV;

void main()
{
	vec3 t = normalize(vec3(_model * vec4(vTangent, 0.0)));
	vec3 b = normalize(vec3(_model * vec4(vBitangent, 0.0)));
	vec3 n = normalize(vec3(_model * vec4(vNormal, 0.0)));
	mat3 tbn = mat3(t, b, n);

	atr_pos = vec3(_model * vec4(vPos, 1.0));
	atr_tbn = tbn;
	atr_UV = vUV;
	gl_Position = _view * _model * vec4(vPos, 1.0);
}