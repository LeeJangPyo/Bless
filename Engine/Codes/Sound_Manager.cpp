#include "..\Headers\Sound_Manager.h"

_IMPLEMENT_SINGLETON(CSound_Manager)

CSound_Manager::CSound_Manager()
{
}



void CSound_Manager::Ready_Sound()
{
	FMOD_System_Create(&m_pSystem);
	FMOD_System_Init(m_pSystem, MAX_CHANNEL, FMOD_INIT_NORMAL, nullptr);

	LoadSoundFile();
}

void CSound_Manager::Update_Sound()
{
	FMOD_System_Update(m_pSystem);
}

void CSound_Manager::Play_Sound(const _tchar* pSoundKey, CHANNELID eID)
{
	if (m_MapSound.empty())
	{
	//	_TAGMSG_BOX(L"m_MapSound is empty", L"Error!");
		return;
	}

	auto& iter = find_if(m_MapSound.begin(), m_MapSound.end(),[&pSoundKey](auto& MyPair)
	{
		return !lstrcmp(pSoundKey, MyPair.first);
	});

	if (m_MapSound.end() == iter)
	{
		_TAGMSG_BOX(L"Not Found Sound", L"Error!");
		return;
	}

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[eID]);
}

void CSound_Manager::Play_BGM(const _tchar * pSoundKey)
{
	if (m_MapSound.empty())
	{
		//_TAGMSG_BOX(L"m_MapSound is empty", L"Error!");
		return;
	}

	auto& iter = find_if(m_MapSound.begin(), m_MapSound.end(),
		[&pSoundKey](auto& MyPair)
	{
		return !lstrcmp(pSoundKey, MyPair.first);
	});

	if (m_MapSound.end() == iter)
	{
		_TAGMSG_BOX(L"Not Found Sound", L"Error!");
		return;
	}

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[BGM]);
	FMOD_Channel_SetMode(m_pChannelArr[BGM], FMOD_LOOP_NORMAL);
}

void CSound_Manager::Stop_Sound(CHANNELID eID)
{
	FMOD_Channel_Stop(m_pChannelArr[eID]);
}

void CSound_Manager::Stop_All()
{
	for (int i = 0; i < MAX_CHANNEL; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}


void CSound_Manager::LoadSoundFile()
{
	_finddata_t fd;

	// 현재 폴더 내에서 첫번째 파일을 찾음.
	long handle = _findfirst("../Bin/Resources/Sound/*.*", &fd);

	if (0 == handle)
	{
		_TAGMSG_BOX(L"Not Found Sound Path", L"Error!");
		return;
	}

	_int iResult = 0;

	_char szCurPath[MAX_PATH] = "../Bin/Resources/Sound/";
	_char szFullPath[MAX_PATH] = "";

	while (-1 != iResult)
	{
		strcpy_s(szFullPath, szCurPath);
		strcat_s(szFullPath, fd.name);

		FMOD_SOUND* pSound = nullptr;

		FMOD_RESULT eResult = FMOD_System_CreateSound(m_pSystem, szFullPath,
			FMOD_HARDWARE, nullptr, &pSound);

		if (FMOD_OK == eResult)
		{
			int iLength = strlen(fd.name) + 1;
			_tchar* pSoundKey = new TCHAR[iLength];
			ZeroMemory(pSoundKey, sizeof(_tchar) * iLength);

			// 멀티바이트 -> 와이드 바이트로 변환.
			MultiByteToWideChar(CP_ACP, 0, fd.name, iLength, pSoundKey, iLength);

			m_MapSound.insert({ pSoundKey, pSound });
		}

		// 그 다음 파일을 찾음.
		iResult = _findnext(handle, &fd);
	}

	_findclose(handle);

	FMOD_System_Update(m_pSystem);
}

void CSound_Manager::Free()
{
	for (auto& MyPair : m_MapSound)
	{
		delete[] MyPair.first;
		FMOD_Sound_Release(MyPair.second);
	}

	m_MapSound.clear();

	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}
