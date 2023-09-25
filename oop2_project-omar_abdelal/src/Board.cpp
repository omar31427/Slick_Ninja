#pragma once
#include "Board.h"

Board::Board()
{
}

void Board::setMovables(std::vector<std::shared_ptr<sf::Sprite>> movables)
{
	m_movableSprites.clear();
	m_movableSprites = movables;
}
void Board::setStables(std::vector<std::shared_ptr<sf::Sprite>> stables)
{
	m_stableSprites.clear();
	m_stableSprites = stables;
}

void Board::setHpBars(std::vector<sf::RectangleShape> bars)
{
	m_hpBars.clear();
	m_hpBars = bars;
}
void Board::setLvl(int curLvl)
{
	m_curLvl = curLvl;
}
void Board::draw(sf::RenderWindow& window)
{
	const Resources& res = Resources::instance();
	sf::RectangleShape holdingRectangle;
	sf::Vector2f curLvlDimensions(res.instance().getLvlWidth(m_curLvl) * TILE_SIZE, res.instance().getLvlHeight(m_curLvl) * TILE_SIZE);
	holdingRectangle.setPosition(sf::Vector2f(0, 0));
	holdingRectangle.setSize(curLvlDimensions);
	holdingRectangle.setTexture(res.instance().getTexture(n_background));

	window.draw(holdingRectangle);
	for (auto& stable : m_stableSprites)
		window.draw(*stable);
	for (auto& movable : m_movableSprites)
		window.draw(*movable);
	for (auto bar : m_hpBars)
		window.draw(bar);
}

void Board::restart() {
	m_hpBars.clear();
	m_movableSprites.clear();
	m_stableSprites.clear();
}
