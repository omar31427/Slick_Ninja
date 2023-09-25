
#pragma once
#include "Monster.h"
class SmartMonster : public Monster
{
public:
	SmartMonster(sf::Vector2f position);

	void buildAnimations();

	std::shared_ptr<sf::Sprite> getSprite();

	void setExplosionAnimation();

	void changeAliveAnimation(sf::Vector2f direction);

	void changeAliveAnimation();


	void move(sf::Time deltaTime) override;

	void startGhosting(bool ghost) override;

	sf::Vector2f createNewScale();

	void setTimeToExplode();

	std::shared_ptr<HpBar> getHpBar()override;

	void setPlayerPosition(const sf::Vector2f& position) override ;

private:
	sf::Vector2f m_playerPosition;
	sf::Sprite m_sprite;
	enum class AnimationIndex
	{
		WalkingRight,
		WalkingLeft,
		GhostingRight,
		GhostingLeft,
		Explosion,
		Count,
	};
	
	sf::Vector2f m_scale;
	bool m_facingRight;
	sf::Vector2f m_direction;
	float m_curHealth = SIMPLE_MONSTER_MAX_HEALTH;
	float m_maxHealth = SIMPLE_MONSTER_MAX_HEALTH;
	bool m_ghostMode = false;
	AnimationIndex m_curAnimation;
	Animation animations[(int)AnimationIndex::Count];
};