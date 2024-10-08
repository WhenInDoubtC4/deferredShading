#version 450

out vec4 FragColor;

in vec2 UV;

uniform layout(binding = 0) sampler2D _colorBuffer;
uniform layout(binding = 1) sampler2D _depthBuffer;

uniform vec2 _focusPoint;

uniform int _boxBlurSize;

uniform float _chromaticAberrationSize;

uniform float _dofMinDistance;
uniform float _dofMaxDistance;
uniform int _dofBlurSize;

//Subroutines
subroutine vec3 BlurFunction(int size, sampler2D colorTexture, vec2 texelSize, vec2 UVs);
subroutine vec3 DepthOfFieldFunction(sampler2D colorTexture, sampler2D depthTexture, vec2 texelSize, in vec2 UVs);
subroutine vec3 ChromaticAberrationFunction(sampler2D colorTexture, sampler2D depthTexture, vec2 texelSize, vec2 UVs);

//Box blur
//From lecture slide and https://lettier.github.io/3d-game-shaders-for-beginners/blur.html
subroutine (BlurFunction) vec3 blurDisabled(int size, sampler2D colorTexture, vec2 texelSize, vec2 UVs)
{
	return texture(colorTexture, UVs).rgb;
}

vec3 boxBlur(int size, sampler2D colorTexture, vec2 texelSize, vec2 UVs)
{
	vec3 color = vec3(0.0);

	for (int x = -size; x <= size; x++)
	{
		for (int y = -size; y <= size; y++)
		{
			vec2 offset = vec2(x, y) * texelSize;
			color += texture(colorTexture, UVs + offset).rgb;
		}
	}

	color /= pow(size * 2 + 1, 2);

	return color;
}

subroutine (BlurFunction) vec3 blurEnabled(int size, sampler2D colorTexture, vec2 texelSize, vec2 UVs)
{
	return boxBlur(size, colorTexture, texelSize, UVs);
}

subroutine uniform BlurFunction _blurFunction;

//Depth of field
subroutine (DepthOfFieldFunction) vec3 dofDisabled(sampler2D colorTexture, sampler2D depthTexture, vec2 texelSize, vec2 UVs)
{
	return _blurFunction(_boxBlurSize, colorTexture, texelSize, UVs);
}

subroutine (DepthOfFieldFunction) vec3 dofEnabled(sampler2D colorTexture, sampler2D depthTexture, vec2 texelSize, vec2 UVs)
{
	vec3 inFocus = _blurFunction(_boxBlurSize, colorTexture, texelSize, UVs);
	vec3 outOfFocus = boxBlur(_dofBlurSize, colorTexture, texelSize, UVs);

	float depth = texture(depthTexture, UVs).b;
	vec3 position = vec3(UVs, depth);

	vec4 dofFocusPoint = vec4(_focusPoint, texture(depthTexture, _focusPoint).ba);
	float blur = smoothstep(_dofMinDistance / 10000.0, _dofMaxDistance / 10000.0, abs(position.z - dofFocusPoint.z));
	vec3 dofColor = mix(inFocus, outOfFocus, blur);

	return dofColor;
}

subroutine uniform DepthOfFieldFunction _dofFunction;

//Chromatic aberration
subroutine (ChromaticAberrationFunction) vec3 chromaticAberrationDisabled(sampler2D colorTexture, sampler2D depthTexture, vec2 texelSize, vec2 UVs)
{
	return _dofFunction(colorTexture, depthTexture, texelSize, UVs);
}

subroutine (ChromaticAberrationFunction) vec3 chromaticAberrationEnabled(sampler2D colorTexture, sampler2D depthTexture, vec2 texelSize, vec2 UVs)
{
	vec3 color;

	vec2 direction = UVs - _focusPoint;

	vec2 rUV = UVs + (direction * vec2(_chromaticAberrationSize));
	vec2 gUV = UVs + (direction * vec2(0.5 * _chromaticAberrationSize));
	vec2 bUV = UVs + (direction * vec2(0.25 * _chromaticAberrationSize));

	color.r = _dofFunction(colorTexture, depthTexture, texelSize, rUV).r;
	color.g = _dofFunction(colorTexture, depthTexture, texelSize, gUV).g;
	color.b = _dofFunction(colorTexture, depthTexture, texelSize, bUV).b;

	return color;
}

subroutine uniform ChromaticAberrationFunction _chromaticAberrationFunction;

void main()
{
	vec2 texelSize = 1.0 / textureSize(_colorBuffer, 0).xy;

	//For some reason not calling all of these will cause chaos
	vec3 step1 = _blurFunction(_boxBlurSize, _colorBuffer, texelSize, UV);
	vec3 step2 = _dofFunction(_colorBuffer, _depthBuffer, texelSize, UV);
	vec3 step3 = _chromaticAberrationFunction(_colorBuffer, _depthBuffer, texelSize, UV);

	FragColor = vec4(step3, 1.0);
}