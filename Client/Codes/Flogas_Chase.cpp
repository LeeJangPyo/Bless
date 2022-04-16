#include "stdafx.h"
#include "Flogas_Chase.h"

#include "Flogas.h"

#include "Management.h"

#include "Collision_Manager.h"
#include "Player_Manager.h"

_USING(Client)

CFlogas_Chase::CFlogas_Chase()
{
}

HRESULT CFlogas_Chase::Begin()
{
	m_pFlogas->Set_AnimationSet(11);


	m_pTarget_Position = CPlayer_Manager::GetInstance()->Get_Player_Position(m_pFlogas->Get_TargetPlayerType());

	return S_OK;
}

_int CFlogas_Chase::Update(const _float & fTimeDelta)
{
	if (m_pFlogas->Get_CurrentHp() < 0)
		m_pFlogas->Change_State(L"Dead");

	m_pFlogasTransform->LookAt_Target(m_pTarget_Position, fTimeDelta);
	m_pFlogas->Go_Target(m_pTarget_Position, fTimeDelta);

	//Monster Collision Check;
	CCollision_Manager::GetInstance()->Check_CollisionAndPush(COL_MONSTER, m_pFlogas);

	_vec3 vMovePos = m_pFlogas->Move_OnNaviMesh(m_pTarget_Position, fTimeDelta);
	m_pFlogasTransform->Set_StateInfo(CTransform::STATE_POSITION, &vMovePos);

	return 0;
}

_int CFlogas_Chase::LastUpdate(const _float & fTimeDelta)
{
	//상태 분기
	_vec3 vPos = *m_pFlogasTransform->Get_StateInfo(CTransform::STATE_POSITION);
	_vec3 vDir = *m_pTarget_Position - vPos;
	_float fDistance = D3DXVec3Length(&vDir);

	if (fDistance < m_pFlogas->Get_AttackRage())
	{
		m_pFlogas->Change_State(L"Idle");
	}

	return _int();
}

HRESULT CFlogas_Chase::End()
{
	return S_OK;
}

HRESULT CFlogas_Chase::Ready_State(CFlogas * pFlogas)
{
	m_pFlogas  = pFlogas;
	m_pFlogasTransform = m_pFlogas->Get_Transform();

	return S_OK;
}

CFlogas_Chase * CFlogas_Chase::Create(CFlogas* pFlogas)
{
	CFlogas_Chase*		pInstance = new CFlogas_Chase();

	if (FAILED(pInstance->Ready_State(pFlogas)))
	{
		_MSG_BOX(L"CFlogas_Chase Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFlogas_Chase::Free()
{
}


