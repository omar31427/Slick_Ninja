#pragma once
#include "Hud.h"

Hud::Hud(): m_scoreAmt(0)
{
}

Hud::Hud(sf::FloatRect dimensions): m_scoreAmt(0)
{
	m_holdingRect = sf::RectangleShape::RectangleShape(sf::Vector2f(dimensions.width,dimensions.height));
	m_holdingRect.setPosition(sf::Vector2f(dimensions.left,dimensions.top));
	m_holdingRect.setFillColor(sf::Color::Blue);

	createScoreRect(dimensions);
	createRestartRect(dimensions);
}

void Hud::createScoreRect(sf::FloatRect dimensions)
{
	const Resources& res = Resources::instance();

	m_scoreRect.setSize(sf::Vector2f(100,25));
	m_scoreRect.setPosition(sf::Vector2f(m_holdingRect.getGlobalBounds().left + 10, m_holdingRect.getGlobalBounds().top + 10));
	m_scoreRect.setFillColor(sf::Color::Green);
	m_clickableRects.push_back(m_scoreRect);
	std::string scoreStr = "score = " + std::to_string(m_scoreAmt);
	m_scoreText.setString(scoreStr);
	m_scoreText.setCharacterSize(17);
	m_scoreText.setPosition(sf::Vector2f(m_scoreRect.getGlobalBounds().left, m_scoreRect.getGlobalBounds().top));
	m_scoreText.setFont(res.instance().get_font(f_menuscreen));
	m_scoreText.setFillColor(sf::Color::Black);
	m_texts.push_back(m_scoreText);
	
}
void Hud::createRestartRect(sf::FloatRect dimensions)
{
	const Resources& res = Resources::instance();

	m_restartRect.setSize(sf::Vector2f(100, 25));
	m_restartRect.setPosition(sf::Vector2f(m_holdingRect.getGlobalBounds().left + 200, m_holdingRect.getGlobalBounds().top + 10));
	m_restartRect.setFillColor(sf::Color::Green);
	m_clickableRects.push_back(m_restartRect);

	m_restartText.setString("restart");
	m_restartText.setCharacterSize(17);
	m_restartText.setPosition(sf::Vector2f(m_restartRect.getGlobalBounds().left, m_restartRect.getGlobalBounds().top));
	m_restartText.setFont(res.instance().get_font(f_menuscreen));
	m_restartText.setFillColor(sf::Color::Black);
	m_texts.push_back(m_restartText);

}
int Hud::getIntersectingRect(sf::Vector2f mousePos)
{
	for (int i = 0; i < m_clickableRects.size(); i++)
		if (m_clickableRects[i].getGlobalBounds().contains(mousePos))
			return i;

	return -1;
}
void Hud::draw(sf::RenderWindow& window)
{
	std::string scoreStr = "score = " + std::to_string(m_scoreAmt);
	m_texts[0].setString(scoreStr);
	window.draw(m_holdingRect);
	for (auto rect : m_clickableRects)
		window.draw(rect);
	for(auto text : m_texts)
		window.draw(text);
}
void Hud::updatePosAll(float posX)
{
	//std::cout << "posX: " << posX << "\n";
	m_holdingRect.setPosition(sf::Vector2f(posX, m_holdingRect.getPosition().y));
	float margin = 10;
	for (auto& rect : m_clickableRects)
	{
		rect.setPosition(m_holdingRect.getPosition().x + margin, rect.getPosition().y);
		margin += 110;
	}
	margin = 10;
	for (auto& text : m_texts)
	{
		text.setPosition(m_holdingRect.getPosition().x + margin, text.getPosition().y);
		margin += 110;
	}
	std::string scoreStr= "score = " + std::to_string(m_scoreAmt);
	m_texts[0].setString(scoreStr);
}

void Hud::setScore(int score)
{
	m_scoreAmt = score;
	std::cout << "\n\nsetScore " << m_scoreAmt << "\n";
}

void Hud::clearScore()
{
	m_scoreAmt = 0;
}
