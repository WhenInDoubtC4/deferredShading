#version 450

layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec3 gAlbedo;
layout(location = 3) out vec3 gShadingModel;

in Surface
{
	vec3 pos;
	mat3 tbn;
	vec2 UV;
} fs_in;

uniform vec4 _solidColor;
uniform sampler2D _mainTex;
uniform sampler2D _normalTex;
uniform vec3 _shadingModelColor;

subroutine vec3 AlbedoFunction();
subroutine vec3 NormalFunction();

subroutine (AlbedoFunction) vec3 albedoFromTexture()
{
	return texture(_mainTex, fs_in.UV).rgb;
}

subroutine (AlbedoFunction) vec3 albedoFromColor()
{
	return _solidColor.rgb;
}

subroutine (NormalFunction) vec3 normalFromTexture()
{
	return fs_in.tbn * normalize(texture(_normalTex, fs_in.UV).rgb * 2.0 - 1.0);
}

subroutine (NormalFunction) vec3 normalFromMesh()
{
	return fs_in.tbn * vec3(0.5, 0.5, 0.0);
}

subroutine uniform AlbedoFunction _albedoFunction;
subroutine uniform NormalFunction _normalFunction;

void main()
{
	gPosition = fs_in.pos;
	gAlbedo = _albedoFunction();
	//gNormal = transpose(fs_in.tbn)[2]; //WS normal form TBN
	gNormal = _normalFunction(); //WS normal
	gShadingModel = _shadingModelColor;
}