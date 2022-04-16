#include "stdafx.h"
#include "Sire_Ready_Combat.h"

#include "Management.h"
#include "Collision_Manager.h"
#include "Sound_Manager.h"


#include "Sire.h"
#include "Staff.h"

_USING(Client)

CSire_Ready_Combat::CSire_Ready_Combat()
{

}

HRESULT CSire_Ready_Combat::Begin()
{
	m_pSire->Set_AnimationSet(8);

	CStaff* pStaff = dynamic_cast<CStaff*>(CManagement::GetInstance()->Get_GameObject(SCENE_STAGE, L"Layer_Staff"));
	if (pStaff == nullptr)
		return E_FAIL;
	pStaff->Set_Equip(true);

	CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::SIRE);
	CSound_Manager::GetInstance()->Play_Sound(L"Sire_Ready_Combat.mp3", CSound_Manager::SIRE);
	return S_OK;
}

_int CSire_Ready_Combat::Update(const _float & fTimeDelta)
{

	return 0;
}

_int CSire_Ready_Combat::LastUpdate(const _float & fTimeDelta)
{

	if (m_pSire->Get_IsAnimationSetEnd())
	{
		m_pSire->Change_State(L"Idle");
	}

	return 0;
}

HRESULT CSire_Ready_Combat::End()
{
	m_pSire->Set_ControlType(CONTROL_AI);

	return S_OK;
}

HRESULT CSire_Ready_Combat::Ready_State(CSire* pSire)
{
	m_pSire = pSire;


	return S_OK;;
}

CSire_Ready_Combat * CSire_Ready_Combat::Create(CSire* pSire)
{
	CSire_Ready_Combat*		pInstance = new CSire_Ready_Combat();

	if (FAILED(pInstance->Ready_State(pSire)))
	{
		_MSG_BOX(L"CSire_Ready_Combat Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSire_Ready_Combat::Free()
{
}


