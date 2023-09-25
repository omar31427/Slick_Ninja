#include "Resources.h"

class Menu {
public:
	Menu();
	void show(bool & start);
	void resizeMenuSprite(sf::RenderWindow& window, sf::Sprite& menuSprite);
private:
	sf::Sprite m_sprite;
};