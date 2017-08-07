#pragma once

#include <SFML/Audio.hpp>

class SoundManager
{
public:
	static void update(const float dt);

	static void setMusic(const std::string& name);
	static sf::Music* getMusic();

	static void setMusicVolume(float volume);
	static float getMusicVolume();

	static void stopMusic();
	static void pauseMusic();
	static void playMusic();

	static sf::Music::Status getMusicStatus();

private:
	static sf::Music* m_currentMusic;
	static bool m_isStopping;
	static float m_musicVolume;
};