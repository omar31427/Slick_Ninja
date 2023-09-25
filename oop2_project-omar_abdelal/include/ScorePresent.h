#pragma once
#include "Presents.h"
#include "Animation.h"
class ScorePresent : public Presents
{
public:
	ScorePresent(sf::Vector2f position);
	void buildAnimations();
	std::shared_ptr<sf::Sprite> getSprite() override;

	void Update(float deltaTime) override;
	~ScorePresent() {};
private:

	sf::Sprite m_sprite;
	Animation animation;
};