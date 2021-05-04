#include "sound_engine.h"

sf::Music SoundEngine::music_menu;
sf::Music SoundEngine::music_level1;

sf::Sound SoundEngine::fx_select;
sf::Sound SoundEngine::fx_jump;
sf::Sound SoundEngine::fx_stun;
sf::Sound SoundEngine::fx_pause;
sf::Sound SoundEngine::fx_rope_hit;
sf::Sound SoundEngine::fx_rope_shoot;

sf::SoundBuffer SoundEngine::buffer_fx_select;
sf::SoundBuffer SoundEngine::buffer_fx_jump;
sf::SoundBuffer SoundEngine::buffer_fx_stun;
sf::SoundBuffer SoundEngine::buffer_fx_pause;
sf::SoundBuffer SoundEngine::buffer_fx_rope_hit;
sf::SoundBuffer SoundEngine::buffer_fx_rope_shoot;

void SoundEngine::Start() {
	std::cout << "SOUNDS LOADED" << std::endl;
	LoadSounds();
	UpdateVolumes();
}

void SoundEngine::LoadSounds()
{
	if (music_menu.openFromFile("res/sounds/music/minescape_menu_theme.ogg")) {
		std::cout << "MENU MUSIC LOADED SUCCESSFULLY!" << std::endl;
	}
	music_level1.openFromFile("res/sounds/music/minescape_main_theme.ogg");
	
	buffer_fx_select.loadFromFile("res/sounds/fx/blip_select.wav");
	buffer_fx_jump.loadFromFile("res/sounds/fx/jump.wav");
	buffer_fx_stun.loadFromFile("res/sounds/fx/new_rock_hit.wav");
	buffer_fx_pause.loadFromFile("res/sounds/fx/pause.wav");
	buffer_fx_rope_hit.loadFromFile("res/sounds/fx/rope_hit.wav");
	buffer_fx_rope_shoot.loadFromFile("res/sounds/fx/rope_shoot.wav");
	
	fx_select.setBuffer(buffer_fx_select);
	fx_jump.setBuffer(buffer_fx_jump);
	fx_stun.setBuffer(buffer_fx_stun);
	fx_pause.setBuffer(buffer_fx_pause);
	fx_rope_hit.setBuffer(buffer_fx_rope_hit);
	fx_rope_shoot.setBuffer(buffer_fx_rope_shoot);

	music_menu.setLoop(true);
	music_level1.setLoop(true);
}

void SoundEngine::UpdateVolumes()
{
	music_menu.setVolume(Engine::getMusicVolume());
	music_level1.setVolume(Engine::getMusicVolume());

	fx_select.setVolume(Engine::getMusicVolume());
	fx_jump.setVolume(Engine::getMusicVolume());
	fx_stun.setVolume(Engine::getMusicVolume());
	fx_pause.setVolume(Engine::getMusicVolume());
	fx_rope_hit.setVolume(Engine::getMusicVolume());
	fx_rope_shoot.setVolume(Engine::getMusicVolume());
}
