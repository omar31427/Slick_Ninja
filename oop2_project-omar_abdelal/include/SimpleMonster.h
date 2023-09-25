#pragma once
#include "Monster.h"
class SimpleMonster : public Monster
{
public:
	SimpleMonster(sf::Vector2f position) ;

	void buildAnimations();

	std::shared_ptr<sf::Sprite> getSprite();

	void changeDirection(bool value)override;

	void move(sf::Time deltaTime) override;

	sf::Vector2f createNewScale();

	void setTimeToExplode();

	std::shared_ptr<HpBar> getHpBar()override;
	
private:
	sf::Sprite m_sprite;
	enum class AnimationIndex
	{
		WalkingRight,
		WalkingLeft,
		Exploding,
		Count,
	};
	
	sf::Vector2f m_scale;
	bool m_facingRight;
	sf::Vector2f m_direction;
	float m_curHealth = SIMPLE_MONSTER_MAX_HEALTH;
	float m_maxHealth = SIMPLE_MONSTER_MAX_HEALTH;

	AnimationIndex m_curAnimation;
	Animation animations[(int)AnimationIndex::Count];
};