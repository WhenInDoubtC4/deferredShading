//#version specified in C++

#if __VERSION__ < 400
precision highp float;
#endif

layout(location = 0) out vec4 gPosition;
layout(location = 1) out vec4 gNormal;
layout(location = 2) out vec4 gAlbedo;
layout(location = 3) out vec4 gShadingModel;
	
in vec3 atr_pos;
in mat3 atr_tbn;
in vec2 atr_UV;

uniform vec4 _solidColor;
uniform sampler2D _mainTex;
uniform sampler2D _normalTex;
uniform vec3 _shadingModelColor;

#if __VERSION__ > 400
subroutine vec4 AlbedoFunction();
subroutine vec4 NormalFunction();
#endif

#if __VERSION__ > 400
subroutine (AlbedoFunction) 
#endif
vec4 albedoFromTexture()
{
	return vec4(texture(_mainTex, atr_UV).rgb, 1.0);
}

#if __VERSION__ > 400
subroutine (AlbedoFunction)
#endif
vec4 albedoFromColor()
{
	return vec4(_solidColor.rgb, 1.0);
}

#if __VERSION__ > 400
subroutine (NormalFunction) 
#endif
vec4 normalFromTexture()
{
	return vec4(atr_tbn * normalize(texture(_normalTex, atr_UV).rgb * 2.0 - 1.0), 1.0);
}

#if __VERSION__ > 400
subroutine (NormalFunction) 
#endif
vec4 normalFromMesh()
{
	return vec4(atr_tbn * vec3(0.5, 0.5, 0.0), 1.0);
}

#if __VERSION__ > 400
subroutine uniform AlbedoFunction _albedoFunction;
subroutine uniform NormalFunction _normalFunction;
#else
uniform int _albedoFunction;
uniform int _normalFunction;
#endif

void main()
{
	gPosition = vec4(atr_pos, 1.0);

#if __VERSION__ > 400
	gAlbedo = _albedoFunction();
#else
	if (_albedoFunction == 0) gAlbedo = albedoFromTexture();
	else if (_albedoFunction == 1) gAlbedo = albedoFromColor();
#endif

	//gNormal = transpose(atr_tbn)[2]; //WS normal form TBN

#if __VERSION__ > 400
	gNormal = _normalFunction(); //WS normal
#else
	if (_normalFunction == 0) gNormal = normalFromTexture();
	else if (_normalFunction == 1) gNormal = normalFromMesh();
#endif

	gShadingModel = vec4(_shadingModelColor, 1.0);
}