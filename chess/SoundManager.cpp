#include "SoundManager.h"

#include "AssetManager.h"

sf::Music* SoundManager::m_currentMusic = nullptr;
bool SoundManager::m_isStopping = false;
float SoundManager::m_musicVolume = 10.0f;

void SoundManager::update(const float dt)
{
}

void SoundManager::setMusic(const std::string & name)
{
	sf::Music* music = AssetManager::get<sf::Music>(name);
	if (music != nullptr) {
		sf::Music::Status status = sf::Music::Stopped;
		if (m_currentMusic != nullptr) {
			status = getMusicStatus();
			stopMusic();
		}

		m_currentMusic = music;
		m_currentMusic->setVolume(m_musicVolume);
		if (status == sf::Music::Playing) {
			playMusic();
		}
	}
}

sf::Music * SoundManager::getMusic()
{
	return m_currentMusic;
}

void SoundManager::setMusicVolume(float volume)
{
	m_musicVolume = volume;
	if (m_currentMusic != nullptr) {
		m_currentMusic->setVolume(m_musicVolume);
	}
}

float SoundManager::getMusicVolume()
{
	return m_musicVolume;
}

void SoundManager::stopMusic()
{
	if (m_currentMusic != nullptr) {
		m_currentMusic->stop();
	}
}

void SoundManager::pauseMusic()
{
	if (m_currentMusic != nullptr) {
		m_currentMusic->pause();
	}
}

void SoundManager::playMusic()
{
	if (m_currentMusic != nullptr) {
		m_currentMusic->play();
	}
}

sf::Music::Status SoundManager::getMusicStatus()
{
	if (m_currentMusic != nullptr) {
		return m_currentMusic->getStatus();
	}
	else {
		return sf::Music::Status::Stopped;
	}
}
