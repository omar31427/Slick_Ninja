#pragma once 
#include "Resources.h"
#include <thread>

//const int PRESENT = 6;


using std::vector;


Resources::Resources()
{
	
	load_texture("Player.png", n_player);
	load_texture("tile.png", n_tile);
	load_texture("Shuriuken.png", n_projectile);
	load_texture("SimpleMonster.png", n_simpleMonster);
	load_texture("Explosion.png", n_explosion);
	load_texture("SmartMonster.png", n_smartMonster);
	load_texture("PoisonExplosion.png", n_posionExplosion);
	load_texture("RotatingHeart.png", n_healthPresent);
	load_texture("Coin.png",n_scorePresent);
	load_texture("Boss.png", n_boss);
	load_texture("Fireball.png", n_fireball);
	load_texture("Minion.png", n_minion);
	load_texture("Menu.jpg", n_menu);
	load_texture("You-won.jpg", n_won);
	load_texture("Next-level.jpg", n_nextLevel);
	load_texture("breakingWall.png", n_breakingWall);
	load_texture("background.jpeg", n_background);
	load_texture("Game-over.jpg", n_gameOver);

	load_font("ChunkFive-Regular.otf", f_menuscreen);

	load_sound("MainSong.wav", main_sound);
	load_sound("Minion.wav", minion_sound);
	load_sound("Explosion.wav", explosion_sound);
	load_sound("Splat.wav", splat_sound);

	std::ifstream file;
	file.open("Levels.txt");
	if (!file.is_open())
	{
		throw std::runtime_error("Couldn't open the levels file.");
	}

	std::string levelName;
	while (std::getline(file, levelName)) {
		std::ifstream curLevel(levelName);
		if (!curLevel.is_open()) {
			throw std::runtime_error("Couldn't load level.");
		}

		std::vector<std::vector<std::string>> level;

		int height, width;
		curLevel >> height;
		curLevel >> width;
		curLevel.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Ignore the remaining characters on the first line

		std::vector<std::string> room;
		for (int i = 0; i < height; i++)
		{
			std::string line;
			std::getline(curLevel, line);
			room.push_back(line);
		}

		m_levels.push_back(room);
	}

}
std::vector<std::vector<std::string>> Resources::readLevel(std::string& name)
{
	std::ifstream file;
	file.open(name);
	if (!file.is_open())
	{
		throw (std::exception("couldn't open the levels file \n"));
		exit(EXIT_FAILURE);
	}
	std::vector<std::vector<std::string>> level;


	while (!file.eof())
	{


		int height, width;
		file >> height;
		file >> width;
		std::vector<std::string> room;
		file.get();
		for (int i = 0; i < height; i++)
		{
			std::string line;
			getline(file, line);
			
			room.push_back(line);
		}

		level.push_back(room);
	}
	return level;
}

void Resources::load_texture(std::string texture_path, obj_types objtype)
{
	if (!m_textures[objtype].loadFromFile(texture_path))
	{
		std::cerr << texture_path << " ";
		throw (std::exception("error loading texture"));
	}
}

void Resources::load_font(std::string font_path, fontnames name)
{
	sf::Font tempfont;
	tempfont.loadFromFile(font_path);
	m_fonts[name] = tempfont;
}

void Resources::load_sound(std::string sound_path, soundnames name)
{
	if (!m_sounds[name].loadFromFile(sound_path))
	{

		throw (std::exception("error loading texture"));
	}
}

int Resources::getTextsNum()
{
	return m_textures.size();
}

sf::SoundBuffer& Resources::get_sound(const soundnames name)
{
	return m_sounds[name];
}

sf::Font& Resources::get_font(const fontnames name)
{
	return m_fonts[name];
}

sf::Texture* Resources::getTexture(const obj_types name)
{
	return &(m_textures[name]);
}


Resources& Resources::instance()
{
	static Resources inst;
	return inst;

}
std::vector<std::string> Resources::get_lvl(const int num)
{
	return m_levels[num];
}
