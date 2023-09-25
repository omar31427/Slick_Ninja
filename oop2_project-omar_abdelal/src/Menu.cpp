#include "Menu.h"

Menu::Menu() 
{
	const Resources& res = Resources::instance();
	sf::Texture* texture = res.instance().getTexture(n_menu);

	m_sprite.setTexture(*texture);
	m_sprite.setPosition(0, 0);
}

void Menu::show(bool & start)
{
    sf::RenderWindow window(sf::VideoMode(725, 610), "Menu");
    sf::Texture* texture = Resources::instance().instance().getTexture(n_menu);
    sf::Sprite menuSprite(*texture);
    sf::Vector2i buttonPosition(264, 496);

    resizeMenuSprite(window, menuSprite);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                if (mousePosition.x >= buttonPosition.x && mousePosition.x <= buttonPosition.x + 195 &&
                    mousePosition.y >= buttonPosition.y && mousePosition.y <= buttonPosition.y + 58) {
                
                    start = true;
                    break;
                }
            }
        }

        if (start)
            window.close();
        window.clear();
        window.draw(menuSprite);
        window.display();
    }
}

void Menu::resizeMenuSprite(sf::RenderWindow& window, sf::Sprite& menuSprite)
{
    sf::Vector2u windowSize = window.getSize();

    float scaleRatioX = static_cast<float>(windowSize.x) / menuSprite.getTexture()->getSize().x;
    float scaleRatioY = static_cast<float>(windowSize.y) / menuSprite.getTexture()->getSize().y;

    float scaleRatio = std::min(scaleRatioX, scaleRatioY);

    menuSprite.setScale(scaleRatio, scaleRatio);
    menuSprite.setPosition((windowSize.x - menuSprite.getGlobalBounds().width) / 2, (windowSize.y - menuSprite.getGlobalBounds().height) / 2);

}
