#include "StdAfx.h"
#include "VolumeManager.h"


CVolumeManager::CVolumeManager(void)
: m_pEnumerator(NULL), m_pEndpoint(NULL), m_pEndpointVolume(NULL)
{
	
	//float fMin, fMax, fInc, fLevel, fDB;
	//pEndpointVolume->GetVolumeRange(&fMin, &fMax, &fInc);
	//pEndpointVolume->GetMasterVolumeLevel(&fDB);
}

CVolumeManager::~CVolumeManager(void)
{
	if (m_pEnumerator) {
		m_pEnumerator->Release();
		m_pEnumerator = NULL;
	}

	if (m_pEndpoint) {
		m_pEndpoint->Release();
		m_pEndpoint = NULL;
	}

	if (m_pEndpointVolume) {
		m_pEndpointVolume->Release();
		m_pEndpointVolume = NULL;
	}

}

HRESULT CVolumeManager::GetMMDeviceEnumerator(void)
{
	const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
	const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);

	return CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&m_pEnumerator);
}

HRESULT CVolumeManager::GetDefaultAudioEndpoint(EDataFlow dataFlow, ERole role)
{
	//return m_pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &m_pEndpoint);
	return m_pEnumerator->GetDefaultAudioEndpoint(dataFlow, role, &m_pEndpoint);
}

HRESULT CVolumeManager::GetAudioEndpointVolume(void)
{
	IID IID_IAudioEndpointVolume = __uuidof(IAudioEndpointVolume);
	return m_pEndpoint->Activate(IID_IAudioEndpointVolume, CLSCTX_ALL, NULL, (void **)&m_pEndpointVolume);
}

float CVolumeManager::GetMasterVolumeLevelScalar(void)
{
	float fLevel = 0;

	if (m_pEndpointVolume == NULL) {
		if (m_pEndpoint == NULL) {
			if (m_pEnumerator == NULL) {
				//GetMMDeviceEnumerator
				if ( S_OK != GetMMDeviceEnumerator()) {
					throw 1004;
				}
			}
			//GetDefaultAudioEndpoint
			if (S_OK != GetDefaultAudioEndpoint()) {
				throw 1005;
			}
		}
		//GetAudioEndpointVolume
		if (S_OK != GetAudioEndpointVolume()) {
			throw 1006;
		}
	}

	m_pEndpointVolume->GetMasterVolumeLevelScalar(&fLevel);
	return fLevel;
}

HRESULT CVolumeManager::SetMasterVolumeLevelScalar(float fLevel)
{
	if (m_pEndpointVolume == NULL) {
		if (m_pEndpoint == NULL) {
			if (m_pEnumerator == NULL) {
				//GetMMDeviceEnumerator
				if ( S_OK != GetMMDeviceEnumerator()) {
					throw 1001;
				}
			}
			//GetDefaultAudioEndpoint
			if (S_OK != GetDefaultAudioEndpoint()) {
				throw 1002;
			}
		}
		//GetAudioEndpointVolume
		if (S_OK != GetAudioEndpointVolume()) {
			throw 1003;
		}
	}

	if (fLevel < 0) fLevel = 0;
	if (fLevel > 100) fLevel = 0;

	return m_pEndpointVolume->SetMasterVolumeLevelScalar(fLevel, NULL);
}

HRESULT CVolumeManager::SetVolumeValue(float fLevel) {
	return SetMasterVolumeLevelScalar(fLevel);
}
	
float CVolumeManager::GetVolumeValue() {
	return GetMasterVolumeLevelScalar();
}



	/*
	IID IID_IAudioClient = __uuidof(IAudioClient);
	IAudioClient *pAudioClient;
	hr = pEndpoint->Activate(IID_IAudioClient, CLSCTX_ALL, NULL, (void **)&pAudioClient); //IID_ISimpleAudioVolume, 
	if (S_OK != hr) {
		pEndpoint->Release();
		pEnumerator->Release();
		goto end;
	}

	//get format first
	WAVEFORMATEX *pFormat;
	pAudioClient->GetMixFormat(&pFormat);

	REFERENCE_TIME hnsBufferDuration = 500 * 10000; //100 nanosecond units, 40 * 10000 means 40 ms
	pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, hnsBufferDuration, 0, pFormat, NULL);



	IID IID_ISimpleAudioVolume = __uuidof(ISimpleAudioVolume);
	ISimpleAudioVolume *pAudioVolume;
	hr = pAudioClient->GetService(IID_ISimpleAudioVolume, (void **)&pAudioVolume);
	if ( S_OK != hr) {
		pAudioClient->Release();
		pEndpoint->Release();
		pEnumerator->Release();
		goto end;
	}

	float fLevel = 0.0;
	hr = pAudioVolume->GetMasterVolume(&fLevel);
	if ( S_OK != hr) {
		pAudioVolume->Release();
		pAudioClient->Release();
		pEndpoint->Release();
		pEnumerator->Release();
		goto end;
	}
*/





/*
	IMMDeviceCollection *pDeviceCollection;

	hr = pEnumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &pDeviceCollection);
	if (hr != S_OK) {
		pEnumerator->Release();
		goto end;
	}


	UINT uDeviceCount = 0;
	pDeviceCollection->GetCount(&uDeviceCount);
	for (UINT i=0;i<uDeviceCount;i++) {
		IMMDevice *pDevice;
		hr = pDeviceCollection->Item(i, &pDevice);
		if (hr != S_OK) {
			continue;
		}

		IPropertyStore *pPropertyStore;
		hr = pDevice->OpenPropertyStore(STGM_READ, &pPropertyStore);
		if (hr != S_OK) {
			pDevice->Release();
			continue;
		}

		DWORD dwPropertyCount = 0;
		pPropertyStore->GetCount(&dwPropertyCount);
		for (DWORD j = 0;j<dwPropertyCount;j++) {
			PROPERTYKEY propertyKey;
			PROPVARIANT propVariant;
			pPropertyStore->GetAt(j, &propertyKey);
			pPropertyStore->GetValue(propertyKey, &propVariant);

			int a = 1;
			assert(TRUE);
		}

		pDevice->Release();
	}
*/	
