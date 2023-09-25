#pragma once
#include "Tile.h"
static auto registerIt = Factory<StableObject>::instance().registerType(
    obj_types::n_tile,
    [](sf::Vector2f position) -> std::shared_ptr<StableObject>
    {
     
        return std::make_shared<Tile>(position);
    }
);
Tile::Tile(sf::Vector2f position) :StableObject(position)
{
    const Resources& res = Resources::instance();
    sf::Texture *texture = res.instance().getTexture(n_tile);
    sf::Vector2f scale;
    scale.x = (float)TILE_SIZE / texture->getSize().x;
    scale.y = (float)TILE_SIZE / texture->getSize().y;
    m_sprite.setTexture(*texture);
    m_sprite.setPosition(position);
    m_sprite.setScale(scale);
}

std::shared_ptr<sf::Sprite> Tile::getSprite()
{
	return std::make_shared<sf::Sprite>(m_sprite);
}