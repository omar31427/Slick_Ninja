#pragma once
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <typeinfo>
#include <fstream>
#include <unordered_map>


//=======================================================
//=======================================================
//object sizes
constexpr auto PROJECTILE_SIZE = 20.f;
constexpr auto TILE_SIZE = 20.f;
constexpr auto PLAYER_SIZE  = 40.f;
constexpr auto SIMPLE_MONSTER_SIZE = 40.f;
constexpr auto SMART_MONSTER_SIZE = 50.f;
constexpr auto BOSS_SIZE = 160.f;
//=======================================================
//=======================================================



//=======================================================
//=======================================================
//window settings
constexpr auto WINDOW_WIDTH = 1100.f;
constexpr auto WINDOW_HEIGHT = 500.f;
//=======================================================
//=======================================================



//=======================================================
//=======================================================
//object settings and details
// ======================BOSS============================
constexpr auto BOSS_MAX_HEALTH = 250;
//=======================SMART MONSTER===================
constexpr auto SMART_MONSTER_SPEED = 100.f;
constexpr auto SMART_MONSTER_MAX_HEALTH = 100.f;
//========================PLAYER=========================
constexpr auto PLAYER_SPEED = 150.f;
constexpr auto PLAYER_MAX_HEALTH = 100.f;
constexpr auto BASE_PLAYER_DAMAGE = -50.f;
//=======================================================
constexpr auto SIMPLE_MONSTER_MAX_HEALTH = 30.f;
constexpr auto SIMPLE_MONSTER_SPEED = 70.f;;
//=====================HP BAR============================
constexpr auto HP_BAR_HEIGHT = 5.f;
//===================GENERAL MONSTER=====================
constexpr auto BASE_MONSTER_DAMAGE = -15.f;
//=======================================================
//=======================================================

//=======================================================
//=======================================================
//game variables
constexpr auto HEALTH_BONUS = 100.f;
//=======================================================
//=======================================================

using std::ifstream;
using std::vector;

//all the object types, used for getting texture
enum obj_types
{
	n_player = 'p',
	n_tile = '#',
	n_simpleMonster = 's',
	n_smartMonster = 'i',
	n_projectile = '*',
	n_explosion = 'e',
	n_posionExplosion = '%',
	n_healthPresent = 'k',
	n_scorePresent = 'o',
	n_boss = 'b',
	n_fireball = 'f',
	n_minion = 'm',
	n_menu ,
	n_won = 'v',
	n_nextLevel = 'n',
	n_breakingWall = 'a',
	n_background = 'g',
	n_gameOver = 'z'
};
enum soundnames
{
	main_sound,
	minion_sound,
	explosion_sound,
	splat_sound,
};
enum fontnames
{
	f_menuscreen
};
class Resources
{
public:
	static Resources& instance();
	std::vector<std::string> get_lvl(const int num);

	size_t getLvlWidth(int cur_lvl) {
		return m_levels[cur_lvl][0].size();
	};
	size_t getLvlHeight(int cur_lvl) {
		return m_levels[cur_lvl].size();
	}
	sf::Font& get_font(const fontnames name);
	sf::Texture* getTexture(const obj_types);
	int getTextsNum();
	size_t getNumLvls() { return m_levels.size(); };
	sf::SoundBuffer& get_sound(const soundnames name);
private:
	Resources();
	std::vector<std::vector<std::string>> readLevel(std::string& name);
	void load_texture(std::string texture_path, obj_types obtype);
	void load_font(std::string font_path, fontnames name);
	void load_sound(std::string sound_path, soundnames name);
	Resources(const Resources&) = default;
	Resources& operator=(const Resources&) = default;
	std::unordered_map <obj_types, sf::Texture> m_textures;
	std::unordered_map <fontnames, sf::Font> m_fonts;
	std::unordered_map <soundnames, sf::SoundBuffer> m_sounds;
	std::vector<std::vector<std::string>> m_levels;
	sf::Texture temp;


};