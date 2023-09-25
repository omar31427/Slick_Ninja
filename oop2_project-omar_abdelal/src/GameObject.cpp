#pragma once
#include "GameObject.h"

void GameObject::setType(obj_types type)
{
	m_myType = type;
}

obj_types GameObject::getType()
{
	return m_myType;
}
