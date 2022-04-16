#include "stdafx.h"
#include "Sire_End_Combat.h"

#include "Management.h"
#include "Collision_Manager.h"
#include "Player_Manager.h"

#include "Sire.h"
#include "Staff.h"

_USING(Client)

CSire_End_Combat::CSire_End_Combat()
{

}

HRESULT CSire_End_Combat::Begin()
{
	m_pSire->Set_AnimationSet(8);


	return S_OK;
}

_int CSire_End_Combat::Update(const _float & fTimeDelta)
{

	return 0;
}

_int CSire_End_Combat::LastUpdate(const _float & fTimeDelta)
{

	if (m_pSire->Get_IsAnimationSetEnd())
	{
		m_pSire->Set_ControlType(CONTROL_AI);
		m_pSire->Change_State(L"AI");
	}

	return 0;
}

HRESULT CSire_End_Combat::End()
{
	CStaff* pStaff = dynamic_cast<CStaff*>(CManagement::GetInstance()->Get_GameObject(SCENE_STAGE, L"Layer_Staff"));
	if (pStaff == nullptr)
		return E_FAIL;

	pStaff->Set_Equip(false);
	return S_OK;
}

HRESULT CSire_End_Combat::Ready_State(CSire* pSire)
{
	m_pSire = pSire;


	return S_OK;;
}

CSire_End_Combat * CSire_End_Combat::Create(CSire* pSire)
{
	CSire_End_Combat*		pInstance = new CSire_End_Combat();

	if (FAILED(pInstance->Ready_State(pSire)))
	{
		_MSG_BOX(L"CSire_End_Combat Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSire_End_Combat::Free()
{
}


