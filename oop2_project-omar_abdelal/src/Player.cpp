#pragma once
#include "Player.h"

static auto registerIt = Factory<MovableObject>::instance().registerType(
     obj_types::n_player,
     [](sf::Vector2f position) -> std::shared_ptr<MovableObject>
     {
         return std::make_shared<Player>(position);
     }
 );
namespace
{


    sf::Vector2f dirFromKey()
    {
        //    
        //    
        //    
        //    sf::Keyboard::Space
        static const
            std::initializer_list<std::pair<sf::Keyboard::Key, sf::Vector2f>>
            keyToVectorMapping =
        {
            { sf::Keyboard::Right, { 1, 0 } },
            { sf::Keyboard::Left , { -1, 0 } },
        };
        sf::Vector2f direction(0.f, 0.f);

        for (const auto& pair : keyToVectorMapping)
        {
            if (sf::Keyboard::isKeyPressed(pair.first))
            {
                direction += pair.second;
            }
        }

        return direction;
    }

}
void Player::buildAnimations()
{

    animations[int(AnimationIndex::WalkingRight)] = Animation(0.08f); 
    animations[int(AnimationIndex::WalkingRight)].addFrame(sf::IntRect(473, 0, 103, 123));
    animations[int(AnimationIndex::WalkingRight)].addFrame(sf::IntRect(575, 0, 115, 134));
    animations[int(AnimationIndex::WalkingRight)].addFrame(sf::IntRect(703, 0, 115, 134));
    animations[int(AnimationIndex::WalkingRight)].addFrame(sf::IntRect(453, 124, 112, 134));
    animations[int(AnimationIndex::WalkingRight)].addFrame(sf::IntRect(571, 124, 117, 134));
    animations[int(AnimationIndex::WalkingRight)].addFrame(sf::IntRect(757, 124, 117, 134));


    animations[int(AnimationIndex::WalkingLeft)] = Animation(0.08f);
    animations[int(AnimationIndex::WalkingLeft)].addFrame(sf::IntRect(576, 0, -103, 123));
    animations[int(AnimationIndex::WalkingLeft)].addFrame(sf::IntRect(690, 0, -115, 134));
    animations[int(AnimationIndex::WalkingLeft)].addFrame(sf::IntRect(818, 0, -115, 134));
    animations[int(AnimationIndex::WalkingLeft)].addFrame(sf::IntRect(565, 124, -112, 134));
    animations[int(AnimationIndex::WalkingLeft)].addFrame(sf::IntRect(688, 124, -117, 134));
    animations[int(AnimationIndex::WalkingLeft)].addFrame(sf::IntRect(874, 124, -117, 134));
    
    animations[int(AnimationIndex::JumpingRight)] = Animation(0.f);
    animations[int(AnimationIndex::JumpingRight)].addFrame(sf::IntRect(509,254,120,137));

    animations[int(AnimationIndex::JumpingLeft)] = Animation(0.f);
    animations[int(AnimationIndex::JumpingLeft)].addFrame(sf::IntRect(630, 254, -120, 137));

    animations[int(AnimationIndex::FallingRight)] = Animation(0.f);
    animations[int(AnimationIndex::FallingRight)].addFrame(sf::IntRect(675, 236, 102, 140));

    animations[int(AnimationIndex::FallingLeft)] = Animation(0.f);
    animations[int(AnimationIndex::FallingLeft)].addFrame(sf::IntRect(778, 236, -120, 140));
    //sliding right is when the player is facing right and slidint on a wall to the left of him
    animations[int(AnimationIndex::SlidingRight)] = Animation(0.1f);
    animations[int(AnimationIndex::SlidingRight)].addFrame(sf::IntRect(435, 685, 125, 202));
    animations[int(AnimationIndex::SlidingRight)].addFrame(sf::IntRect(560, 689, 125, 202));
    animations[int(AnimationIndex::SlidingRight)].addFrame(sf::IntRect(685, 689, 125, 202));

    animations[int(AnimationIndex::SlidingLeft)] = Animation(0.1f);
    animations[int(AnimationIndex::SlidingLeft)].addFrame(sf::IntRect(810, 689, -125, 202));
    animations[int(AnimationIndex::SlidingLeft)].addFrame(sf::IntRect(685, 689, -125, 202));
    animations[int(AnimationIndex::SlidingLeft)].addFrame(sf::IntRect(560, 689, -125, 202));

    animations[int(AnimationIndex::ShootingRight)] = Animation(0.1);
    animations[int(AnimationIndex::ShootingRight)].addFrame(sf::IntRect(430,900,133,134));
    animations[int(AnimationIndex::ShootingRight)].addFrame(sf::IntRect(655, 900, 133, 134));

    animations[int(AnimationIndex::ShootingLeft)] = Animation(0.1);
    animations[int(AnimationIndex::ShootingLeft)].addFrame(sf::IntRect(788, 900, -133, 134));
    animations[int(AnimationIndex::ShootingLeft)].addFrame(sf::IntRect(563, 900, -133, 134));
}
Player::Player(sf::Vector2f position) :MovableObject(position), m_score(0)
{
    m_damageClock.restart();
	const Resources& res = Resources::instance();

    //===============================================================
    //here we setup the animations
    m_curAnimation = AnimationIndex::WalkingRight;
    m_sprite.setTextureRect(sf::IntRect(470, 0, 116, 134));
    buildAnimations();
    //================================================================



    //===============================================================
    //here we situp the sprite
    sf::Texture* texture = res.instance().getTexture(n_player);
    m_playerScale.x = (float)PLAYER_SIZE/116.f ;
    m_playerScale.y = (float)PLAYER_SIZE / 134.f;
	m_sprite.setTexture(*texture);
	m_sprite.setPosition(position);
    
    animations[(int)m_curAnimation].applyToSprite(m_sprite,m_playerScale);
    m_sprite.setScale(m_playerScale);
    //===============================================================
  
    //===============================================================
    //here we setup the hp bar object
    m_curMaxHealthPair.first = PLAYER_MAX_HEALTH;
    m_curMaxHealthPair.second = PLAYER_MAX_HEALTH;

    float width = m_sprite.getGlobalBounds().width;
    m_hpBar = std::make_shared<HpBar>(m_sprite.getPosition(), width);
    //===============================================================

    //===============================================================
    //projectile settings
    //m_shootingClock.restart();
    //===============================================================
    
}

