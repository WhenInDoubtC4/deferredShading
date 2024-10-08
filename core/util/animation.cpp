#include "animation.h"

Animation::Animation(KNode* node)
	: _node(node)
{

}

Animation* Animation::addKeyframe(float time, ew::Transform transform)
{
	_keyframes.push_back(std::make_pair(time, transform));
	_areKeyframesSorted = false;

	return this;
}

Animation* Animation::addKeyframe(float time, glm::vec3 position, glm::quat rotation, glm::vec3 scale)
{
	ew::Transform transform;
	transform.position = position;
	transform.rotation = rotation;
	transform.scale = scale;

	return addKeyframe(time, transform);
}


void Animation::sortKeyframes()
{
	std::sort(_keyframes.begin(), _keyframes.end(), [](const Keyframe& a, const Keyframe& b)
		{
			return a.first < b.first;
		});

	_areKeyframesSorted = true;
}

float Animation::getLength()
{
	if (!_areKeyframesSorted) sortKeyframes();

	return _keyframes.back().first;
}

ew::Transform Animation::getTransformAtNormalizedTime(float time)
{
	if (!_areKeyframesSorted) sortKeyframes();

	Keyframe* keyframe0 = nullptr;
	Keyframe* keyframe1 = nullptr;

	//Find keyframes between specified interval
	int nextKeyframeIndex = _currentKeyframeIndex + 1 % _keyframes.size();
	int i = 0;
	while (_keyframes[_currentKeyframeIndex].first > time || _keyframes[nextKeyframeIndex].first <= time)
	{
		_currentKeyframeIndex = nextKeyframeIndex++;
		nextKeyframeIndex %= _keyframes.size();

		//Prevent infinite loops
		i++;
		assert(i < _keyframes.size());
	}

	keyframe0 = &_keyframes[_currentKeyframeIndex];
	keyframe1 = &_keyframes[nextKeyframeIndex];

	assert(keyframe0 && keyframe1);

	float alpha = (time - keyframe0->first) / (keyframe1->first - keyframe0->first);

	ew::Transform result;
	result.position = glm::mix(keyframe0->second.position, keyframe1->second.position, alpha);
	result.rotation = glm::slerp(keyframe0->second.rotation, keyframe1->second.rotation, alpha);
	result.scale = glm::mix(keyframe0->second.scale, keyframe1->second.scale, alpha);

	return result;
}
