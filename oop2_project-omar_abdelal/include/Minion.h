
#pragma once
#include "Monster.h"
class Minion : public Monster
{
public:
	Minion(sf::Vector2f position);

	void buildAnimations();

	std::shared_ptr<sf::Sprite> getSprite();

	void move(sf::Time deltaTime) override;

	sf::Vector2f createNewScale();

	void setTimeToExplode();

	void setPlayerPosition(const sf::Vector2f& position) override;

private:
	sf::Clock m_timeAlive;
	sf::Vector2f m_playerPosition;
	sf::Sprite m_sprite;
	sf::Vector2f m_scale;
	sf::Vector2f m_prevDimensions;
	sf::Vector2f m_direction;
	Animation animation;
};