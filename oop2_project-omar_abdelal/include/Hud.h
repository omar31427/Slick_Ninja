#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Resources.h"

class Hud {
public:
	Hud();
	Hud(sf::FloatRect dimensions);
	void createScoreRect(sf::FloatRect dimensions);
	void createRestartRect(sf::FloatRect dimensions);
	int getIntersectingRect(sf::Vector2f mousePos);
	void draw(sf::RenderWindow& window);
	void updatePosAll(float posX);
	void setScore(int);
	void clearScore();
private:
	sf::RectangleShape m_holdingRect;
	sf::RectangleShape m_restartRect;
	sf::RectangleShape m_menuRect;
	sf::RectangleShape m_scoreRect;
	

	std::vector<sf::RectangleShape> m_clickableRects;
	std::vector<sf::Text> m_texts;
	int m_scoreAmt;

	sf::Text m_scoreText;
	sf::Text m_restartText;
	sf::Text m_menuText;

	
};