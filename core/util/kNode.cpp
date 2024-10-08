#include "kNode.h"

KNode::KNode(std::string name, glm::vec3 localPosition, glm::quat localRotation, glm::vec3 localScale)
	: _name(name)
	, _localPosition(localPosition)
	, _localRotation(localRotation)
	, _localScale(localScale)
{

}

KNode::~KNode()
{
	//Move children to parent
	if (_parent)
	{
		for (KNode* childNode : _children)
		{
			childNode->attach(_parent);
		}
	}

	detach();
}

glm::mat4 KNode::getLocalTransformMatrix() const
{
	ew::Transform transform;
	transform.position = _localPosition;
	transform.rotation = _localRotation;
	transform.scale = _localScale;

	return transform.modelMatrix();
}

glm::mat4 KNode::getGlobalTransformMatrix() const
{
	return _globalTransform;
}

void KNode::attach(KNode* target)
{
	//Detach from hierarchy first if already attached
	if (_parent) detach();

	_parent = target;

	target->_children.push_back(this);
}

void KNode::detach()
{
	if (!_parent) return;

	std::vector<KNode*>& childrenVec = _parent->_children;
	childrenVec.erase(std::find(childrenVec.begin(), childrenVec.end(), this));

	_parent = nullptr;
}

void KNode::outputHierarchy() const
{
	printf(_name.c_str());

	for (KNode* childNode : _children)
	{
		printf("\t");
		childNode->outputHierarchy();
	}

	printf("\n");
}

void KNode::iterateHierarchy(std::function<void(KNode*)> callback)
{
	callback(this);

	for (KNode* childNode : _children)
	{
		childNode->iterateHierarchy(callback);
	}
}

void KNode::solveFKRecursive(KNode* start)
{
	if (!start->_parent)
	{
		start->_globalTransform = start->getLocalTransformMatrix();
	}
	else
	{
		start->_globalTransform = start->_parent->_globalTransform * start->getLocalTransformMatrix();
	}

	for (KNode* childNode : start->_children)
	{
		solveFKRecursive(childNode);
	}
}

void KNode::setLocalTransform(ew::Transform localTransform)
{
	_localPosition = localTransform.position;
	_localRotation = localTransform.rotation;
	_localScale = localTransform.scale;
}

void KNode::setLocalPosition(glm::vec3 localPosition)
{
	_localPosition = localPosition;
}

void KNode::setLocalRotation(glm::quat localRotation)
{
	_localRotation = localRotation;
}

void KNode::setLocalRotation(glm::vec3 localRotation)
{
	glm::vec3 localRotationRad = glm::vec3(
		glm::radians(localRotation.x),
		glm::radians(localRotation.y),
		glm::radians(localRotation.z));

	_localRotation = glm::quat(localRotationRad);
}

void KNode::setLocalScale(glm::vec3 localScale)
{
	_localScale = localScale;
}
