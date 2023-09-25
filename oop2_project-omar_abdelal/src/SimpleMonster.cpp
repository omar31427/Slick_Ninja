#pragma once
#include "SimpleMonster.h"

static auto registerIt = Factory<MovableObject>::instance().registerType(
    obj_types::n_simpleMonster,
    [](sf::Vector2f position) -> std::shared_ptr<MovableObject>
    {
        return std::make_shared<SimpleMonster>(position);
    }
);

SimpleMonster::SimpleMonster(sf::Vector2f position) :Monster(position)
{
    m_damageClock.restart();
    m_direction = sf::Vector2f(1, 0);
    const Resources& res = Resources::instance();
    sf::Texture* texture = res.instance().getTexture(n_simpleMonster);
    sf::Vector2f scale;
    scale.x = (float)SIMPLE_MONSTER_SIZE / 26;
    scale.y = (float)SIMPLE_MONSTER_SIZE / 30;
    m_curAnimation = AnimationIndex::WalkingRight;

    m_sprite.setTexture(*texture);
    m_sprite.setTextureRect(sf::IntRect(160, 0, -26, 30));
    m_sprite.setPosition(position);
    m_sprite.setScale(scale);
    buildAnimations();
    animations[(int)m_curAnimation].applyToSprite(m_sprite, scale);

    m_curMaxHealthPair.first = SIMPLE_MONSTER_MAX_HEALTH;
    m_curMaxHealthPair.second = SIMPLE_MONSTER_MAX_HEALTH;

    float width = m_sprite.getGlobalBounds().width;
    m_hpBar = std::make_shared<HpBar>(m_sprite.getPosition(), width);
    m_scale = scale;

    setMonster();
}

void SimpleMonster::buildAnimations()
{
    animations[int(AnimationIndex::WalkingRight)] = Animation(0.1f);
    animations[int(AnimationIndex::WalkingRight)].addFrame(sf::IntRect(85,0,-25,30));
    animations[int(AnimationIndex::WalkingRight)].addFrame(sf::IntRect(110, 0, -25, 30));
    animations[int(AnimationIndex::WalkingRight)].addFrame(sf::IntRect(137, 0, -25, 30));
    animations[int(AnimationIndex::WalkingRight)].addFrame(sf::IntRect(163, 0, -25, 30));

    animations[int(AnimationIndex::WalkingLeft)] = Animation(0.1f);
    animations[int(AnimationIndex::WalkingLeft)].addFrame(sf::IntRect(60, 0, 25, 30));
    animations[int(AnimationIndex::WalkingLeft)].addFrame(sf::IntRect(85, 0, 25, 30));
    animations[int(AnimationIndex::WalkingLeft)].addFrame(sf::IntRect(112, 0, 25, 30));
    animations[int(AnimationIndex::WalkingLeft)].addFrame(sf::IntRect(138, 0, 25, 30));

    animations[int(AnimationIndex::Exploding)] = Animation(0.1f);
    animations[int(AnimationIndex::Exploding)].addFrame(sf::IntRect(70, 60, 113, 120));
    animations[int(AnimationIndex::Exploding)].addFrame(sf::IntRect(180, 70, 150, 120));
    animations[int(AnimationIndex::Exploding)].addFrame(sf::IntRect(330, 75, 165, 125));
    animations[int(AnimationIndex::Exploding)].addFrame(sf::IntRect(490, 55, 165, 130));
    animations[int(AnimationIndex::Exploding)].addFrame(sf::IntRect(690, 55, 165, 130));
    animations[int(AnimationIndex::Exploding)].addFrame(sf::IntRect(900, 55, 165, 130));
    animations[int(AnimationIndex::Exploding)].addFrame(sf::IntRect(1050, 55, 165, 130));
}
std::shared_ptr<sf::Sprite> SimpleMonster::getSprite()
{
    return std::make_shared<sf::Sprite>(m_sprite);
}
void SimpleMonster::changeDirection(bool walkRight)
{
    if (walkRight) {
        m_curAnimation = AnimationIndex::WalkingRight;
        m_direction = sf::Vector2f(1, 0);
    }
    else {
        m_curAnimation = AnimationIndex::WalkingLeft;
        m_direction = sf::Vector2f(-1, 0);
    }
}

void SimpleMonster::move(sf::Time deltaTime)
{
    

    if (m_curMaxHealthPair.first != 0)
        m_sprite.move(m_direction * SIMPLE_MONSTER_SPEED * deltaTime.asSeconds());
    if(m_timeToExplode)
    {
        m_curMaxHealthPair.first = 0;
        const Resources& res = Resources::instance();
        sf::Texture* explode = res.instance().getTexture(n_explosion);
        m_sprite.setTexture(*explode);
        m_sprite.setTextureRect(sf::IntRect(80, 55, 120, 130));
        m_curAnimation = AnimationIndex::Exploding;
        animations[(int)m_curAnimation].applyToSprite(m_sprite, createNewScale());
    }
    
    if (m_curAnimation == AnimationIndex::Exploding && animations[(int)m_curAnimation].completedIteration())
    {
        m_needsToBeDeleted = true;
        return;
    }

    animations[(int)m_curAnimation].update(deltaTime.asSeconds());
    animations[(int)m_curAnimation].applyToSprite(m_sprite,createNewScale());

    m_hpBar.get()->setPosition(m_sprite.getPosition());
}


sf::Vector2f SimpleMonster::createNewScale()
{
    sf::Vector2f newScale;
    newScale.x = (m_scale.x * 26) / std::abs(animations[(int)m_curAnimation].getFrameDimensions().x);
    newScale.y = (m_scale.y * 30) / std::abs(animations[(int)m_curAnimation].getFrameDimensions().y);
    return newScale;
}

void SimpleMonster::setTimeToExplode()
{
    const Resources& res = Resources::instance();
    sf::Sound sound;
    sound.setBuffer(res.instance().get_sound(explosion_sound));
    sound.play();
    sound.setVolume(15);
    m_timeToExplode = true;
}




std::shared_ptr<HpBar> SimpleMonster::getHpBar()
{
    return m_hpBar;
}
