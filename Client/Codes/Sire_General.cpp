#include "stdafx.h"
#include "Sire_General.h"

#include "Management.h"
#include "Collision_Manager.h"

#include "Sire.h"
#include "Staff.h"

_USING(Client)

CSire_General::CSire_General()
{

}

HRESULT CSire_General::Begin()
{
	m_pSire->Set_AnimationSet(50);

	CStaff* pStaff = dynamic_cast<CStaff*>(CManagement::GetInstance()->Get_GameObject(SCENE_STAGE, L"Layer_Sword"));
	if (pStaff == nullptr)
		return E_FAIL;
	pStaff->Set_Equip(false);

	return S_OK;
}

_int CSire_General::Update(const _float & fTimeDelta)
{
	m_pSire->Check_KeyInput_Move(fTimeDelta);


	return 0;
}

_int CSire_General::LastUpdate(const _float & fTimeDelta)
{

	return 0;
}

HRESULT CSire_General::End()
{
	return S_OK;
}

HRESULT CSire_General::Ready_State(CSire* pSire)
{
	m_pSire = pSire;


	return S_OK;;
}

CSire_General * CSire_General::Create(CSire* pSire)
{
	CSire_General*		pInstance = new CSire_General();

	if (FAILED(pInstance->Ready_State(pSire)))
	{
		_MSG_BOX(L"CSire_General Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSire_General::Free()
{
}


