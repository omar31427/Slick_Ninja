#pragma once
#include "GameObject.h"
#include "Factory.h"
class StableObject : public GameObject {
public:
	StableObject(sf::Vector2f position) :GameObject(position) {

	};
	virtual void Update([[maybe_unused]] float deltaTime) { ; };
	
};