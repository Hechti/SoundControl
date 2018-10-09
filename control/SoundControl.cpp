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
  std::cout << "setMute: " << std::boolalpha << mute << std::endl;
}

bool isMuted()
{
  std::cout << "isMuted: " << std::endl;
  return false;
}

void setVolume(uint8_t volume)
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
  std::cout << "next: " << std::endl;
}

void prev()
{
  std::cout << "prev: " << std::endl;
}

void play()
{
  std::cout << "play: " << std::endl;
}

void pause()
{
  std::cout << "pause: " << std::endl;
}

void stop()
{
  std::cout << "stop: " << std::endl;
}

