#include "stdafx.h"
#include "Balrog_Idle.h"

#include "Management.h"
#include "Collision_Manager.h"

#include "Balrog.h"

_USING(Client)

CBalrog_Idle::CBalrog_Idle()
{

}

HRESULT CBalrog_Idle::Begin()
{
	m_pBalrog->Set_AnimationSet(10);



	return S_OK;
}

_int CBalrog_Idle::Update(const _float & fTimeDelta)
{
	_vec3 vPlayerPos = *m_pPlayerTransform->Get_StateInfo(CTransform::STATE_POSITION);
	m_pTransform->LookAt_Target(&vPlayerPos, 1.f);

	return 0;
}

_int CBalrog_Idle::LastUpdate(const _float & fTimeDelta)
{
	
	switch (m_iAnimationIndex)
	{
	case 0:
		m_pBalrog->Change_State(L"Electric_Whip");
		m_iAnimationIndex++;
		break;
	case 1:
		m_pBalrog->Change_State(L"Fire_Ring");
		m_iAnimationIndex++;
		break;
	case 2:
		m_pBalrog->Change_State(L"Kick");
		m_iAnimationIndex++;
		break;
	case 3:
		m_pBalrog->Change_State(L"Breath");
		m_iAnimationIndex++;
		break;
	}
	if (m_iAnimationIndex > 3)
		m_iAnimationIndex = 0;

	return 0;
}

HRESULT CBalrog_Idle::End()
{
	return S_OK;
}

HRESULT CBalrog_Idle::Ready_State(CBalrog * pBalrog)
{
	m_pBalrog = pBalrog;
	m_iAnimationIndex = 0;

	//Get Player's Transform;
	m_pPlayerTransform = dynamic_cast<CTransform*>
		(CManagement::GetInstance()->Get_Component(SCENE_STAGE, L"Layer_Lups", L"Com_Transform"));
	if (m_pPlayerTransform == nullptr)
		return E_FAIL;

	m_pTransform = m_pBalrog->Get_Transform();
	return S_OK;;
}

CBalrog_Idle * CBalrog_Idle::Create(CBalrog * pBalrog)
{
	CBalrog_Idle*		pInstance = new CBalrog_Idle();

	if (FAILED(pInstance->Ready_State(pBalrog)))
	{
		_MSG_BOX(L"CBalrog_Idle Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBalrog_Idle::Free()
{
}


