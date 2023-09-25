#pragma once
#include "MovableObject.h"
class Projectile : public MovableObject
{
public:
    std::shared_ptr<sf::Sprite> getSprite();
    Projectile(const sf::Vector2f& position);
    void buildAnimations();
    void setAsFireball() override;
    void move(sf::Time deltaTime)override;
    void setPlayerPosition(  const sf::Vector2f& position) override;
    void setDirection(sf::Vector2f direction) override;
    void setShooter(const obj_types& shooterIdentity)override;
    obj_types getShooter()override;
    sf::Vector2f createNewScale();
private:
    enum AnimationIndex {
        fireball, shuriken, Count
    };
    float m_projSpeed;
    sf::Vector2f m_playerPosition;
    AnimationIndex m_curAnimation;
    Animation animations[(int)AnimationIndex::Count];
    obj_types m_shooter;
    sf::Sprite m_sprite;
    sf::Vector2f m_direction;
};
