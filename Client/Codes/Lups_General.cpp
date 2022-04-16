#include "stdafx.h"
#include "Lups_General.h"

#include "Management.h"
#include "Collision_Manager.h"

#include "Lups.h"
#include "Sword.h"
#include "Shield.h"

_USING(Client)

CLups_General::CLups_General()
{

}

HRESULT CLups_General::Begin()
{
	m_pLups->Set_AnimationSet(40);

	CSword* pSword = dynamic_cast<CSword*>(CManagement::GetInstance()->Get_GameObject(SCENE_STAGE, L"Layer_Sword"));
	if (pSword == nullptr)
		return E_FAIL;
	pSword->Set_Equip(false);

	CShield* pShield = dynamic_cast<CShield*>(CManagement::GetInstance()->Get_GameObject(SCENE_STAGE, L"Layer_Shield"));
	if (pShield == nullptr)
		return E_FAIL;
	pShield->Set_Equip(false);

	return S_OK;
}

_int CLups_General::Update(const _float & fTimeDelta)
{
	m_pLups->Check_KeyInput_Move(fTimeDelta);


	return 0;
}

_int CLups_General::LastUpdate(const _float & fTimeDelta)
{

	return 0;
}

HRESULT CLups_General::End()
{
	return S_OK;
}

HRESULT CLups_General::Ready_State(CLups* pLups)
{
	m_pLups = pLups;


	return S_OK;;
}

CLups_General * CLups_General::Create(CLups* pLups)
{
	CLups_General*		pInstance = new CLups_General();

	if (FAILED(pInstance->Ready_State(pLups)))
	{
		_MSG_BOX(L"CLups_General Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLups_General::Free()
{
}


