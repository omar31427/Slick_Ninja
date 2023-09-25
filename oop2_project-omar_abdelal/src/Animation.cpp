#pragma once
#include "Animation.h"



Animation::Animation(const float frameDuration)  : m_timePerFrame(frameDuration), m_curFrame(0), m_elapsedTime(0)
{

}
bool Animation::completedIteration()
{
    if(m_curFrame == m_frames.size()-1)
        return true;
    return false;
}
void Animation::addFrame(const sf::IntRect& frame)
{
    m_frameDimensions.x = frame.width;
    m_frameDimensions.y = frame.height;
	m_frames.push_back(frame);
}
const sf::Vector2f Animation::getFrameDimensions() 
{
    return m_frameDimensions;
}
int Animation::getCurFrame()
{
    return m_curFrame;
}
void Animation::setCurFrame(int frame)
{
    m_curFrame = frame;
}
void Animation::update(float dt)
{
    if (m_timePerFrame == 0)
        return;
    m_elapsedTime += dt;
    if (m_elapsedTime >= m_timePerFrame)
    {
        m_curFrame++;
        if (m_curFrame >= m_frames.size())
        {
            m_completedIteration = true;
            m_curFrame = 0;
        }
        else
            m_completedIteration = false;
        m_elapsedTime = 0.f;
    }
}

void Animation::applyToSprite(sf::Sprite& sprite,sf::Vector2f scale)
{
	sprite.setTextureRect(m_frames[m_curFrame]);
    sprite.setScale(scale);
}



