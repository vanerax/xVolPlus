#pragma once

#include <mmdeviceapi.h>
#include <Endpointvolume.h>
//#include <Audioclient.h>


class CVolumeManager
{
public:
	CVolumeManager(void);
	~CVolumeManager(void);

	HRESULT SetVolumeValue(float fLevel);
	float GetVolumeValue();
	float GetMasterVolumeLevelScalar(void);
	HRESULT SetMasterVolumeLevelScalar(float fLevel);

private:
	IMMDeviceEnumerator* m_pEnumerator;
	IMMDevice* m_pEndpoint;
	IAudioEndpointVolume* m_pEndpointVolume;

	HRESULT GetMMDeviceEnumerator(void);
	HRESULT GetDefaultAudioEndpoint(EDataFlow dataFlow=eRender, ERole role=eConsole);
	HRESULT GetAudioEndpointVolume(void);

};
