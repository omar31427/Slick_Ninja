#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Resources.h"
#include <memory>
class GameObject {
public:
	GameObject(sf::Vector2f position) :m_position(position) {};
	virtual std::shared_ptr<sf::Sprite> getSprite() { return nullptr; };
	void startTextureClock() { m_textureClock.restart(); };
	sf::Int32 getTextureClock() { return m_textureClock.getElapsedTime().asMilliseconds(); };
	void setType(obj_types type);
	obj_types getType();
	bool m_needsToBeDeleted = false;
	
	bool m_createPresent = false;
private:
	obj_types m_myType;
	sf::Vector2f m_position;
	sf::Clock m_textureClock;
};