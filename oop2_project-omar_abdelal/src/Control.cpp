#pragma once
#include "Control.h"

Control::Control()
{
    
}

void Control::play()
{

    m_window.close();

    const Resources& res = Resources::instance();
    bool start = false;
    
    sf::Sound sound;
    sound.setBuffer(res.instance().get_sound(main_sound));
    sound.stop();
    sound.play();
    sound.setVolume(15);
    

    Menu menu;
    menu.show(start);
    if (start)
    {
        sound.setVolume(10);
        setWindow();
        m_board.restart();
        m_movables.clear();
        m_stables.clear();
        initiateGameObjects();
        initiateBoard();
        m_gameClock.restart();
        for (auto& tiles : m_stables)
        {
            std::cout << "tile pos.x: " << tiles.get()->getSprite().get()->getGlobalBounds().left;
            std::cout << " tile pos.y: " << tiles.get()->getSprite().get()->getGlobalBounds().top << "\n";
        }
        run();
    }
}

void Control::initiateGameObjects()
{

    //you need to flip the i and j because j iterates over a single row and i iterates over a column
    const Resources& res = Resources::instance();
    std::vector<std::string> curLvl = res.instance().get_lvl(m_curLvl);
    int i = 0;

 
    for (auto& lvlRow : curLvl)
    {
        int j = 0;

        for (auto object : lvlRow)
        {
          
            sf::Vector2f newPos = sf::Vector2f(j * TILE_SIZE, i * TILE_SIZE);
            if (object == ' ')
            {
                j++;
                continue;
            }
            auto type = static_cast<obj_types>(object);

            auto movable = Factory<MovableObject>::instance().create(type, newPos);
            if (movable)
            {
                if (object == 'p')
                    m_playerPointer = movable;
                else if (object == 's' || object == 'i') {
                    m_numOfMonsters++;
                }
                else if (object == 'b') {
                    m_bossPointer = movable;
                }
                if (object == 'i')
                {
                    m_smartMonsters.push_back(movable);
                    
                }
                movable.get()->setType(type);
                m_movables.push_back(std::move(movable));
                
                j++;
           
                continue;
            }
          
            auto unmovable = Factory<StableObject>::instance().create(type, newPos);
            if (unmovable)
            {
                unmovable.get()->setType(type);
                
                m_stables.push_back(std::move(unmovable));
                j++;
                continue;
            }
            j++;
        }
        i++;
    }

}

void Control::initiateBoard()
{
    std::vector<std::shared_ptr<sf::Sprite>> movableSprites;
    std::vector<std::shared_ptr<sf::Sprite>> stableSprites;

    for (auto& movable : m_movables)
        movableSprites.push_back(movable.get()->getSprite());

    for (auto& stable : m_stables)
        stableSprites.push_back(stable.get()->getSprite());
    
    m_board.setMovables(movableSprites);
    m_board.setStables(stableSprites);
    m_board.setLvl(m_curLvl);
    //===========================================================
    //this should bne in a seperate function but i didnt have much time
    const Resources& res = Resources::instance();
    float lvlHeight = res.instance().getLvlHeight(m_curLvl);
    m_hud = Hud(sf::FloatRect(0, lvlHeight*TILE_SIZE, WINDOW_WIDTH, WINDOW_HEIGHT - (lvlHeight*TILE_SIZE)));

}

void Control::setWindow()
{
    const Resources& res = Resources::instance();

    m_window.setFramerateLimit(60);
    m_window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "commando");
    sf::Vector2f screenCenter;
    screenCenter.x = m_window.getSize().x / 2.f;
    screenCenter.y = m_window.getSize().y / 2.f;

    // (screenCenter, m_window.getSize());
    m_view.setCenter(screenCenter);
    m_view.setSize(sf::Vector2f(m_window.getSize()));
}

