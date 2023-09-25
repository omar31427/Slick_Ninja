#pragma once
#include "MovableObject.h"

const  float JUMP_VELOCITY = -98; 
const float JUMP_HEIGHT = 120;

class Player : public MovableObject {
public:
	void move(sf::Time deltaTime) override;
	sf::Vector2f createNewPlayerScale();
	void buildAnimations();
	Player(sf::Vector2f position);
	std::shared_ptr<sf::Sprite> getSprite() override;
	bool isConnected();
	void addForbiddenMove(const sf::Vector2f& move, const float& margin, const float& horizontalDifference, const float& verticalDifference)override;
	void jump(sf::Time deltaTime);
	std::shared_ptr<HpBar> getHpBar() override;
	//void setHealth(float newHealth);
	void detattachTiles(bool deAttach) override;
	void shoot()override;
	sf::Vector2f releaseShot() override;
	bool canICreateProjectile()override;
	void addScore(int score) override;
	int getScore()const override;
	bool isColliding;
private:
	int m_score;
	sf::Sprite m_sprite;
	bool m_isJumping = false;
	bool m_isConnected = true;
	bool m_collisionDetected = false;
	float m_previousConnectedTile = 0;
	float m_jumpStartY;
	sf::Vector2f m_playerScale;
	enum class AnimationIndex
	{
		WalkingRight,
		WalkingLeft,
		JumpingRight,
		JumpingLeft,
		FallingRight,
		FallingLeft,
		SlidingRight,
		SlidingLeft,
		ShootingRight,
		ShootingLeft,
		Count,
	};
	int m_ammo = 100;
	bool m_createProjectile;
	bool m_facingRight = true;
	AnimationIndex m_curAnimation;
	Animation animations[(int)AnimationIndex::Count];
	sf::Clock m_fallingClock;
	sf::Clock m_shootingClock;
};