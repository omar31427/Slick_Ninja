#pragma once
#include "HpBar.h"

HpBar::HpBar(sf::Vector2f position, float width) :  m_width(width)
{
    m_position.y = position.y - HP_BAR_HEIGHT;
    m_position.x = position.x;
    m_staticBar.setSize(sf::Vector2f(m_width, HP_BAR_HEIGHT));
    m_staticBar.setFillColor(sf::Color::Red);
    m_staticBar.setPosition(m_position);

    m_dynamicBar.setSize(sf::Vector2f(m_width, HP_BAR_HEIGHT));
    m_dynamicBar.setFillColor(sf::Color::Green);
    m_dynamicBar.setPosition(m_position);
}

void HpBar::setPosition(sf::Vector2f position)
{
    m_position.y = position.y - HP_BAR_HEIGHT;
    m_position.x = position.x;
    m_dynamicBar.setPosition(m_position);
    m_staticBar.setPosition(m_position);
}

void HpBar::setHealth(const std::pair<float, float> curMaxHealth)
{
    float healthRatio = curMaxHealth.first / curMaxHealth.second;
    if (healthRatio < 0)
        healthRatio = 0;

    m_dynamicBar.setSize(sf::Vector2f(m_width * healthRatio, HP_BAR_HEIGHT));
}

sf::RectangleShape HpBar::getStaticBar()
{
    return m_staticBar;
}

sf::RectangleShape HpBar::getDynamicBar()
{
    return m_dynamicBar;
}

