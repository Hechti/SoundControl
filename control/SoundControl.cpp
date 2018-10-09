#include "SoundControl.hpp"
#include <iostream>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
  switch (ul_reason_for_call)
  {
  case DLL_PROCESS_ATTACH:
    init();
    break;

  case DLL_PROCESS_DETACH:
    release();
    break;

  default:
    break;
  }
  
  return true;
}

IAudioEndpointVolume* endpointVolume = nullptr;

void init()
{
  CoInitialize(nullptr);

  IMMDeviceEnumerator* deviceEnumerator = nullptr;
  auto result = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID *) &deviceEnumerator);
  if (FAILED(result)) {
    std::cerr << "CoCreateInstance failed" << std::endl;
    return;
  }

  IMMDevice* defaultDevice = nullptr;
  result = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
  deviceEnumerator->Release();
  deviceEnumerator = nullptr;
  if (FAILED(result)) {
    std::cerr << "GetDefaultAudioEndpoint failed" << std::endl;
    return;
  }

  result = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, nullptr, (LPVOID *) &endpointVolume);
  defaultDevice->Release();
  defaultDevice = nullptr;
  if (FAILED(result)) {
    std::cerr << "DefaultAudioEndpoint->Activate failed" << std::endl;
    return;
  }
}

void release()
{
  endpointVolume->Release();
  endpointVolume = nullptr;

  CoUninitialize();
}

void setMute(const bool mute)
{
  auto result = endpointVolume->SetMute(mute, nullptr);
  std::cout << "setMute: " << std::boolalpha << mute << "::" << result << std::endl;
}

bool isMuted()
{
  BOOL isMuted = false;
  endpointVolume->GetMute(&isMuted);
  std::cout << "isMuted: " << std::boolalpha << isMuted << std::endl;

  return isMuted;
}

void setVolume(const uint8_t volume)
{
  const float value = static_cast<float>(volume) * 0.01f;
  auto result = endpointVolume->SetMasterVolumeLevelScalar(value, nullptr);
  std::cout << "setVolume: " << +volume << " :: " << result << std::endl;
}

uint8_t getVolume()
{
  float value = 0.0f;
  endpointVolume->GetMasterVolumeLevelScalar(&value);
  std::cout << "getVolume: " << value << std::endl;

  return static_cast<uint8_t>(value * 100.0f);
}

void next()
{
  SendKeyInput(VK_MEDIA_NEXT_TRACK);
  std::cout << "next: " << std::endl;
}

void prev()
{
  SendKeyInput(VK_MEDIA_PREV_TRACK);
  std::cout << "prev: " << std::endl;
}

void playpause()
{
  SendKeyInput(VK_MEDIA_PLAY_PAUSE);
  std::cout << "play: " << std::endl;
}

void stop()
{
  SendKeyInput(VK_MEDIA_STOP);
  std::cout << "stop: " << std::endl;
}

void SendKeyInput(const uint16_t& key)
{
  INPUT input;
  memset(&input, 0, sizeof(INPUT));

  input.type = INPUT_KEYBOARD;
  input.ki.wVk = key;
  SendInput(1, &input, sizeof(INPUT));
}