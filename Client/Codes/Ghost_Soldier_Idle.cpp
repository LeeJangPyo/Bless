#include "stdafx.h"
#include "Ghost_Soldier_Idle.h"

#include "Management.h"
#include "Collision_Manager.h"

#include "Ghost_Soldier.h"

_USING(Client)

CGhost_Soldier_Idle::CGhost_Soldier_Idle()
	:m_fIdelTime(0.f)
{

}

HRESULT CGhost_Soldier_Idle::Begin()
{
	m_fIdelTime = 0.f;

	m_pGhost_Solider->Set_AnimationSet(3);



	return S_OK;
}

_int CGhost_Soldier_Idle::Update(const _float & fTimeDelta)
{
	m_fIdelTime += fTimeDelta;

	if(m_pGhost_Solider->Get_CurrentHp() < 0)
		m_pGhost_Solider->Change_State(L"Dead");

	return 0;
}

_int CGhost_Soldier_Idle::LastUpdate(const _float & fTimeDelta)
{
	if (m_fIdelTime < 0.5f)
		return 0;

	//상태 분기
	_vec3 vPos = *m_pTransform->Get_StateInfo(CTransform::STATE_POSITION);
	_vec3 vPlayerPos = *m_pPlayerTransform->Get_StateInfo(CTransform::STATE_POSITION);
	_vec3 vDir = vPlayerPos - vPos;
	_float fDistance = D3DXVec3Length(&vDir);

	if (CCollision_Manager::GetInstance()->Check_Collision(COL_PLAYER, m_pGhost_Solider))
	{
		// Attack
		m_pGhost_Solider->Change_State(L"Attack");
	}
	else
	{
		// 추격
		m_pGhost_Solider->Change_State(L"Chase");
	}

	return 0;
}

HRESULT CGhost_Soldier_Idle::End()
{
	return S_OK;
}

HRESULT CGhost_Soldier_Idle::Ready_State(CGhost_Soldier * pGhost_Soldier)
{
	m_pGhost_Solider = pGhost_Soldier;


	//Get Player's Transform;
	m_pPlayerTransform = dynamic_cast<CTransform*>
		(CManagement::GetInstance()->Get_Component(SCENE_STAGE, L"Layer_Lups", L"Com_Transform"));
	if (m_pPlayerTransform == nullptr)
		return E_FAIL;

	m_pTransform = m_pGhost_Solider->Get_Transform();
	return S_OK;
}

CGhost_Soldier_Idle * CGhost_Soldier_Idle::Create(CGhost_Soldier * pGhost_Soldier)
{
	CGhost_Soldier_Idle*		pInstance = new CGhost_Soldier_Idle();

	if (FAILED(pInstance->Ready_State(pGhost_Soldier)))
	{
		_MSG_BOX(L"CGhost_Soldier_Idle Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGhost_Soldier_Idle::Free()
{
}


