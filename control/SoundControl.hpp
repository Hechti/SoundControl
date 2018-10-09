#ifndef SOUND_CONTROL_H
#define SOUND_CONTROL_H

#include <cstdint>
#include <Windows.h>

#ifdef EXPORT_FUNC
#define EXPORT_API __declspec(dllexport)
#else 
#define EXPORT_API __declspec(dllimport)
#endif

BOOL APIENTRY DllMain(HANDLE hModule,	DWORD ul_reason_for_call, LPVOID lpReserved);

void init();
void release();
void SendKeyInput(const uint16_t& key);

extern "C" 
{
  void EXPORT_API setMute(const bool mute);
  bool EXPORT_API isMuted();

  void EXPORT_API setVolume(const uint8_t volume);
  uint8_t EXPORT_API getVolume();

  void EXPORT_API next();
  void EXPORT_API prev();
  void EXPORT_API playpause();
  void EXPORT_API stop();
}

#endif