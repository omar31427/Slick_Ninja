#pragma once
#include "Minion.h"
static auto registerIt = Factory<MovableObject>::instance().registerType(
    obj_types::n_minion,
    [](sf::Vector2f position) -> std::shared_ptr<MovableObject>
    {
        return std::make_shared<Minion>(position);
    }
);
Minion::Minion(sf::Vector2f position) : Monster(position)
{
    const Resources& res = Resources::instance();
    sf::Texture* texture = res.instance().getTexture(n_minion);
    sf::Vector2f scale;
    scale.x = (float)SMART_MONSTER_SIZE ;
    scale.y = (float)SMART_MONSTER_SIZE ;

    m_sprite.setTexture(*texture);
    m_sprite.setPosition(position);
    m_sprite.setScale(scale);
    buildAnimations();
    animation.applyToSprite(m_sprite, scale);

    m_scale = scale;
    m_prevDimensions = sf::Vector2f(140, 180);

    m_timeAlive.restart();

}

void Minion::buildAnimations()
{
    animation = Animation(0.1);
    animation.addFrame(sf::IntRect( 450, 145, 40, 45));
    animation.addFrame(sf::IntRect( 485, 185, 45, 50));
    animation.addFrame(sf::IntRect( 540, 130, 60, 72));
    animation.addFrame(sf::IntRect(610, 115, 80, 100));
    animation.addFrame(sf::IntRect(700, 105, 120, 125));
    for(int i = 0 ; i < 4 ; i++)
        animation.addFrame(sf::IntRect(227, 244, 200, 190));


}

std::shared_ptr<sf::Sprite> Minion::getSprite()
{
    return std::make_shared<sf::Sprite>(m_sprite);
}


void Minion::move(sf::Time deltaTime)
{
    // Calculate the direction vector towards the player
    sf::Vector2f direction = m_playerPosition - m_sprite.getPosition();

    // Normalize the direction vector
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0)
        direction /= length;
    //=================================================================================
    

    if (!m_timeToExplode)
        m_sprite.move(direction * SIMPLE_MONSTER_SPEED * deltaTime.asSeconds());
    
    if ( animation.completedIteration() )
    {
        m_needsToBeDeleted = true;
        return;
    }
    if(m_timeAlive.getElapsedTime().asSeconds() < 6.2 && animation.getCurFrame() < 6)
        animation.update(deltaTime.asSeconds());
    else if(m_timeAlive.getElapsedTime().asSeconds() > 6.2 || m_timeToExplode)
    {
        std::cout << "this is the current frame: " << animation.getCurFrame() << "\n";
        animation.update(deltaTime.asSeconds());
    }
    animation.applyToSprite(m_sprite, createNewScale());
    //=================================================================================

    //m_hpBar.get()->setPosition(m_sprite.getPosition());
}

sf::Vector2f Minion::createNewScale()
{
    sf::Vector2f newScale;
    newScale.x = m_scale.x / std::abs(animation.getFrameDimensions().x);
    newScale.y = m_scale.y / std::abs(animation.getFrameDimensions().y);

    return newScale;
}

void Minion::setTimeToExplode()
{
    m_timeToExplode = true;
}



void Minion::setPlayerPosition(const sf::Vector2f& position)
{
    m_playerPosition = position;
}
