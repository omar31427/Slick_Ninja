#pragma once
#include "SmartMonster.h"
static auto registerIt = Factory<MovableObject>::instance().registerType(
    obj_types::n_smartMonster,
    [](sf::Vector2f position) -> std::shared_ptr<MovableObject>
    {
        return std::make_shared<SmartMonster>(position);
    }
);
SmartMonster::SmartMonster(sf::Vector2f position) : Monster(position)
{
    m_damageClock.restart();
    m_direction = sf::Vector2f(1, 0);
    const Resources& res = Resources::instance();
    sf::Texture* texture = res.instance().getTexture(n_smartMonster);
    sf::Vector2f scale;
    scale.x = (float)SMART_MONSTER_SIZE / 48;
    scale.y = (float)SMART_MONSTER_SIZE / 47;
    m_curAnimation = AnimationIndex::WalkingRight;

    m_sprite.setTexture(*texture);
    m_sprite.setTextureRect(sf::IntRect(0, 50, 48, 47));
    m_sprite.setPosition(position);
    m_sprite.setScale(scale);
    buildAnimations();
    animations[(int)m_curAnimation].applyToSprite(m_sprite, scale);

    m_curMaxHealthPair.first = SMART_MONSTER_MAX_HEALTH;
    m_curMaxHealthPair.second = SMART_MONSTER_MAX_HEALTH;

    float width = m_sprite.getGlobalBounds().width;
    m_hpBar = std::make_shared<HpBar>(m_sprite.getPosition(), width);
    m_scale = scale;

    setMonster();
}

void SmartMonster::buildAnimations()
{
    //===========================================================================================
    animations[int(AnimationIndex::WalkingLeft)] = Animation(0.08f);
    for (int i = 0; i < 6; i++)
        animations[int(AnimationIndex::WalkingLeft)].addFrame(sf::IntRect(i*48, 50, 45, 47));
   //===========================================================================================

    //===========================================================================================
    animations[int(AnimationIndex::WalkingRight)] = Animation(0.08f);
    for (int i = 0; i < 6; i++)
        animations[int(AnimationIndex::WalkingRight)].addFrame(sf::IntRect(i*48, 95, 45, 47));
    //===========================================================================================

    //===========================================================================================
    animations[int(AnimationIndex::GhostingLeft)] = Animation(0.08f);
    for (int i = 0; i < 6; i++)
        animations[int(AnimationIndex::GhostingLeft)].addFrame(sf::IntRect(285 +(i * 48), 50, 45, 47));
    //===========================================================================================

    //===========================================================================================
    animations[int(AnimationIndex::GhostingRight)] = Animation(0.08f);
    for (int i = 0; i < 6; i++)
        animations[int(AnimationIndex::GhostingRight)].addFrame(sf::IntRect(285 + (i * 48), 95, 45, 47));
    //===========================================================================================

    //===========================================================================================
    animations[(int)(AnimationIndex::Explosion)] = Animation(0.08f);
    animations[(int)(AnimationIndex::Explosion)].addFrame(sf::IntRect(55, 85, 115, 95));
    animations[(int)(AnimationIndex::Explosion)].addFrame(sf::IntRect(170, 85, 115, 95));
    animations[(int)(AnimationIndex::Explosion)].addFrame(sf::IntRect(300, 85, 115, 95));
    animations[(int)(AnimationIndex::Explosion)].addFrame(sf::IntRect(430, 85, 115, 95));
    animations[(int)(AnimationIndex::Explosion)].addFrame(sf::IntRect(55, 215, 115, 95));
    animations[(int)(AnimationIndex::Explosion)].addFrame(sf::IntRect(175, 225, 115, 95));
    animations[(int)(AnimationIndex::Explosion)].addFrame(sf::IntRect(310, 210, 115, 95));
}

std::shared_ptr<sf::Sprite> SmartMonster::getSprite()
{
    return std::make_shared<sf::Sprite>(m_sprite); 
}

