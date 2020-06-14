#pragma once

#include"Pause.h"

class Music {
 public:
   //播放背景音乐
  void PlayMusic();

 private:
  int _audioID;
  bool _loopEnabled;
  float _volume;
};

