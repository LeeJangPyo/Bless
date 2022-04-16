#include "stdafx.h"
#include "Balrog_Kick.h"

#include "Management.h"
#include "Collision_Manager.h"

#include "Balrog.h"

_USING(Client)

CBalrog_Kick::CBalrog_Kick()
{

}

HRESULT CBalrog_Kick::Begin()
{
	m_pBalrog->Set_AnimationSet(27);



	return S_OK;
}

_int CBalrog_Kick::Update(const _float & fTimeDelta)
{


	return 0;
}

_int CBalrog_Kick::LastUpdate(const _float & fTimeDelta)
{
	if (m_pBalrog->Get_IsAnimationSetEnd())
	{
		m_pBalrog->Change_State(L"Idle");
	}
	return 0;
}

HRESULT CBalrog_Kick::End()
{
	return S_OK;
}

HRESULT CBalrog_Kick::Ready_State(CBalrog * pBalrog)
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

CBalrog_Kick * CBalrog_Kick::Create(CBalrog * pBalrog)
{
	CBalrog_Kick*		pInstance = new CBalrog_Kick();

	if (FAILED(pInstance->Ready_State(pBalrog)))
	{
		_MSG_BOX(L"CBalrog_Kick Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBalrog_Kick::Free()
{
}


