#pragma once

#include <SFML/Audio.hpp>

class SoundManager
{
public:
	static void update(const float dt);

	static void set_music(const std::string& name);
	static sf::Music* get_music();

	static void set_music_volume(float volume);
	static float get_music_volume();

	static void stop_music();
	static void pause_music();
	static void play_music();

	static sf::Music::Status get_music_status();

private:
	static sf::Music* m_current_music;
	static float m_music_volume;
};