std::shared_ptr<sf::Sprite> Player::getSprite()
{
	return std::make_shared<sf::Sprite>(m_sprite);
}
bool Player::isConnected()
{
    return m_isConnected;
}
void Player::addForbiddenMove(const sf::Vector2f& move, const float& margin, const float& horizontalDifference, const float& verticalDifference)
{

    if (move.x == 0 && move.y == 1 && horizontalDifference <= TILE_SIZE/2 && verticalDifference >= TILE_SIZE)
    {
        isGrounded = true;
        m_isConnected = false;
        m_previousConnectedTile = 0;
    }//this means the player collided with something from the top side and now we have to stop the jump so that the player wont "fly:
    else if (move.x == 0 && move.y == -1)
    {
        m_isJumping = false;
        m_isConnected = false;
    }//here the player collided with something from right or left while not being grounded , he could be in mid at this moment
    //m_previousConnectedTile indicates where is the tile im connected to, if it is -1 then the tile is to the left
    //if it is 1 then the tile is on the right
    else if (move.y == 0 && move.x != 0 && !isGrounded && (move.x != m_previousConnectedTile))
    {
        m_isConnected = true;
        m_previousConnectedTile = move.x;
    }

    
    

    m_sprite.move(move * margin * -0.2f);
    

    for (auto nextMove : m_forbiddenMoves)
        if (nextMove == move)
            return;


    if (move.x != 0 && verticalDifference >= TILE_SIZE)
        return;

    m_forbiddenMoves.push_back(move);
}
void Player::jump(sf::Time deltaTime) {
/*
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && (isGrounded || m_isConnected))
    {
        m_jumpStartY = m_sprite.getPosition().y;
        m_verticalVelocity = JUMP_VELOCITY;
        m_isJumping = true;
        isGrounded = false;
        m_isConnected = false;
    }


    if (m_isJumping)
    {
        float gravityForce = GRAVITY * deltaTime.asSeconds();

        m_sprite.move(0.0f, m_verticalVelocity * deltaTime.asSeconds());

        m_verticalVelocity += gravityForce;

        if (m_sprite.getPosition().y <= m_jumpStartY - JUMP_HEIGHT || isGrounded)
        {
            m_isJumping = false;
            m_verticalVelocity = 0.0f;
        }

    }*/
}

