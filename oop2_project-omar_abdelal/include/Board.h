#pragma once
#include "GameObject.h"

class Board {
public:
	Board();
	void setMovables(std::vector<std::shared_ptr<sf::Sprite>> movables);
	void setStables(std::vector<std::shared_ptr<sf::Sprite>> stables);
	void setHpBars(std::vector<sf::RectangleShape> bars);
	void setLvl(int curLvl);
	void draw(sf::RenderWindow& window);
	void restart();
private:
	std::vector<std::shared_ptr<sf::Sprite>> m_movableSprites;
	std::vector<std::shared_ptr<sf::Sprite>> m_stableSprites;
	std::vector<sf::RectangleShape> m_hpBars;
	int m_curLvl;
};