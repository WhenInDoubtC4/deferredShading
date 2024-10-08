#pragma once

#include <vector>
#include <string>
#include <functional>
#include <glm/glm.hpp>

#include "../ew/transform.h"

class KNode
{
public:
	KNode(std::string name, glm::vec3 localPosition = glm::vec3(0.f), glm::quat localRotation = glm::quat(1.f, 0.f, 0.f, 0.f), glm::vec3 localScale = glm::vec3(1.f));
	~KNode();

	std::string getName() const { return _name; };

	glm::mat4 getLocalTransformMatrix() const;
	glm::mat4 getGlobalTransformMatrix() const;

	void attach(KNode* target);
	void detach();

	void outputHierarchy() const;
	void iterateHierarchy(std::function<void(KNode*)> callback);

	static void solveFKRecursive(KNode* start);

	glm::vec3 getLocalPosition() const { return _localPosition; };
	glm::quat getLocalRotation() const { return _localRotation; };
	glm::vec3 getLocalScale() const { return _localScale; };

	void setLocalTransform(ew::Transform localTransform);
	void setLocalPosition(glm::vec3 localPosition);
	void setLocalRotation(glm::quat localRotation);
	void setLocalRotation(glm::vec3 localRotation);
	void setLocalScale(glm::vec3 localScale);

private:
	std::string _name;

	glm::vec3 _localPosition;
	glm::quat _localRotation;
	glm::vec3 _localScale;

	glm::mat4 _globalTransform = glm::mat4(1.f);

	KNode* _parent = nullptr;
	std::vector<KNode*> _children;
};