std::shared_ptr<HpBar> Player::getHpBar()
{
    return m_hpBar;
}

//void Player::setHealth(float newHealth)
//{
 //   m_curMaxHealthPair.first = newHealth;
  //  m_hpBar.get()->setHealth(m_curMaxHealthPair);
//}

void Player::detattachTiles(bool deAttach)
{
    if( !m_isJumping && deAttach && m_fallingClock.getElapsedTime().asSeconds() > 0.01 && m_shootingClock.getElapsedTime().asSeconds() > 0.2)
    {
        if (m_curAnimation == AnimationIndex::WalkingRight || m_curAnimation == AnimationIndex::JumpingRight 
            || m_curAnimation == AnimationIndex::SlidingRight || m_curAnimation == AnimationIndex::ShootingRight)
        {
            m_curAnimation = AnimationIndex::FallingRight;
            m_facingRight = true;
        }
        else if (m_curAnimation == AnimationIndex::WalkingLeft || m_curAnimation == AnimationIndex::JumpingLeft 
            || m_curAnimation == AnimationIndex::SlidingLeft || m_curAnimation == AnimationIndex::ShootingLeft)
        {
            m_curAnimation = AnimationIndex::FallingLeft;
            m_facingRight = false;
        }
    }

    if (deAttach)
    {
       // m_collisionDetected = false;
        m_isConnected = false;
        isGrounded = false;
    }
}
void Player::shoot() 
{
    
    if (m_shootingClock.getElapsedTime().asSeconds() > 0.4 && m_ammo > 0)
    {
        m_isConnected = false;
        if (m_facingRight)
            m_curAnimation = AnimationIndex::ShootingRight;
        else
            m_curAnimation = AnimationIndex::ShootingLeft;

        m_shootingClock.restart();
        --m_ammo;
        m_createProjectile = true;
    }
}

sf::Vector2f Player::releaseShot()
{
    
    if(m_createProjectile)
    {
        
        m_createProjectile = false;
        if (m_facingRight)
            return sf::Vector2f(1, 0);
        else
            return sf::Vector2f(-1, 0);
    }
    //if this value is reutrned then the player cant shoot
    return sf::Vector2f(0,0);
}

bool Player::canICreateProjectile()
{
    return m_createProjectile;
}

void Player::addScore(int score)
{
    m_score += score;
    std::cout << "SDddddddddd\n";
}

int Player::getScore() const
{
    return m_score;
}

