#pragma once

#include "Music.h"
#include"DataSet.h"

void Music::PlayMusic() {
  const auto& data = DataSet::getConfig()["music"]["song"];
  if (Pause::_audioID == cocos2d::AudioEngine::INVALID_AUDIO_ID) {
    Pause::_audioID =
        cocos2d::AudioEngine::play2d(data.GetString(), Pause::_loopEnabled);
    AudioEngine::setVolume(Pause::_audioID, Pause::_volume);
  }
  if (Pause::_audioID != cocos2d::AudioEngine::INVALID_AUDIO_ID) {
    cocos2d::AudioEngine::setFinishCallback(
        Pause::_audioID, [&](int id, const std::string& filePath) {
          Pause::_audioID = cocos2d::AudioEngine::INVALID_AUDIO_ID;
        });
  }
}