void SmartMonster::setExplosionAnimation()
{
    m_curMaxHealthPair.first = 0;
    const Resources& res = Resources::instance();
    sf::Texture* explode = res.instance().getTexture(n_posionExplosion);
    m_sprite.setTexture(*explode);
    m_sprite.setTextureRect(sf::IntRect(55, 85, 115, 95));
    m_curAnimation = AnimationIndex::Explosion;
    animations[(int)m_curAnimation].applyToSprite(m_sprite, createNewScale());
}
void SmartMonster::changeAliveAnimation(sf::Vector2f direction)
{
    if (direction.x < 0 && !m_ghostMode)
        m_curAnimation = AnimationIndex::WalkingLeft;
    else if (direction.x >= 0 && !m_ghostMode)
        m_curAnimation = AnimationIndex::WalkingRight;
    else if (direction.x >= 0 && m_ghostMode)
        m_curAnimation = AnimationIndex::GhostingRight;
    else if (direction.x < 0 && m_ghostMode)
        m_curAnimation = AnimationIndex::GhostingLeft;
}
void SmartMonster::move(sf::Time deltaTime)
{
    // Calculate the direction vector towards the player
    sf::Vector2f direction = m_playerPosition - m_sprite.getPosition();

    // Normalize the direction vector
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0)
        direction /= length;
    //=================================================================================
    //setting the animation
    if(m_curMaxHealthPair.first != 0)
        changeAliveAnimation(direction);


    if (m_curMaxHealthPair.first != 0 && !m_timeToExplode)
        m_sprite.move(direction * SIMPLE_MONSTER_SPEED * deltaTime.asSeconds());
    else if (m_timeToExplode)
        setExplosionAnimation();

    if (m_curAnimation == AnimationIndex::Explosion && animations[(int)m_curAnimation].completedIteration())
    {
        m_needsToBeDeleted = true;
        return;
    }
    animations[(int)m_curAnimation].update(deltaTime.asSeconds());
    animations[(int)m_curAnimation].applyToSprite(m_sprite, createNewScale());
    //=================================================================================
    
  

    m_hpBar.get()->setPosition(m_sprite.getPosition());
}
void SmartMonster::startGhosting(bool ghost)
{
    if (m_timeToExplode)
        return;
    m_ghostMode = ghost;
    int curFrame = animations[(int)m_curAnimation].getCurFrame();
    
    if (m_ghostMode && m_curAnimation == AnimationIndex::WalkingRight)
        m_curAnimation = AnimationIndex::GhostingRight;
    else if (m_ghostMode && m_curAnimation == AnimationIndex::WalkingLeft)
        m_curAnimation = AnimationIndex::GhostingLeft;
    else if (!m_ghostMode && m_curAnimation == AnimationIndex::GhostingRight)
        m_curAnimation = AnimationIndex::WalkingRight;
    else if(!m_ghostMode && m_curAnimation == AnimationIndex::GhostingLeft)
        m_curAnimation = AnimationIndex::WalkingRight;

    animations[(int)m_curAnimation].setCurFrame(curFrame);
}
sf::Vector2f SmartMonster::createNewScale()
{
    sf::Vector2f newScale;
    newScale.x = (m_scale.x * 45) / std::abs(animations[(int)m_curAnimation].getFrameDimensions().x);
    newScale.y = (m_scale.y * 55) / std::abs(animations[(int)m_curAnimation].getFrameDimensions().y);
    return newScale;
}

void SmartMonster::setTimeToExplode()
{
    const Resources& res = Resources::instance();
    sf::Sound sound;
    sound.setBuffer(res.instance().get_sound(splat_sound));
    sound.play();
    sound.setVolume(15);
    m_timeToExplode = true;
}

std::shared_ptr<HpBar> SmartMonster::getHpBar()
{
	return m_hpBar;
}

void SmartMonster::setPlayerPosition(const sf::Vector2f& position)
{
    m_playerPosition = position;
}