void Player::move(sf::Time deltaTime)
{

    sf::Vector2f nextMove = dirFromKey();


    for (auto move : m_forbiddenMoves)
        if (nextMove == move)
        {
            nextMove.x = 0;
            nextMove.y = 0;
        }

        

    m_sprite.move(nextMove * PLAYER_SPEED * deltaTime.asSeconds());
  
    //=====================================================================================
    //=====================================================================================
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        m_curMaxHealthPair.first = PLAYER_MAX_HEALTH;
        m_hpBar.get()->setHealth(m_curMaxHealthPair);
    }
    //THIS IS FOR DEVELOPMENT DELETE LATE

    //=====================================================================================
    //=====================================================================================
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && (isGrounded || m_isConnected))
    {
        if (m_shootingClock.getElapsedTime().asSeconds() > 0.2 && (m_curAnimation == AnimationIndex::WalkingRight
            || m_curAnimation == AnimationIndex::SlidingRight))
        {
            m_curAnimation = AnimationIndex::JumpingRight;
            m_facingRight = true;
        }
        else if (m_shootingClock.getElapsedTime().asSeconds() > 0.2)
        {
            m_curAnimation = AnimationIndex::JumpingLeft;
            m_facingRight = false;
        }

        m_jumpStartY = m_sprite.getPosition().y;
        m_isJumping = true;
        isGrounded = false;
        m_isConnected = false;
    }

    if (m_isJumping)
    {
        m_sprite.move(0.0f, JUMP_VELOCITY * deltaTime.asSeconds());

        if (m_sprite.getPosition().y <= m_jumpStartY - JUMP_HEIGHT || isGrounded || m_isConnected)
            m_isJumping = false;
        if (m_shootingClock.getElapsedTime().asSeconds() > 0.2 && nextMove.x > 0
            && (m_curAnimation == AnimationIndex::JumpingLeft || m_curAnimation == AnimationIndex::ShootingRight))
        {
            m_curAnimation = AnimationIndex::JumpingRight;
            m_facingRight = true;
        }
        else if (m_shootingClock.getElapsedTime().asSeconds() > 0.2
            && (nextMove.x < 0 && m_curAnimation == AnimationIndex::JumpingRight || m_curAnimation == AnimationIndex::ShootingLeft))
        {
            m_curAnimation = AnimationIndex::JumpingLeft;
            m_facingRight = false;
        }

    }
    else if (!m_isConnected && !isGrounded)
    {
        if (m_shootingClock.getElapsedTime().asSeconds() > 0.2 && nextMove.x > 0
            && (m_curAnimation == AnimationIndex::FallingLeft || m_curAnimation == AnimationIndex::ShootingLeft))
        {
            m_curAnimation = AnimationIndex::FallingRight;
            m_facingRight = true;
        }
        else if (m_shootingClock.getElapsedTime().asSeconds() > 0.2 && nextMove.x < 0
            && (m_curAnimation == AnimationIndex::FallingRight || m_curAnimation == AnimationIndex::ShootingRight))
        {
            m_curAnimation = AnimationIndex::FallingLeft;
            m_facingRight = false;
        }
        m_sprite.move(0.0f, GRAVITY * deltaTime.asSeconds());
    }
    else if (m_isConnected && !isGrounded)
    {
        if (m_curAnimation == AnimationIndex::JumpingLeft || m_curAnimation == AnimationIndex::FallingLeft)
        {
            m_curAnimation = AnimationIndex::SlidingRight;
            m_facingRight = true;
        }
        else if (m_curAnimation == AnimationIndex::JumpingRight || m_curAnimation == AnimationIndex::FallingRight)
        {
            m_curAnimation = AnimationIndex::SlidingLeft;
            m_facingRight = false;
        }
        m_sprite.move(0.0f, GRAVITY * deltaTime.asSeconds() / 4);
    }
    else if (!m_isConnected && isGrounded && m_shootingClock.getElapsedTime().asSeconds() > 0.2) {
        m_fallingClock.restart();

        m_sprite.move(nextMove.x * -1 * deltaTime.asSeconds(), nextMove.y * -1 * deltaTime.asSeconds());
        if (m_curAnimation == AnimationIndex::FallingRight || m_curAnimation == AnimationIndex::JumpingRight ||
            m_curAnimation == AnimationIndex::SlidingRight || m_curAnimation == AnimationIndex::ShootingRight || nextMove.x > 0)
        {
            m_curAnimation = AnimationIndex::WalkingRight;
            m_facingRight = true;
        }
        else if (m_curAnimation == AnimationIndex::FallingLeft || m_curAnimation == AnimationIndex::JumpingLeft ||
            m_curAnimation == AnimationIndex::SlidingLeft || m_curAnimation == AnimationIndex::ShootingLeft || nextMove.x < 0)
        {
            m_curAnimation = AnimationIndex::WalkingLeft;
            m_facingRight = false;
        }
    }


    if (nextMove.x != 0 || nextMove.y != 0)
    {
       // isGrounded = false;
        m_isConnected = false;
    }

    if ((nextMove.x ==0) && (m_curAnimation == AnimationIndex::WalkingRight||m_curAnimation == AnimationIndex::WalkingLeft))
        animations[(int)m_curAnimation].update(0);
    else
        animations[(int)m_curAnimation].update(deltaTime.asSeconds());
    
    animations[(int)m_curAnimation].applyToSprite(m_sprite,createNewPlayerScale());
 
    if(m_hpBar!=nullptr)
        m_hpBar.get()->setPosition(m_sprite.getPosition());
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
    m_forbiddenMoves.clear();
}

sf::Vector2f Player::createNewPlayerScale() 
{
    sf::Vector2f newScale;
    newScale.x = (m_playerScale.x * 116) / std::abs(animations[(int)m_curAnimation].getFrameDimensions().x);
    newScale.y = (m_playerScale.y * 137) / std::abs(animations[(int)m_curAnimation].getFrameDimensions().y);
    return newScale;
}
