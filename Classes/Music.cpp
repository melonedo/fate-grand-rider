#pragma once

#include "Music.h"

void Music::PlayMusic() {
  if (Pause::_audioID == cocos2d::AudioEngine::INVALID_AUDIO_ID) {
    Pause::_audioID =
        cocos2d::AudioEngine::play2d("loser.mp3", Pause::_loopEnabled);
    AudioEngine::setVolume(Pause::_audioID, Pause::_volume);
  }
  if (Pause::_audioID != cocos2d::AudioEngine::INVALID_AUDIO_ID) {
    cocos2d::AudioEngine::setFinishCallback(
        Pause::_audioID, [&](int id, const std::string& filePath) {
          Pause::_audioID = cocos2d::AudioEngine::INVALID_AUDIO_ID;
        });
  }
}
