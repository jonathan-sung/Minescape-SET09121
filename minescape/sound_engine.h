#pragma once
#include <SFML/Audio.hpp>
#include "engine.h"

class SoundEngine {
public:
	SoundEngine() = delete;
	static void Start();
	static void LoadSounds();
	static void UpdateVolumes();
	static sf::Music music_menu;
	static sf::Music music_level1;

	static sf::Sound fx_select;
	static sf::Sound fx_jump;
	static sf::Sound fx_stun;
	static sf::Sound fx_pause;
	static sf::Sound fx_rope_hit;
	static sf::Sound fx_rope_shoot;

	static sf::SoundBuffer buffer_fx_select;
	static sf::SoundBuffer buffer_fx_jump;
	static sf::SoundBuffer buffer_fx_stun;
	static sf::SoundBuffer buffer_fx_pause;
	static sf::SoundBuffer buffer_fx_rope_hit;
	static sf::SoundBuffer buffer_fx_rope_shoot;

protected:

};