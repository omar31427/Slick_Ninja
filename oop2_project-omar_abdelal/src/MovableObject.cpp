#pragma once
#include "MovableObject.h"

void MovableObject::setHealth(float newHealth)
{
    if ( m_damageClock.getElapsedTime().asSeconds() < 0.4)
        return;
    
    m_damageClock.restart();
    m_curMaxHealthPair.first += newHealth;

    if (m_curMaxHealthPair.first <= 0)
    {
        m_createPresent = true;
        m_timeToExplode = true;
        m_curMaxHealthPair.first = 0;
    }
    else if (m_curMaxHealthPair.first > m_curMaxHealthPair.second)
        m_curMaxHealthPair.first = m_curMaxHealthPair.second;


    m_hpBar.get()->setHealth(m_curMaxHealthPair);
}

void MovableObject::setMonster()
{
    m_monster = true;
}
