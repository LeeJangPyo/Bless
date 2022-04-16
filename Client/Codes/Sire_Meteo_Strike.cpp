#include "stdafx.h"
#include "Sire_Meteo_Strike.h"

#include "Management.h"
#include "Collision_Manager.h"
#include "Sound_Manager.h"

#include "Sire.h"
#include "Staff.h"

#include "EffectGroup.h"
#include "UI_Frame.h"

#include "Meteo_Strike.h"
_USING(Client)

CSire_Meteo_Strike::CSire_Meteo_Strike()
{

}

HRESULT CSire_Meteo_Strike::Begin()
{
	m_pSire->Set_AnimationSet(23);

	CMeteo_Strike*	pMeteoStrike;
	//[L"GameObject_Meteo_Strike"] = 0xc9af4720 {m_pEffect_Meteo_Strike=0x00000000 <NULL> m_pEffect_Meteo_Explosion=0x00000000 <NULL> m_bActivate=...}
	CManagement::GetInstance()->Add_Layer(SCENE_STAGE, L"Layer_Meteo_Strike", L"GameObject_Meteo_Strike", (CGameObject**)&pMeteoStrike);
		
	CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::SIRE);
	CSound_Manager::GetInstance()->Play_Sound(L"Sire_MeteoStrike.wav", CSound_Manager::SIRE);

	return S_OK;
}

_int CSire_Meteo_Strike::Update(const _float & fTimeDelta)
{

	return 0;
}

_int CSire_Meteo_Strike::LastUpdate(const _float & fTimeDelta)
{

	if (m_pSire->Get_IsAnimationSetEnd())
	{
		m_pSire->Change_State(L"Idle");
	}

	return 0;
}

HRESULT CSire_Meteo_Strike::End()
{
	// m_pEffect_Meteo_Strike->Set_Activate(false);

	return S_OK;
}

HRESULT CSire_Meteo_Strike::Ready_State(CSire* pSire)
{
	m_pSire = pSire;

	return S_OK;
}

CSire_Meteo_Strike * CSire_Meteo_Strike::Create(CSire* pSire)
{
	CSire_Meteo_Strike*		pInstance = new CSire_Meteo_Strike();

	if (FAILED(pInstance->Ready_State(pSire)))
	{
		_MSG_BOX(L"CSire_Meteo_Strike Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSire_Meteo_Strike::Free()
{
}


