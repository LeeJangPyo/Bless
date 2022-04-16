#include "stdafx.h"
#include "Balrog_Electric_Whip.h"

#include "Management.h"
#include "Collision_Manager.h"

#include "Balrog.h"

_USING(Client)

CBalrog_Electric_Whip::CBalrog_Electric_Whip()
{

}

HRESULT CBalrog_Electric_Whip::Begin()
{
	m_pBalrog->Set_AnimationSet(3);



	return S_OK;
}

_int CBalrog_Electric_Whip::Update(const _float & fTimeDelta)
{


	return 0;
}

_int CBalrog_Electric_Whip::LastUpdate(const _float & fTimeDelta)
{
	if (m_pBalrog->Get_IsAnimationSetEnd())
	{
		m_pBalrog->Change_State(L"Idle");
	}
	return 0;
}

HRESULT CBalrog_Electric_Whip::End()
{
	return S_OK;
}

HRESULT CBalrog_Electric_Whip::Ready_State(CBalrog * pBalrog)
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

CBalrog_Electric_Whip * CBalrog_Electric_Whip::Create(CBalrog * pBalrog)
{
	CBalrog_Electric_Whip*		pInstance = new CBalrog_Electric_Whip();

	if (FAILED(pInstance->Ready_State(pBalrog)))
	{
		_MSG_BOX(L"CBalrog_Electric_Whip Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBalrog_Electric_Whip::Free()
{
}


