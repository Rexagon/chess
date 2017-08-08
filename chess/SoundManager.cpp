#include "SoundManager.h"

#include "AssetManager.h"

sf::Music* SoundManager::m_current_music = nullptr;
float SoundManager::m_music_volume = 10.0f;

void SoundManager::update(const float dt)
{
}

void SoundManager::set_music(const std::string & name)
{
	sf::Music* music = AssetManager::get<sf::Music>(name);
	if (music != nullptr) {
		sf::Music::Status status = sf::Music::Stopped;
		if (m_current_music != nullptr) {
			status = get_music_status();
			stop_music();
		}

		m_current_music = music;
		m_current_music->setVolume(m_music_volume);
		if (status == sf::Music::Playing) {
			play_music();
		}
	}
}

sf::Music * SoundManager::get_music()
{
	return m_current_music;
}

void SoundManager::set_music_volume(float volume)
{
	m_music_volume = volume;
	if (m_current_music != nullptr) {
		m_current_music->setVolume(m_music_volume);
	}
}

float SoundManager::get_music_volume()
{
	return m_music_volume;
}

void SoundManager::stop_music()
{
	if (m_current_music != nullptr) {
		m_current_music->stop();
	}
}

void SoundManager::pause_music()
{
	if (m_current_music != nullptr) {
		m_current_music->pause();
	}
}

void SoundManager::play_music()
{
	if (m_current_music != nullptr) {
		m_current_music->play();
	}
}

sf::Music::Status SoundManager::get_music_status()
{
	if (m_current_music != nullptr) {
		return m_current_music->getStatus();
	}
	else {
		return sf::Music::Status::Stopped;
	}
}
