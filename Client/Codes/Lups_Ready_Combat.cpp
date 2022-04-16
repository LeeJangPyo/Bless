#include "stdafx.h"
#include "Lups_Ready_Combat.h"

#include "Management.h"
#include "Collision_Manager.h"
#include "Sound_Manager.h"

#include "Lups.h"
#include "Sword.h"
#include "Shield.h"

_USING(Client)

CLups_Ready_Combat::CLups_Ready_Combat()
{

}

HRESULT CLups_Ready_Combat::Begin()
{
	m_pLups->Set_AnimationSet(12);

	m_pSword = dynamic_cast<CSword*>(CManagement::GetInstance()->Get_GameObject(SCENE_STAGE, L"Layer_Sword"));
	if (m_pSword == nullptr)
		return E_FAIL;

	m_pShield = dynamic_cast<CShield*>(CManagement::GetInstance()->Get_GameObject(SCENE_STAGE, L"Layer_Shield"));
	if (m_pShield == nullptr)
		return E_FAIL;

	m_pShield->Set_Equip(true);
	m_pSword->Set_Equip(true);
	m_pSword->Begin_Trail();

	CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::LUPS);
	CSound_Manager::GetInstance()->Play_Sound(L"Lups_Ready_Combat.mp3", CSound_Manager::LUPS);

	return S_OK;
}

_int CLups_Ready_Combat::Update(const _float & fTimeDelta)
{

	return 0;
}

_int CLups_Ready_Combat::LastUpdate(const _float & fTimeDelta)
{

	if (m_pLups->Get_IsAnimationSetEnd())
	{
		m_pLups->Change_State(L"Idle");
	}

	return 0;
}

HRESULT CLups_Ready_Combat::End()
{
	m_pSword->End_Trail();
	m_pLups->Set_ControlType(CONTROL_PLAYER);

	return S_OK;
}

HRESULT CLups_Ready_Combat::Ready_State(CLups* pLups)
{
	m_pLups = pLups;




	return S_OK;;
}

CLups_Ready_Combat * CLups_Ready_Combat::Create(CLups* pLups)
{
	CLups_Ready_Combat*		pInstance = new CLups_Ready_Combat();

	if (FAILED(pInstance->Ready_State(pLups)))
	{
		_MSG_BOX(L"CLups_Ready_Combat Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLups_Ready_Combat::Free()
{
}


