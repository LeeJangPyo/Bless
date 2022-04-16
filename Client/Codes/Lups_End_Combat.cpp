#include "stdafx.h"
#include "Lups_End_Combat.h"

#include "Management.h"
#include "Collision_Manager.h"

#include "Lups.h"
#include "Sword.h"
#include "Shield.h"

_USING(Client)

CLups_End_Combat::CLups_End_Combat()
{

}

HRESULT CLups_End_Combat::Begin()
{
	m_pSword = dynamic_cast<CSword*>(CManagement::GetInstance()->Get_GameObject(SCENE_STAGE, L"Layer_Sword"));
	if (m_pSword == nullptr)
		return E_FAIL;

	m_pShield = dynamic_cast<CShield*>(CManagement::GetInstance()->Get_GameObject(SCENE_STAGE, L"Layer_Shield"));
	if (m_pShield == nullptr)
		return E_FAIL;

//	m_pSword->Begin_Trail();

	m_pLups->Set_AnimationSet(13);

	return S_OK;
}

_int CLups_End_Combat::Update(const _float & fTimeDelta)
{

	return 0;
}

_int CLups_End_Combat::LastUpdate(const _float & fTimeDelta)
{

	if (m_pLups->Get_IsAnimationSetEnd())
	{
		m_pLups->Change_State(L"Idle");
	}

	return 0;
}

HRESULT CLups_End_Combat::End()
{
//	m_pSword->End_Trail();

	

	m_pShield->Set_Equip(false);
	m_pSword->Set_Equip(false);
	return S_OK;
}

HRESULT CLups_End_Combat::Ready_State(CLups* pLups)
{
	m_pLups = pLups;




	return S_OK;;
}

CLups_End_Combat * CLups_End_Combat::Create(CLups* pLups)
{
	CLups_End_Combat*		pInstance = new CLups_End_Combat();

	if (FAILED(pInstance->Ready_State(pLups)))
	{
		_MSG_BOX(L"CLups_End_Combat Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLups_End_Combat::Free()
{
}


