#pragma once
#include "Player.h"
#include "Tile.h"
#include "SimpleMonster.h"
#include "SmartMonster.h"
#include "Boss.h"
#include "Minion.h"
#include "Board.h"
#include "CollisionHandling.h"
#include "Hud.h"
#include "Menu.h"

class Control {
public:
	Control();
    void play();
private:
    //=============================================================================================================
    //initiator function
	void initiateGameObjects();
	void initiateBoard();
    void setWindow();
    //=============================================================================================================
	//this function contains the gameloop
    void run();
    bool advanceNextLevel();
    //=============================================================================================================
    //this is paint but different
    void drawObject();
    //=============================================================================================================
    //tells people to move and sometimes also tells them how
    void allowMovement();
    //=============================================================================================================
	//controlled chaos
    void checkCollisions();
    //=============================================================================================================
    //update block
    void updateBoard();
    void updateView();
    void updateStableAnimations();
    void updateMonsterPlayerPosition();
    void checkForProjectiles();
    void createPresent(obj_types monsterType, sf::Vector2f monsterPosition);
    sf::Vector2f createRandomProjectilePosition(sf::FloatRect shooterDimensions);
    void checkForDeletedObjects();
    void checkPlayerFalling();
    void checkForSmartMonstersOnTiles();
    void updateObjects();
    //=============================================================================================================
    bool collide(GameObject& a, GameObject& b);
    
    //==========================================================================================================
    void showWinningMenu();
    void showNextLevelMenu();
    void resizing(sf::Sprite& menuSprite);
    void showLoseScreen();

    template <typename FwdIt1, typename FwdIt2, typename Fn>
	void for_each_pair(FwdIt1 movableBegin, FwdIt1 movableEnd, FwdIt2 stableBegin, FwdIt2 stableEnd, Fn fn);



	std::vector<std::shared_ptr<MovableObject>> m_movables;
	std::vector<std::shared_ptr<StableObject>> m_stables;
	//const Resources& m_res = Resources::instance();
	int m_curLvl = 0;
	sf::RenderWindow m_window;
	//======================================================
    //drawing tools
    Board m_board;
    Hud m_hud;
    //======================================================
    sf::Clock m_gameClock;
    std::shared_ptr<MovableObject> m_playerPointer;
    std::shared_ptr<MovableObject> m_bossPointer;
    sf::View m_view;
    bool m_isPlayerFalling = false;
    sf::Clock m_gameTime;
    int m_numOfMonsters = 0;
    std::vector<std::shared_ptr<MovableObject>> m_smartMonsters;
    template<typename T>
    void deleteFromArray(T& objVec);
};

template<typename FwdIt1, typename FwdIt2, typename Fn>
inline void Control::for_each_pair(FwdIt1 movableBegin, FwdIt1 movableEnd, FwdIt2 stableBegin, FwdIt2 stableEnd, Fn fn)
{
    for (auto movableIt = movableBegin; movableIt != movableEnd; ++movableIt)
    {
        for (auto stableIt = stableBegin; stableIt != stableEnd; ++stableIt)
        {
            fn(*movableIt, *stableIt);// Check collision between movable and stable objects
            
        }
    }

    for (auto first = movableBegin; first != movableEnd; ++first)
    {
        for (auto second = first + 1; second != movableEnd; ++second)
        {
            fn(*first, *second); // Check collision between movable objects
        }
    }
}
