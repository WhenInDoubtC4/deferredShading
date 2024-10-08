#pragma once

#include <utility>
#include <algorithm>

#include "../ew/transform.h"

#include "kNode.h"

typedef std::pair<float, ew::Transform> Keyframe;

class Animation
{
public:
	Animation(KNode* node);

	Animation* addKeyframe(float time, ew::Transform transform);
	Animation* addKeyframe(float time, glm::vec3 position, glm::quat rotation, glm::vec3 scale);

private:
	friend class Animator;

	void sortKeyframes();
	float getLength();
	ew::Transform getTransformAtNormalizedTime(float time);

	KNode* _node;
	bool _areKeyframesSorted = false;
	std::vector<Keyframe> _keyframes;

	int _currentKeyframeIndex = 0;
	float _currentKeyframeEndTime = 0.f;
};