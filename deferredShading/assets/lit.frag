#version 450

in Surface
{
	vec3 pos;
	mat3 tbn;
	vec2 UV;
} fs_in;

in vec4 vs_lightSpacePos;

struct Material
{
	float ambientStrength;
	float diffuseStrength;
	float specularStrength;
	float shininess;
};

uniform sampler2D _shadowMap;

uniform vec3 _cameraPosition;
uniform vec3 _lightPosition;
uniform vec3 _ambientColor;
uniform vec3 _lightColor;
uniform float _shadowBrightness;
uniform float _shadowMinBias;
uniform float _shadowMaxBias;
uniform sampler2D _mainTex;
uniform sampler2D _normalTex;
uniform Material _material;

out vec4 FragColor;

float calcShadow(sampler2D shadowMap, vec3 normal, vec3 toLight, vec4 lightSpacePosition)
{
	vec3 samplerCoord = lightSpacePosition.xyz / lightSpacePosition.w;
	samplerCoord = samplerCoord * 0.5 + 0.5;

	float bias = max(_shadowMaxBias * (1.0 - dot(normal, toLight)), _shadowMinBias);
	float depth = samplerCoord.z - bias;
	float shadowMapDepth = texture(shadowMap, samplerCoord.xy).b;

	//PCF filtering
	float totalShadow = 0.0;
	vec2 texelOffset = 1.0 / textureSize(_shadowMap, 0);
	for (int y = -1; y <= 1; y++)
	{
		for (int x = -1; x <= 1; x++)
		{
			vec2 uv = samplerCoord.xy + vec2(x * texelOffset.x, y * texelOffset.y);
			totalShadow += step(texture(_shadowMap, uv).r, depth);
		}
	}

	return totalShadow / 9.0;
}

void main()
{
	//Light calculations
	vec3 normal = texture(_normalTex, fs_in.UV).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	vec3 _lightDirection = normalize(fs_in.pos - _lightPosition);
	vec3 toLight =  fs_in.tbn * -_lightDirection;
	vec3 toCamera = fs_in.tbn * normalize(_cameraPosition - fs_in.pos);
	float diffuseFactor = max(dot(normal, toLight), 0.0);
	
	//Specular reflection
	vec3 h = normalize(toLight + toCamera);
	float specularFactor = pow(max(dot(normal, h), 0.0), _material.shininess);

	float shadow = min(calcShadow(_shadowMap, normal, toLight, vs_lightSpacePos), 1.0 - _shadowBrightness);

	vec3 light = _ambientColor * _material.ambientStrength;
	light += _lightColor * diffuseFactor * _material.diffuseStrength;
	light += _lightColor * specularFactor * _material.specularStrength;
	light *= 1.0 - shadow;
	vec3 object = texture(_mainTex, fs_in.UV).rgb;

	FragColor = vec4(object * light, 1.0);
}