#include "stdafx.h"
#include "Sire_Ready_Meteo.h"

#include "Management.h"
#include "Collision_Manager.h"
#include "Sound_Manager.h"

#include "Sire.h"
#include "Staff.h"

//UI
#include "UI_Frame.h"

//Effect
#include "EffectGroup.h"

_USING(Client)

CSire_Ready_Meteo::CSire_Ready_Meteo()
{

}

HRESULT CSire_Ready_Meteo::Begin()
{
	if (!m_pSire->Use_Mp(750))
		return S_OK;

	m_fTimeAcc = 0.f;

	m_pSire->Set_AnimationSet(22);

	// CastMeteo Parent Matrix
	m_pEffect_FireOrb->Set_ParentMatrix(L"Layer_Sire", "Bip01-L-Finger11");
	 
	m_pEffect_FireOrb->Set_Activate(true);

	_vec3 vPosition = *m_pSire->Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION);
	vPosition.y += 0.2f;
	m_pEffect_MagicZone->Set_Position(&vPosition);
	m_pEffect_MagicZone->Set_Activate(true);

	CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::SIRE);
	CSound_Manager::GetInstance()->Play_Sound(L"Sire_Ready_Spell.mp3", CSound_Manager::SIRE);

	return S_OK;
}

_int CSire_Ready_Meteo::Update(const _float & fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;


	return 0;
}

_int CSire_Ready_Meteo::LastUpdate(const _float & fTimeDelta)
{
	if (m_fTimeAcc > m_fCastingTime)
		m_pSire->Change_State(L"Meteo_Strike");

	if (m_pSire->Get_IsAnimationSetEnd())
	{
		m_pSire->Set_AnimationSet(22);
	}

	return 0;
}

HRESULT CSire_Ready_Meteo::End()
{

	m_pEffect_FireOrb->Set_Activate(false);
	m_pEffect_MagicZone->Set_Activate(false);

	return S_OK;
}

HRESULT CSire_Ready_Meteo::Ready_State(CSire* pSire)
{
	m_pSire = pSire;

	m_fCastingTime = 7.5f;


	m_pEffect_FireOrb = dynamic_cast<CEffectGroup*>(CManagement::GetInstance()->Get_GameObject(SCENE_STAGE, L"Layer_Effect_Sire",0));
	if (m_pEffect_FireOrb == nullptr)
		return E_FAIL;

	m_pEffect_MagicZone = dynamic_cast<CEffectGroup*>(CManagement::GetInstance()->Get_GameObject(SCENE_STAGE, L"Layer_Effect_Sire", 5));
	if (m_pEffect_MagicZone == nullptr)
		return E_FAIL;

	return S_OK;;
}

CSire_Ready_Meteo * CSire_Ready_Meteo::Create(CSire* pSire)
{
	CSire_Ready_Meteo*		pInstance = new CSire_Ready_Meteo();

	if (FAILED(pInstance->Ready_State(pSire)))
	{
		_MSG_BOX(L"CSire_Ready_Meteo Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSire_Ready_Meteo::Free()
{
}


