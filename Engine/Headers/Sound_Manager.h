
#ifndef Sound_Manager_h__
#define Sound_Manager_h__

#include "Base.h"

_BEGIN(Engine)

class _ENGINE_DLL CSound_Manager final : public CBase
{
	_DECLARE_SINGLETON(CSound_Manager)

public:
	enum CHANNELID { BGM, LUPS,SIRE, MONSTER,BOSS, EFFECT, UI, MAX_CHANNEL };

private:
	CSound_Manager();
	~CSound_Manager() = default;

public:
	void Ready_Sound();
	void Update_Sound();
	void Play_Sound(const _tchar* pSoundKey, CHANNELID eID);
	void Play_BGM(const _tchar* pSoundKey);
	void Stop_Sound(CHANNELID eID);
	void Stop_All();

private:
	void LoadSoundFile();

private:
	map<const _tchar*, FMOD_SOUND*>	m_MapSound;
	FMOD_CHANNEL*					m_pChannelArr[MAX_CHANNEL];
	FMOD_SYSTEM*					m_pSystem;

protected:
	virtual void Free() override;
};

_END

#endif // Sound_Manager_h__