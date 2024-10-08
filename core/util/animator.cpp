#include "animator.h"

Animator::Animator(std::initializer_list<Animation*>& initAnimations)
{
	for (Animation* anim : initAnimations)
	{
		_animations.push_back(anim);
	}
}

void Animator::addAnimation(Animation* anim)
{
	_animations.push_back(anim);
}

void Animator::updateAll(float deltaTime)
{
	for (Animation* anim : _animations)
	{
		float animLength = anim->getLength();
		float normalizedTime = _time - animLength * floor(_time / animLength);
		ew::Transform currentTransform = anim->getTransformAtNormalizedTime(normalizedTime);

		anim->_node->setLocalTransform(currentTransform);
	}

	_time += deltaTime;
}
