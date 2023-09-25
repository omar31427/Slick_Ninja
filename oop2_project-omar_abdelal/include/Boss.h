#pragma once
#include "Monster.h"

class Boss : public Monster
{
public:
	Boss(sf::Vector2f position);

	void buildAnimations();

	std::shared_ptr<sf::Sprite> getSprite();

	obj_types getAttType()override;

	void activateBoss()override;

	void move(sf::Time deltaTime) override;

	sf::Vector2f createNewScale();

	void setTimeToExplode();

	std::shared_ptr<HpBar> getHpBar()override;

	bool canICreateProjectile()override;


	void setPlayerPosition(const sf::Vector2f& position) override;

private:
	sf::Sprite m_sprite;
	sf::Vector2f m_scale;
	bool m_facingRight;
	sf::Vector2f m_direction;
	float m_curHealth = SIMPLE_MONSTER_MAX_HEALTH;
	float m_maxHealth = SIMPLE_MONSTER_MAX_HEALTH;
	sf::Clock m_normalAttClock;
	sf::Clock m_specAttClock;
	enum AnimationIndex {
		active,
		wall,
		Count,
	};
	bool m_isActivated = false;
	Animation m_animation[(int)AnimationIndex::Count];
	AnimationIndex m_curAnimation;
	obj_types m_attType;
	bool m_createProjectile = false;
};