void Control::run()
{
    m_gameTime.restart();
	while (m_window.isOpen())
	{
        //==========================================================
        //moving movables
        if (m_gameTime.getElapsedTime().asSeconds() >= 2) {
            allowMovement();
        }
        else {
            m_gameClock.restart();
        }
        //============================================================
        //checks for collisions between every 2 pairs of objects
        checkCollisions();
        //===========================================================
        // update objects
        updateObjects();
        //============================================================
        //drawing
        drawObject();
        //============================================================

        sf::Event event;
        while (m_window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                m_window.close();
                break;

            case sf::Event::KeyPressed:
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                    m_playerPointer.get()->shoot();
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    int index = m_hud.getIntersectingRect(m_window.mapPixelToCoords
                    (sf::Vector2i(event.mouseButton.x, event.mouseButton.y)));
                    
                    switch (index) {
                    case 1:
                        play();
                        break;
                    defaul:
                        break;
                    }
                }
                break;
            default:
                break;
            }
        }
       
        if (m_bossPointer.get()->m_curMaxHealthPair.first == 0) {
            m_curLvl++;
            const Resources& res = Resources::instance();
            if (res.instance().getNumLvls() == m_curLvl) //player won
                showWinningMenu();
            else
                showNextLevelMenu();
        }
        if (m_playerPointer.get()->m_curMaxHealthPair.first == 0)//player dead
            showLoseScreen();
        
	}
}
bool Control::advanceNextLevel()
{
    if (m_numOfMonsters == 0) {
        //m_curLvl++;
        //const Resources& res = Resources::instance();
       // if (res.instance().getNumLvls() == m_curLvl) //player won
       // {

        showWinningMenu();
            return true;
       // }

    }
    return false;
}
void Control::drawObject()
{
    m_window.clear();
    m_board.draw(m_window);
    m_hud.draw(m_window);
    m_window.display();
}
void Control::allowMovement()
{
    
    const auto deltaTime = m_gameClock.restart();

    for (auto& movable : m_movables)
    {
       
        movable.get()->move(deltaTime);
    }

    m_playerPointer.get()->detattachTiles(m_isPlayerFalling);

    if (m_numOfMonsters == 0)
        m_bossPointer.get()->activateBoss();
}

void Control::checkCollisions()
{
    for_each_pair(m_movables.begin(), m_movables.end(), m_stables.begin(), m_stables.end(), [this](auto& a, auto& b) {
        if (collide(*a, *b))
        {
            processCollision(*a, *b);
            
        }
        });
    
}
//=============================================================================================================
//=============================================================================================================
//=============================================================================================================
//update block
void Control::updateBoard()
{
    std::vector<std::shared_ptr<sf::Sprite>> movableSprites;
    std::vector<std::shared_ptr<sf::Sprite>> stableSprites;
    std::vector<sf::RectangleShape> hpBars;

    for (auto& movable : m_movables)
    {
        movableSprites.push_back(movable.get()->getSprite());
        if (movable.get()->getHpBar() != nullptr)
        {
            hpBars.push_back(movable.get()->getHpBar().get()->getStaticBar());
            hpBars.push_back(movable.get()->getHpBar().get()->getDynamicBar());
        }
    }

    for (auto& stable : m_stables)
        stableSprites.push_back(stable.get()->getSprite());
    
    m_board.setMovables(movableSprites);
    m_board.setStables(stableSprites);
    m_board.setHpBars(hpBars);
}

void Control::updateView()
{
    float playerPosition = m_playerPointer.get()->getSprite()->getPosition().x;

    const Resources& res = Resources::instance();
    // Calculate the minimum and maximum allowed x-axis values for the view center  
    float minX = m_view.getSize().x / 2;
    float maxX = (res.instance().getLvlWidth(m_curLvl) * TILE_SIZE) - (m_view.getSize().x / 2);


     // Adjust the view's center if the player is too close to the edges of the map
    if (playerPosition < minX) {
        playerPosition = minX;

    }
    else if (playerPosition > maxX) {
        playerPosition = maxX;

    }

    float viewY = m_view.getCenter().y;
    m_view.setCenter(playerPosition, viewY);
    m_window.setView(m_view);

    m_hud.updatePosAll(m_view.getCenter().x - (m_view.getSize().x/2));
    m_hud.setScore(m_playerPointer.get()->getScore());
}

