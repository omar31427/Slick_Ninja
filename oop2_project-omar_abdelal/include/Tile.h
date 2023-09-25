#pragma once
#include "StableObject.h"
class Tile : public StableObject {
public:
	Tile(sf::Vector2f position);
	std::shared_ptr<sf::Sprite> getSprite() override;

private:
	sf::Sprite m_sprite;
};