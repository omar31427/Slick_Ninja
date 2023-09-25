#pragma once 
#include "LifePresent.h"

static auto registerIt = Factory<StableObject>::instance().registerType(
    obj_types::n_healthPresent,
    [](sf::Vector2f position) -> std::shared_ptr<StableObject>
    {
        return std::make_shared<LifePresent>(position);
    }
);

LifePresent::LifePresent(sf::Vector2f position):Presents(position)
{
    const Resources& res = Resources::instance();
    sf::Texture* texture = res.instance().getTexture(n_healthPresent);
    sf::Vector2f scale;
    scale.x = (float)TILE_SIZE / 40;
    scale.y = (float)TILE_SIZE / 35;
    m_sprite.setTexture(*texture);
    m_sprite.setPosition(position);
    m_sprite.setScale(scale);
    buildAnimations();
    animation.applyToSprite(m_sprite,scale);
}
void  LifePresent::buildAnimations() 
{
    animation = Animation(0.1);
    animation.addFrame(sf::IntRect(30, 110, 40 , 35));
    animation.addFrame(sf::IntRect(84, 110, 36, 35));
    animation.addFrame(sf::IntRect(130, 110, 27, 35));
    animation.addFrame(sf::IntRect(168, 110, 23, 35));
    animation.addFrame(sf::IntRect(197, 110, 22, 35));
    animation.addFrame(sf::IntRect(226, 110, 23, 35));
    animation.addFrame(sf::IntRect(260, 110, 27, 35));
    animation.addFrame(sf::IntRect(299, 110, 36, 35));
    animation.addFrame(sf::IntRect(346, 110, 40, 35));

}
std::shared_ptr<sf::Sprite> LifePresent::getSprite()
{
	return std::make_shared<sf::Sprite>(m_sprite);
}
void LifePresent::Update(float deltaTime)
{
   
    animation.update(deltaTime);
    
    animation.applyToSprite(m_sprite, m_sprite.getScale());
}