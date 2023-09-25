#include "CollisionHandling.h"

#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <typeinfo>
#include <typeindex>

#include "Tile.h"
#include "Player.h"
#include "Projectile.h"
#include "SimpleMonster.h"
#include "SmartMonster.h"
#include "LifePresent.h"
#include "Minion.h"
#include "Boss.h"
namespace // anonymous namespace — the standard way to make function "static"
{

// primary collision-processing functions
void playerTile(GameObject& player,
                  GameObject& tile)
{
    

    Player& antagonist = dynamic_cast<Player&>(player);
    Tile& block = dynamic_cast<Tile&>(tile);

    sf::Vector2f playerPos = antagonist.getSprite()->getPosition();
    sf::Vector2f tilePos = block.getSprite()->getPosition();
    float verticalDifference = std::abs(playerPos.y - tilePos.y);
    float horizontalDifference = std::abs(playerPos.x - tilePos.x);
    std::unordered_map<float, sf::Vector2f> directionMargin;
    sf::Vector2f goUp = sf::Vector2f(0, -1);
    sf::Vector2f goDown = sf::Vector2f(0, 1);
    sf::Vector2f goLeft = sf::Vector2f(-1, 0);
    sf::Vector2f goRight = sf::Vector2f(1, 0);

   directionMargin.emplace(playerPos.y + (float)PLAYER_SIZE - tilePos.y, goDown);
   directionMargin.emplace(tilePos.y + (float)TILE_SIZE - playerPos.y, goUp);
   directionMargin.emplace(playerPos.x + (float)PLAYER_SIZE - tilePos.x, goRight);
   directionMargin.emplace(tilePos.x + (float)TILE_SIZE - playerPos.x, goLeft);
   
   auto smallestMargin = directionMargin.begin();
   for (auto it = directionMargin.begin(); it != directionMargin.end(); ++it) {
       if (it->first < smallestMargin->first) {
           smallestMargin = it;
       }
   }

  
      
  antagonist.addForbiddenMove(smallestMargin->second, smallestMargin->first, horizontalDifference, verticalDifference);

}

//...

// secondary collision-processing functions that just
// implement symmetry: swap the parameters and call a
// primary function
void tilePlayer(GameObject& tile,
                  GameObject& player)
{
    playerTile(player, tile);
}

void projectileTile(GameObject& projectile, GameObject& tile)
{
    Projectile& proj = dynamic_cast<Projectile&>(projectile);
    
    proj.m_needsToBeDeleted = true;
}

void tileProjectile(GameObject& tile, GameObject& projectile)
{
    projectileTile(projectile, tile);
}

void playerProjectile(GameObject& player, GameObject& projectile)
{
    Projectile& proj = dynamic_cast<Projectile&>(projectile);
    Player& antagonist = dynamic_cast<Player&>(player);

    if (proj.getShooter() == antagonist.getType())
        return;
    proj.m_needsToBeDeleted = true;
    antagonist.setHealth(2*BASE_MONSTER_DAMAGE);
    
}

void projectilePlayer(GameObject& projectile, GameObject& player)
{
    playerProjectile(player,projectile);
}
void playerSimpleMonster(GameObject& player, GameObject& sMonster)
{
    if (typeid(player) != typeid(Player))
        return;
    Player& antagonist = dynamic_cast<Player&>(player);
    SimpleMonster& monster = dynamic_cast<SimpleMonster&>(sMonster);

    if (monster.m_curMaxHealthPair.first == 0)
        return;
    antagonist.setHealth( BASE_MONSTER_DAMAGE);
    monster.setTimeToExplode();
}
void simpleMonsterPlayer(GameObject& sMonster, GameObject player)
{
    ;// PlayerSimpleMonster(player, sMonster);
}
void simpleMonsterProjectile(GameObject& sMonster, GameObject& projectile)
{

    Projectile& proj = dynamic_cast<Projectile&>(projectile);
    SimpleMonster& monster = dynamic_cast<SimpleMonster&>(sMonster);
  
    if (monster.m_curMaxHealthPair.first == 0 || proj.getShooter() == obj_types::n_boss)
        return;
    proj.m_needsToBeDeleted = true;
    monster.setHealth(BASE_PLAYER_DAMAGE);

}
void projectileSimpleMonster(GameObject& projectile, GameObject& sMonster)
{
    
     simpleMonsterProjectile(sMonster, projectile);
}
void simpleMonsterTile(GameObject& sMonster, GameObject& tile)
{
    SimpleMonster& monster = dynamic_cast<SimpleMonster&>(sMonster);
    Tile& block = dynamic_cast<Tile&>(tile);
    sf::Vector2f monsterPos = monster.getSprite().get()->getPosition();
    sf::Vector2f blockPos = block.getSprite().get()->getPosition();
    if (blockPos.y - monsterPos.y < SIMPLE_MONSTER_SIZE)
    {
        if (monsterPos.x > blockPos.x)
            monster.changeDirection(true);
        else
            monster.changeDirection(false);
    }
}
void tileSimpleMonster(GameObject& tile, GameObject& sMonster)
{
     simpleMonsterTile(sMonster, tile);
}
void smartMonsterTile(GameObject& smartMonster, GameObject& tile) {
    SmartMonster& monster = dynamic_cast<SmartMonster&>(smartMonster);
    
    monster.startGhosting(true);

}

void smartMonsterPlayer(GameObject& smartMonster, GameObject& player) {
    if (typeid(player) != typeid(Player))
        return;
    Player& antagonist = dynamic_cast<Player&>(player);
    SmartMonster& monster = dynamic_cast<SmartMonster&>(smartMonster);
    if (monster.m_curMaxHealthPair.first == 0)
        return;
    antagonist.setHealth(3 * BASE_MONSTER_DAMAGE);
    monster.setTimeToExplode();
}

void playerSmartMonster(GameObject& player, GameObject& smartMonster)
{
    smartMonsterPlayer(smartMonster, player);
}
void smartMonsterProjectile(GameObject& smartMonster, GameObject& projectile) {
    Projectile& proj = dynamic_cast<Projectile&>(projectile);
    SmartMonster& monster = dynamic_cast<SmartMonster&>(smartMonster);

    if (monster.m_curMaxHealthPair.first == 0 || proj.getShooter() == obj_types::n_boss)
        return;
    proj.m_needsToBeDeleted = true;
    monster.setHealth(BASE_PLAYER_DAMAGE);

}

void projectileSmartMonster(GameObject& projectile, GameObject& smartMonster)
{
    smartMonsterProjectile(smartMonster, projectile);
}
void playerLifePresent(GameObject& player, GameObject& present)
{
    Player& antagonist = dynamic_cast<Player&>(player);
    LifePresent& lPresent = dynamic_cast<LifePresent&>(present);

    antagonist.setHealth(HEALTH_BONUS);
    lPresent.m_needsToBeDeleted = true;
}
void minionPlayer(GameObject& minion, GameObject& player)
{
    Player& antagonist = dynamic_cast<Player&>(player);
    Minion& min = dynamic_cast<Minion&>(minion);

    antagonist.setHealth(4 * BASE_MONSTER_DAMAGE);
    min.setTimeToExplode();
}
void bossProjectile(GameObject& boss, GameObject& projectile)
{
    Projectile& proj = dynamic_cast<Projectile&>(projectile);
    Boss& bos = dynamic_cast<Boss&>(boss);

    if (proj.getShooter() == bos.getType() || !bos.m_isAlive)
        return;
    proj.m_needsToBeDeleted = true;
    bos.setHealth(BASE_MONSTER_DAMAGE);
}
void projectileBoss(GameObject& projectile, GameObject& boss)
{
    bossProjectile(projectile, boss);
}
void playerMinion(GameObject& player, GameObject& minion)
{
    minionPlayer(minion, player);
}
void minionTile(GameObject& minion, GameObject& tile)
{
   // Minion& min = dynamic_cast<Minion&>(minion);
    //min.setTimeToExplode();;
    ;
}
void tileMinion(GameObject& tile, GameObject& minion)
{
    minionTile(minion, tile);
}
using HitFunctionPtr = std::function<void(GameObject&, GameObject&)>;
// typedef void (*HitFunctionPtr)(GameObject&, GameObject&);
using Key = std::pair<std::type_index, std::type_index>;
// std::unordered_map is better, but it requires defining good hash function for pair
using HitMap = std::map<Key, HitFunctionPtr>;

HitMap initializeCollisionMap()
{
    HitMap phm;
    phm[Key(typeid(Player), typeid(Tile))] = &playerTile;
    phm[Key(typeid(Tile), typeid(Player))] = &tilePlayer;
    phm[Key(typeid(Tile), typeid(Projectile))] = &tileProjectile;
    phm[Key(typeid(Projectile), typeid(Tile))] = &projectileTile;
    phm[Key(typeid(Projectile), typeid(Player))] = &projectilePlayer;
    phm[Key(typeid(Player), typeid(Projectile))] = &playerProjectile;
    phm[Key(typeid(Player), typeid(SimpleMonster))] = &playerSimpleMonster;
    phm[Key(typeid(SimpleMonster), typeid(Player))] = &simpleMonsterPlayer;
    phm[Key(typeid(SimpleMonster), typeid(Projectile))] = &simpleMonsterProjectile;
    phm[Key(typeid(Projectile), typeid(SimpleMonster))] = &projectileSimpleMonster;
    phm[Key(typeid(SimpleMonster), typeid(Tile))] = &simpleMonsterTile;
    phm[Key(typeid(Tile), typeid(SimpleMonster))] = &tileSimpleMonster;
    phm[Key(typeid(SmartMonster), typeid(Tile))] = &smartMonsterTile;
    phm[Key(typeid(SmartMonster), typeid(Player))] = &smartMonsterPlayer;
    phm[Key(typeid(SmartMonster), typeid(Projectile))] = &smartMonsterProjectile;
    phm[Key(typeid(Player), typeid(LifePresent))] = &playerLifePresent;
    phm[Key(typeid(Player), typeid(SmartMonster))] = &playerSmartMonster;
    phm[Key(typeid(Projectile), typeid(SmartMonster))] = &projectileSmartMonster;
    phm[Key(typeid(Player), typeid(Minion))] = &playerMinion;
    phm[Key(typeid(Minion), typeid(Player))] = &minionPlayer;
    phm[Key(typeid(Boss), typeid(Projectile))] = &bossProjectile;
    phm[Key(typeid(Projectile), typeid(Boss))] = &projectileBoss;
    phm[Key(typeid(Minion), typeid(Tile))] = &minionTile;
    phm[Key(typeid(Tile), typeid(Minion))] = &tileMinion;
    return phm;
}

HitFunctionPtr lookup(const std::type_index& class1, const std::type_index& class2)
{
    static HitMap collisionMap = initializeCollisionMap();
    auto mapEntry = collisionMap.find(std::make_pair(class1, class2));
    if (mapEntry == collisionMap.end())
    {
        return nullptr;
    }
    return mapEntry->second;
}

} // end namespace

void processCollision(GameObject& object1, GameObject& object2)
{
    if (typeid(object1) == typeid(object2))
        return;
    auto phf = lookup(typeid(object1), typeid(object2));
    if (phf)
    {

        //throw UnknownCollision(object1, object2);
        //return;
        phf(object1, object2);
    }
    
}
