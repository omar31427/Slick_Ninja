#pragma once
#include "GameObject.h"
#include "Factory.h"
#include "HpBar.h"
#include "Animation.h"

const float GRAVITY = 98;

class MovableObject: public GameObject {
public:
	MovableObject(sf::Vector2f position) :GameObject(position), m_monster(false) {
	};
	virtual void setAsFireball() { ; };
	virtual std::shared_ptr<HpBar> getHpBar() { return nullptr; };
	virtual void shoot() { ; };
	virtual void move(sf::Time deltaTime) = 0;
	virtual void addForbiddenMove( [[maybe_unused]] const sf::Vector2f& move, [[maybe_unused]] const float& margin, [[maybe_unused]] const float& horizontalDifference, [[maybe_unused]] const float& verticalDifference) { ; };
	virtual void detattachTiles(  bool deAttach) { ; };
	virtual sf::Vector2f releaseShot() { return sf::Vector2f(0,0); };
	virtual void setDirection([[maybe_unused]] sf::Vector2f direction) { ; };
	virtual bool canICreateProjectile() { return false; };
	void setHealth(float newHealth);
	virtual void changeDirection([[maybe_unused]] bool value) { ; };
	std::vector<sf::Vector2f> m_forbiddenMoves;
	bool isGrounded = true;
	bool m_timeToExplode = false;
	virtual void activateBoss() { ; };
	virtual void setShooter([[maybe_unused]] const obj_types& shooterIdentity) { ; };
	virtual obj_types getAttType() { return n_projectile; };
	virtual obj_types getShooter() { return n_player; };
	std::shared_ptr<HpBar> m_hpBar ;
	std::pair<float, float> m_curMaxHealthPair;
	bool goingRight = true;
	virtual void startGhosting([[maybe_unused]] bool ghost) { ; };
	void setMonster();
	bool m_isAlive = false;
	bool getMonster() const { return m_monster; }
	virtual void setPlayerPosition([[maybe_unused]] const sf::Vector2f& position) { ; };
	sf::Clock m_damageClock;
	virtual void addScore(int score) { ; };
	virtual int getScore()const { return 0; };
private:
	bool m_monster;
};