void Control::updateStableAnimations() 
{
    const auto deltaTime = m_gameClock.getElapsedTime().asSeconds();

    for (auto& stable : m_stables)
        if(stable.get()->getType() == obj_types::n_healthPresent || stable.get()->getType() == obj_types::n_scorePresent)
            stable.get()->Update(deltaTime);

}
void Control::checkForProjectiles()
{
    sf::Vector2f playerPos = m_playerPointer.get()->getSprite().get()->getPosition();
    sf::Vector2f bossPos = m_bossPointer.get()->getSprite().get()->getPosition();

    if (m_playerPointer.get()->canICreateProjectile())
    {
        sf::Vector2f projectilePosition = m_playerPointer.get()->getSprite().get()->getPosition();
        projectilePosition.x += (float)PROJECTILE_SIZE / 2.f;
        projectilePosition.y += (float)PROJECTILE_SIZE / 2.f;
        auto newProjectile = Factory<MovableObject>::instance().create(n_projectile, projectilePosition);
        newProjectile.get()->setShooter(n_player);
        newProjectile.get()->setType(n_projectile);
        if (newProjectile)
        {
            newProjectile.get()->setDirection(m_playerPointer.get()->releaseShot());
            m_movables.push_back(std::move(newProjectile));
        }
    }
    const Resources& res = Resources::instance();
    if (m_bossPointer.get()->canICreateProjectile())
    {
        obj_types projType = m_bossPointer.get()->getAttType();
        if (projType == obj_types::n_fireball)
        {
            auto newProjectile = Factory<MovableObject>::instance().create(n_projectile,
                createRandomProjectilePosition(m_bossPointer.get()->getSprite().get()->getGlobalBounds()));
            std::cout << "creating projectile11111111111111\n";
            newProjectile.get()->setType(n_projectile);
            newProjectile.get()->setShooter(n_boss);

            if (newProjectile)
                newProjectile.get()->setDirection(bossPos.x - playerPos.x >= 0 ? sf::Vector2f(-1, 0) : sf::Vector2f(1, 0));

            m_movables.push_back(std::move(newProjectile));
        }
        else if (projType == obj_types::n_minion)
        {
            sf::Sound sound;
            sound.setBuffer(res.instance().get_sound(minion_sound));
            sound.play();
            sound.setVolume(15);


            auto newMinion = Factory<MovableObject>::instance().create(n_minion, m_bossPointer.get()->getSprite().get()->getPosition());
            newMinion.get()->setType(n_minion);
            m_movables.push_back(std::move(newMinion));
        }
        
    }
}
sf::Vector2f Control::createRandomProjectilePosition(sf::FloatRect shooterDimensions)
{
    sf::Vector2f newPos;
    newPos.x = shooterDimensions.left;

    int shooterHeight = static_cast<int>(shooterDimensions.height);
    int randomOffset = rand() % shooterHeight;
    newPos.y = shooterDimensions.top + static_cast<float>(randomOffset);

    return newPos;
}
void Control::createPresent(obj_types monsterType, sf::Vector2f monsterPosition)
{
    int rng = rand();
    if (monsterType == obj_types::n_simpleMonster&& rng % 3 == 0)
    {
        auto present = Factory<StableObject>::instance().create(n_healthPresent, monsterPosition);
        present.get()->setType(n_healthPresent);
        m_stables.push_back(present);
        return;
    }
    if (monsterType == obj_types::n_smartMonster /* && rng % 5 == 0*/)
    {
        m_playerPointer.get()->addScore(5);
        m_hud.setScore(m_playerPointer.get()->getScore());
        auto present = Factory<StableObject>::instance().create(n_scorePresent, m_playerPointer.get()->getSprite().get()->getPosition());
        present.get()->setType(n_scorePresent);
        m_stables.push_back(present);
    }
}
template<typename T>
void Control::deleteFromArray(T& objVec)
{
    std::vector<int> itemsToBeDeleted;
    int itemIndex = 0;
    for (auto& object : objVec)
    {
        if (object.get()->m_needsToBeDeleted) {
            
            itemsToBeDeleted.push_back(itemIndex);
            
            if ((object.get()->getType() == obj_types::n_simpleMonster || object.get()->getType() == obj_types::n_smartMonster
                || object.get()->getType() == obj_types::n_boss))
            {
                createPresent(object.get()->getType(), object.get()->getSprite().get()->getPosition());
                --m_numOfMonsters;
            }
            
        }


        itemIndex++;
    }

    for (const auto& index : itemsToBeDeleted)
    {
        if (index >= 0 && index < objVec.size())
            objVec[index].reset();
    }

    objVec.erase(std::remove_if(objVec.begin(), objVec.end(),
        [](const auto& object) {
            return object == nullptr;
        }), objVec.end());
}
void Control::checkForDeletedObjects() 
{
   deleteFromArray(m_movables);
   deleteFromArray(m_stables);

}
void Control::checkPlayerFalling()
{
    for (auto& object : m_stables)
        if (collide(*object.get(), *m_playerPointer.get()))
        {
            m_isPlayerFalling = false;
            return;
        }
    m_isPlayerFalling = true;
}
void Control::checkForSmartMonstersOnTiles()
{
    for (auto monster : m_smartMonsters)
    {
        for (auto& object : m_stables)
        {
            if (collide(*object.get(), *monster.get()))
            {
                monster.get()->startGhosting(true);
                break;
            }
            monster.get()->startGhosting(false);
        }
    }
}
void Control::updateMonsterPlayerPosition() 
{
    sf::Vector2f playerPos = m_playerPointer.get()->getSprite().get()->getPosition();

    for(auto& movable:m_movables)
        movable.get()->setPlayerPosition(playerPos);
}
void Control::updateObjects() 
{

    //=========================================================================================================
    //check if u can make a projectile
    checkForProjectiles();
    //=========================================================================================================

    //=========================================================================================================
    //check which items to delete and delete them
    checkForDeletedObjects();
    //=========================================================================================================

    //=========================================================================================================
    //check if player is connected to an tile, can only be done from control
    checkPlayerFalling();
    //=========================================================================================================

    //=========================================================================================================
    //updates player position for all smart monsters
    updateMonsterPlayerPosition();
    //=========================================================================================================
    
    //=========================================================================================================
    //checking if the each ghost should change color or not
    checkForSmartMonstersOnTiles();
    //=========================================================================================================
    
    //=========================================================================================================
    //this update animations of all stable objects
    updateStableAnimations();
    //=========================================================================================================

    //=========================================================================================================
     //updating board
    updateBoard();
    //=========================================================================================================
    
    //=========================================================================================================
    //update view
    updateView();
    //=========================================================================================================

    

}
//=============================================================================================================
//=============================================================================================================
//=============================================================================================================
bool Control::collide(GameObject& a, GameObject& b)
{
    
    if (a.getSprite().get()->getGlobalBounds().intersects(b.getSprite().get()->getGlobalBounds()))
    {

        return true;
    }
    return false;

}

