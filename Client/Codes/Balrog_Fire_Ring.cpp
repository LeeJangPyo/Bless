#include "stdafx.h"
#include "Balrog_Fire_Ring.h"

#include "Management.h"
#include "Collision_Manager.h"

#include "Balrog.h"

_USING(Client)

CBalrog_Fire_Ring::CBalrog_Fire_Ring()
{

}

HRESULT CBalrog_Fire_Ring::Begin()
{
	m_pBalrog->Set_AnimationSet(13);



	return S_OK;
}

_int CBalrog_Fire_Ring::Update(const _float & fTimeDelta)
{


	return 0;
}

_int CBalrog_Fire_Ring::LastUpdate(const _float & fTimeDelta)
{
	if (m_pBalrog->Get_IsAnimationSetEnd())
	{
		m_pBalrog->Change_State(L"Idle");
	}
	return 0;
}

HRESULT CBalrog_Fire_Ring::End()
{
	return S_OK;
}

HRESULT CBalrog_Fire_Ring::Ready_State(CBalrog * pBalrog)
{
	m_pBalrog = pBalrog;


	//Get Player's Transform;
	m_pPlayerTransform = dynamic_cast<CTransform*>
		(CManagement::GetInstance()->Get_Component(SCENE_STAGE, L"Layer_Lups", L"Com_Transform"));
	if (m_pPlayerTransform == nullptr)
		return E_FAIL;

	m_pTransform = m_pBalrog->Get_Transform();
	return S_OK;;
}

CBalrog_Fire_Ring * CBalrog_Fire_Ring::Create(CBalrog * pBalrog)
{
	CBalrog_Fire_Ring*		pInstance = new CBalrog_Fire_Ring();

	if (FAILED(pInstance->Ready_State(pBalrog)))
	{
		_MSG_BOX(L"CBalrog_Fire_Ring Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBalrog_Fire_Ring::Free()
{
}


