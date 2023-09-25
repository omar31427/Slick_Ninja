#pragma once
#include "Boss.h"

static auto registerIt = Factory<MovableObject>::instance().registerType(
    obj_types::n_boss,
    [](sf::Vector2f position) -> std::shared_ptr<MovableObject>
    {
        return std::make_shared<Boss>(position);
    }
);

Boss::Boss(sf::Vector2f position) :Monster(position)
{
    m_damageClock.restart();
    m_direction = sf::Vector2f(1, 0);
    const Resources& res = Resources::instance();
    sf::Texture* texture = res.instance().getTexture(n_breakingWall);
    sf::Vector2f scale;
    scale.x = (float)BOSS_SIZE / 115;
    scale.y = (float)BOSS_SIZE / 100;

    m_sprite.setTexture(*texture);
    m_sprite.setTextureRect(sf::IntRect(280, 125, 115, 100));
    m_sprite.setPosition(position);
    m_sprite.setScale(scale);
    m_curAnimation = AnimationIndex::wall;
    buildAnimations();
    m_scale = sf::Vector2f(BOSS_SIZE,BOSS_SIZE);
    m_animation[m_curAnimation].applyToSprite(m_sprite, createNewScale());

    m_curMaxHealthPair.first = BOSS_MAX_HEALTH;
    m_curMaxHealthPair.second = BOSS_MAX_HEALTH;

    float width = m_sprite.getGlobalBounds().width;
    m_hpBar = std::make_shared<HpBar>(m_sprite.getPosition(), width);
   
    setMonster();
}

void Boss::buildAnimations()
{
    m_animation[(int)AnimationIndex::active] = Animation(0.12);
    m_animation[(int)AnimationIndex::active].addFrame(sf::IntRect(8, 0, 83, 160));
    m_animation[(int)AnimationIndex::active].addFrame(sf::IntRect(111, 0, 113, 160));
    m_animation[(int)AnimationIndex::active].addFrame(sf::IntRect(242, 0, 72, 160));
    m_animation[(int)AnimationIndex::active].addFrame(sf::IntRect(333, 0, 113, 160));
    m_animation[(int)AnimationIndex::active].addFrame(sf::IntRect(449, 0, 87, 160));

    m_animation[(int)AnimationIndex::wall] = Animation(0.15);
    for (int i = 0; i < 6; i++)
        m_animation[(int)AnimationIndex::wall].addFrame(sf::IntRect(280+(i*115), 125,115,100));

}
std::shared_ptr<sf::Sprite> Boss::getSprite()
{
    return std::make_shared<sf::Sprite>(m_sprite);
}
obj_types Boss::getAttType()
{
    return m_attType;
}

void Boss::activateBoss()
{
    if (m_isActivated)
        return;
    m_normalAttClock.restart();
    m_specAttClock.restart();
    
    m_isActivated = true;
}
void Boss::move(sf::Time deltaTime)
{
   
    
    if (m_curAnimation == wall && m_animation[m_curAnimation].completedIteration())
        m_curAnimation = active;

    if (m_isActivated)
    {
        if (m_curAnimation == wall && !m_animation[m_curAnimation].completedIteration())
        {
            m_animation[m_curAnimation].update(deltaTime.asSeconds());
        }
  
        if ( m_curAnimation == active)
        {
            m_createProjectile = true;
            m_isAlive = true;
            const Resources& res = Resources::instance();
            sf::Texture* demon = res.instance().getTexture(n_boss);
            m_sprite.setTexture(*demon);
           
            m_curAnimation = AnimationIndex::active;
            m_animation[(int)m_curAnimation].applyToSprite(m_sprite, createNewScale());

            m_animation[m_curAnimation].update(deltaTime.asSeconds());
           
        }
    }
    m_animation[m_curAnimation].applyToSprite(m_sprite, m_sprite.getScale());
    m_hpBar.get()->setPosition(m_sprite.getPosition());
}


sf::Vector2f Boss::createNewScale()
{
    sf::Vector2f newScale;
    newScale.x = m_scale.x / std::abs(m_animation[m_curAnimation].getFrameDimensions().x);
    newScale.y = m_scale.y / std::abs(m_animation[m_curAnimation].getFrameDimensions().y);

    return newScale;
}
void Boss::setTimeToExplode()
{
    m_timeToExplode = true;
}

std::shared_ptr<HpBar> Boss::getHpBar()
{
    return m_hpBar;
}

bool Boss::canICreateProjectile()
{
    if (!m_createProjectile)
        return false;
    std::cout << "normal clock time: " << m_normalAttClock.getElapsedTime().asSeconds() << "\n";
    if (m_normalAttClock.getElapsedTime().asSeconds() > 1.5)
    {
        std::cout << "i can normal create projectile\n";
        m_attType = n_fireball;
        m_normalAttClock.restart();
        return true;
    }
    if (m_specAttClock.getElapsedTime().asSeconds() > 10.f)
    {
        std::cout << "i can epiccreate projectile\n";
        m_attType = n_minion;
        m_specAttClock.restart();
        return true;
    }

    return false;
}

void Boss::setPlayerPosition( const sf::Vector2f& position)
{
  /*  if (m_createProjectile)
        return;
    if (m_sprite.getPosition().x - position.x < 500)
    {
        m_createProjectile = true;
        activateBoss();
    }*/
}
