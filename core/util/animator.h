#pragma once

#include "animation.h"

class Animator
{
public:
	Animator() {};
	Animator(std::initializer_list<Animation*>& initAnimations);

	void addAnimation(Animation* anim);
	void updateAll(float deltaTime);

private:
	float _time = 0.f;
	std::vector<Animation*> _animations;
};