void Control::showWinningMenu()
{
    m_movables.clear();
    m_stables.clear();
    m_board.restart();
    m_window.close();
    m_window.create(sf::VideoMode(447, 437), "You won!");

    sf::Texture* texture = Resources::instance().instance().getTexture(n_won);
    sf::Sprite menuSprite(*texture);
    sf::Vector2i exitButton(21, 166);
    sf::Vector2i restartButton(24, 288);

    resizing(menuSprite);
   
    while (m_window.isOpen()) {
        sf::Event event;
        while (m_window.pollEvent(event)) {

            if (event.type == sf::Event::Closed) {
                m_window.close();
                return;
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(m_window);
                if (mousePosition.x >= exitButton.x && mousePosition.x <= exitButton.x + 96 &&
                   mousePosition.y >= exitButton.y && mousePosition.y <= exitButton.y + 40) {
                    m_window.close();
                    return;
                }
  
                if (mousePosition.x >= restartButton.x && mousePosition.x <= restartButton.x + 91 &&
                    mousePosition.y >= restartButton.y && mousePosition.y <= restartButton.y + 26) {
                    m_window.close();
                }
            }
        }
        m_window.clear();
        m_window.draw(menuSprite);
        m_window.display();
    }
    m_curLvl = 0;
    setWindow();
    m_gameTime.restart();
    play();
}

