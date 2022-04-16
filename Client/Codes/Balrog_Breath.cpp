#include "stdafx.h"
#include "Balrog_Breath.h"

#include "Management.h"
#include "Collision_Manager.h"

#include "Balrog.h"

_USING(Client)

CBalrog_Breath::CBalrog_Breath()
{

}

HRESULT CBalrog_Breath::Begin()
{
	m_pBalrog->Set_AnimationSet(6);



	return S_OK;
}

_int CBalrog_Breath::Update(const _float & fTimeDelta)
{


	return 0;
}

_int CBalrog_Breath::LastUpdate(const _float & fTimeDelta)
{
	if (m_pBalrog->Get_IsAnimationSetEnd())
	{
		m_pBalrog->Change_State(L"Idle");
	}

	return 0;
}

HRESULT CBalrog_Breath::End()
{
	return S_OK;
}

HRESULT CBalrog_Breath::Ready_State(CBalrog * pBalrog)
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

CBalrog_Breath * CBalrog_Breath::Create(CBalrog * pBalrog)
{
	CBalrog_Breath*		pInstance = new CBalrog_Breath();

	if (FAILED(pInstance->Ready_State(pBalrog)))
	{
		_MSG_BOX(L"CBalrog_Breath Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBalrog_Breath::Free()
{
}


