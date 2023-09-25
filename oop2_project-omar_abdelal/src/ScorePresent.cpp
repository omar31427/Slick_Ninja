#pragma once
#include "ScorePresent.h"
static auto registerIt = Factory<StableObject>::instance().registerType(
    obj_types::n_scorePresent,
    [](sf::Vector2f position) -> std::shared_ptr<StableObject>
    {
        return std::make_shared<ScorePresent>(position);
    }
);
ScorePresent::ScorePresent(sf::Vector2f position) :Presents(position)
{
    const Resources& res = Resources::instance();
    sf::Texture* texture = res.instance().getTexture(n_scorePresent);
    sf::Vector2f scale;
    scale.x = (float)TILE_SIZE / 116;
    scale.y = (float)TILE_SIZE / 140;
    m_sprite.setTexture(*texture);
    m_sprite.setPosition(position);
    m_sprite.setScale(scale);
    buildAnimations();
    animation.applyToSprite(m_sprite, scale);
}
void  ScorePresent::buildAnimations()
{
    animation = Animation(0.1);
    animation.addFrame(sf::IntRect(114, 308, 127, 140));
    animation.addFrame(sf::IntRect(260, 308, 100, 140));
    animation.addFrame(sf::IntRect(413, 308, 66, 140));
    animation.addFrame(sf::IntRect(568, 308, 37, 140));
    animation.addFrame(sf::IntRect(700, 308, 66, 140));
    animation.addFrame(sf::IntRect(827, 308, 100, 140));
}
std::shared_ptr<sf::Sprite> ScorePresent::getSprite()
{
    return std::make_shared<sf::Sprite>(m_sprite);
}
void ScorePresent::Update(float deltaTime)
{
    if (animation.completedIteration())
    {
        m_needsToBeDeleted = true;
        return;
    }
    animation.update(deltaTime);
    animation.applyToSprite(m_sprite, m_sprite.getScale());
}