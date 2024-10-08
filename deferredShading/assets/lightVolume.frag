//#version specified in C++

#if __VERSION__ < 400
precision highp float;
#endif

//MUST match values in mainGlobals.h
#define MAX_GRID_SIZE 8
#define MAX_LIGHTS_PER_MONKEY 16
#define MAX_POINT_LIGHTS MAX_GRID_SIZE * MAX_GRID_SIZE * MAX_LIGHTS_PER_MONKEY

struct PointLight
{
	vec3 position;
	float radius;
	vec4 color;
};

struct Material
{
	float ambientStrength;
	float diffuseStrength;
	float specularStrength;
	float shininess;
};

uniform sampler2D _gPosition;
uniform sampler2D _gNormal;
uniform sampler2D _gAlbedo;
uniform sampler2D _gShadingModel;
uniform sampler2D _gShadowMap;

uniform vec3 _cameraPosition;

uniform int _lightIndex;
layout(std140) uniform AdditionalLights
{
	PointLight _pointLights[MAX_POINT_LIGHTS];
};

uniform Material _material;

#if __VERSION__ > 400
subroutine float AttenuateFunction(float dist, float radius);
#endif

#if __VERSION__ > 400
subroutine (AttenuateFunction)
#endif
float attenuateLinear(float dist, float radius)
{
	return clamp((radius - dist) / radius, 0.0, 1.0);
}

#if __VERSION__ > 400
subroutine (AttenuateFunction)
#endif
float attenuateExponential(float dist, float radius)
{
	float i = clamp(1.0 - pow(dist / radius, 4.0), 0.0, 1.0);
	return i * i;
}

#if __VERSION__ > 400
subroutine uniform AttenuateFunction _attenuateFunction;
#else
uniform int _attenuateFunction;
#endif

vec3 calcPointLight(PointLight light, vec3 worldPosition, vec3 normal, vec3 toCamera)
{
	vec3 diff = light.position - worldPosition;
	vec3 toLight = normalize(diff);
	
	float diffuseFactor = max(dot(normal, toLight), 0.0);
	vec3 h = normalize(toLight + toCamera);
	float specularFactor = pow(max(dot(normal, h), 0.0), _material.shininess);

	vec3 lightColor = light.color.rgb * diffuseFactor * _material.diffuseStrength;
	lightColor += light.color.rgb * specularFactor * _material.specularStrength;

	//Attenuation
	float dist = length(diff);

#if __VERSION__ > 400
	lightColor *= _attenuateFunction(dist, light.radius);
#else
	if (_attenuateFunction == 0) lightColor *= attenuateLinear(dist, light.radius);
	else if (_attenuateFunction == 1) lightColor *= attenuateExponential(dist, light.radius);
#endif

	return lightColor;
}

out vec4 FragColor;

void main()
{
	vec2 UV = gl_FragCoord.xy / vec2(textureSize(_gNormal,0)); 
	vec3 normal = texture(_gNormal, UV).rgb;
	vec3 position = texture(_gPosition, UV).xyz;

	vec3 toCamera = normalize(_cameraPosition - position);

	PointLight light = _pointLights[_lightIndex];
	vec3 lightColor = calcPointLight(light, position, normal, toCamera);

	FragColor = vec4(lightColor * texture(_gAlbedo, UV).rgb, 1.0);
}