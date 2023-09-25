#pragma once
#include "MovableObject.h"

class BaseMonster : public MovableObject
{
public:
	BaseMonster(sf::Vector2f position) :MovableObject(position) {};

};