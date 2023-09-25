#pragma once
#include "Projectile.h"
static auto registerIt = Factory<MovableObject>::instance().registerType(
    obj_types::n_projectile,
    [](sf::Vector2f position) -> std::shared_ptr<MovableObject>
    {
        return std::make_shared<Projectile>(position);
    }
);

std::shared_ptr<sf::Sprite> Projectile::getSprite()
{
    return std::make_shared<sf::Sprite>(m_sprite);
}

Projectile::Projectile(const sf::Vector2f& position) : MovableObject(position) 
{
    const Resources& res = Resources::instance();
    sf::Texture* texture = res.instance().getTexture(n_projectile);
    sf::Vector2f scale;
    scale.x = (float)PROJECTILE_SIZE/ 103;
    scale.y = (float)PROJECTILE_SIZE / 120;
    m_curAnimation = AnimationIndex::shuriken;
    m_sprite.setTexture(*texture);
    buildAnimations();
    animations[(int)m_curAnimation].applyToSprite(m_sprite, scale);
    m_sprite.setPosition(position);
    m_sprite.setScale(scale);
}
void Projectile::buildAnimations()
{
    animations[(int)AnimationIndex::shuriken] = Animation(0.1f);
    for (int i = 0; i < 4; i++)
        animations[(int)AnimationIndex::shuriken].addFrame(sf::IntRect(i * 153, 145, 104, 120));

    animations[(int)AnimationIndex::fireball] = Animation(0.12f);
    for(int i = 0 ; i < 6 ; i++)
        animations[(int)AnimationIndex::fireball].addFrame(sf::IntRect( 918 -(i*153), 0, -153, 150));
    


}
void Projectile::setAsFireball()
{
    m_curAnimation = fireball;
    animations[(int)m_curAnimation].applyToSprite( m_sprite, m_sprite.getScale());
}

void Projectile::move(sf::Time deltaTime)
{
    float dt = deltaTime.asSeconds();
        
    animations[(int)m_curAnimation].update(deltaTime.asSeconds());
    animations[(int)m_curAnimation].applyToSprite(m_sprite,createNewScale());
    if(m_shooter == obj_types::n_player)
        m_sprite.move(m_direction * PLAYER_SPEED*5.f * deltaTime.asSeconds());
    else
        m_sprite.move(m_direction * PLAYER_SPEED * 2.5f * deltaTime.asSeconds());
}

void Projectile::setPlayerPosition(  const sf::Vector2f& position)
{
    m_playerPosition = position;
}

void Projectile::setDirection(sf::Vector2f direction)
{
    m_direction = direction;
}

void Projectile::setShooter(const obj_types& shooterIdentity)
{
    m_shooter = shooterIdentity;
    if (shooterIdentity == obj_types::n_boss&&m_curAnimation!= AnimationIndex::fireball)
    {
        const Resources& res = Resources::instance();
        sf::Texture* fireball = res.instance().getTexture(n_fireball);
        m_sprite.setTexture(*fireball);
        m_curAnimation = AnimationIndex::fireball;
        animations[(int)m_curAnimation].applyToSprite(m_sprite, createNewScale());
    }
    
}

obj_types Projectile::getShooter()
{
    return m_shooter;
}
sf::Vector2f Projectile::createNewScale()
{
    if (m_shooter == obj_types::n_player)
        return sf::Vector2f((float)PROJECTILE_SIZE / 103.f, (float)PROJECTILE_SIZE / 120.f);

    if (m_shooter == obj_types::n_boss)
        return sf::Vector2f((float)PROJECTILE_SIZE / 100, (float)PROJECTILE_SIZE /100 );
}   