void Control::showNextLevelMenu()
{
    m_window.close();
    m_window.create(sf::VideoMode(447, 437), "Well done!");

    sf::Texture* texture = Resources::instance().instance().getTexture(n_nextLevel);
    sf::Sprite menuSprite(*texture);
    sf::Vector2i nextLevel(244, 368);
    sf::Vector2i exit(49, 366);

    resizing(menuSprite);

    while (m_window.isOpen()) {
        sf::Event event;
        while (m_window.pollEvent(event)) {

            if (event.type == sf::Event::Closed) {
                m_window.close();
                return;
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(m_window);
                if (mousePosition.x >= exit.x && mousePosition.x <= exit.x + 80 &&
                    mousePosition.y >= exit.y && mousePosition.y <= exit.y + 34) {
                    //exit
                    m_window.close();
                    return;
                }

                if (mousePosition.x >= nextLevel.x && mousePosition.x <= nextLevel.x + 166 &&
                    mousePosition.y >= nextLevel.y && mousePosition.y <= nextLevel.y + 31) {
                  //restart
                    m_window.close();
                }
            }
        }
        m_window.clear();
        m_window.draw(menuSprite);
        m_window.display();
    }
    setWindow();
    m_gameTime.restart();
    play();
}

void Control::resizing(sf::Sprite & menuSprite)
{
    sf::Vector2u windowSize = m_window.getSize();

    float scaleRatioX = static_cast<float>(windowSize.x) / menuSprite.getTexture()->getSize().x;
    float scaleRatioY = static_cast<float>(windowSize.y) / menuSprite.getTexture()->getSize().y;

    float scaleRatio = std::min(scaleRatioX, scaleRatioY);

    menuSprite.setScale(scaleRatio, scaleRatio);
    menuSprite.setPosition((windowSize.x - menuSprite.getGlobalBounds().width) / 2, (windowSize.y - menuSprite.getGlobalBounds().height) / 2);
}

void Control::showLoseScreen()
{
    m_window.close();
    m_window.create(sf::VideoMode(447, 437), "you are bad at this!");

    sf::Texture* texture = Resources::instance().instance().getTexture(n_gameOver);
    sf::Sprite menuSprite(*texture);
    sf::Vector2i gameOver(273, 365);
    sf::Vector2i exit(49, 366);

    resizing(menuSprite);

    while (m_window.isOpen()) {
        sf::Event event;
        while (m_window.pollEvent(event)) {

            if (event.type == sf::Event::Closed) {
                m_window.close();
                return;
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(m_window);
                if (mousePosition.x >= exit.x && mousePosition.x <= exit.x + 80 &&
                    mousePosition.y >= exit.y && mousePosition.y <= exit.y + 34) {
                    //exit
                    m_window.close();
                    return;
                }
               
                if (mousePosition.x >= gameOver.x && mousePosition.x <= gameOver.x + 128 &&
                    mousePosition.y >= gameOver.y && mousePosition.y <= gameOver.y + 36) {
                      //restart
                      m_window.close();
                }
            
            }
        }
        m_window.clear();
        m_window.draw(menuSprite);
        m_window.display();
    }
    setWindow();
    m_gameTime.restart();
    play();
}
