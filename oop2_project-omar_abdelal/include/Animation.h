#pragma once
#include "Resources.h"

class Animation
{
public:
	Animation() = default;
	Animation(const float frameDuration);
	void update(float dt);
	void applyToSprite(sf::Sprite& spritush,sf::Vector2f scale);
	void change_anim_frame_Rate(float rate)
	{
		m_timePerFrame = rate;
	}
	bool completedIteration();
	void addFrame(const sf::IntRect& frame);
	const sf::Vector2f getFrameDimensions();
	int getCurFrame();
	void setCurFrame(int frame);
private:
	
	bool m_completedIteration = false;
	std::vector<sf::IntRect> m_frames;
	sf::Vector2f m_frameDimensions;
	float m_elapsedTime ;
	float m_timePerFrame;
	int m_curFrame;

};