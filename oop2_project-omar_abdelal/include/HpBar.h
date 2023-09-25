#pragma once
#include <SFML/Graphics.hpp>
#include <utility>
#include "Resources.h"
class HpBar
{
public:
    HpBar() = default;
    HpBar(sf::Vector2f position, float width);
   
    void setPosition(sf::Vector2f position);

    void setHealth(const std::pair<float, float> curMaxHealth);

    sf::RectangleShape getStaticBar();

    sf::RectangleShape getDynamicBar();

private:
    sf::Vector2f m_position;
    //width needs to be here because it has to be the same width as the charachter holding it
    //height is table for all charachters so its defined in resources
    float m_width;
    //=============================================
    //static bar is the background bar it stays red
    //and its job is to create contrast so its easier
    //to read the hp  bar
    sf::RectangleShape m_staticBar;
    //==============================================
    //dynamic bar is the bar that shows the true current
    //health its color would be green
    sf::RectangleShape m_dynamicBar;
};