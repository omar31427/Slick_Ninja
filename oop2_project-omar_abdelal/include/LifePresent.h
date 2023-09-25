#pragma once
#include "Presents.h"
#include "Animation.h"
class LifePresent : public Presents {
public:
	LifePresent(sf::Vector2f position);
	void buildAnimations();
	std::shared_ptr<sf::Sprite> getSprite() override;

	void Update(float deltaTime) override;

private:

	sf::Sprite m_sprite;
	Animation animation;
};