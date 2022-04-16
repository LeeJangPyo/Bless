#include "stdafx.h"
#include "Balrog_Chase.h"

#include "Management.h"
#include "Collision_Manager.h"


#include "Balrog.h"

_USING(Client)

CBalrog_Chase::CBalrog_Chase()
{

}

HRESULT CBalrog_Chase::Begin()
{
	m_pBalrog->Set_AnimationSet(11);

	//Get


	return S_OK;
}

_int CBalrog_Chase::Update(const _float & fTimeDelta)
{
	_vec3 vPlayerPos = *m_pPlayerTransform->Get_StateInfo(CTransform::STATE_POSITION);
	m_pTransform->LookAt_Target(&vPlayerPos, fTimeDelta);
//	m_pBalrog->Go_Target(&vPlayerPos, fTimeDelta);

	//Monster Collision Check;
	CCollision_Manager::GetInstance()->Check_CollisionAndPush(COL_MONSTER, m_pBalrog);

	_vec3 vMovePos = m_pBalrog->Move_OnNaviMesh(&vPlayerPos, fTimeDelta);
	m_pTransform->Set_StateInfo(CTransform::STATE_POSITION, &vMovePos);
	/*_vec3 vPosition = m_pGhost_Solider->Stand_OnNaviMesh(m_pTransform->Get_StateInfo(CTransform::STATE_POSITION));
	m_pTransform->Set_StateInfo(CTransform::STATE_POSITION, &vPosition);*/

	return 0;
}

_int CBalrog_Chase::LastUpdate(const _float & fTimeDelta)
{

	_float fDistance = m_pTransform->Get_Distance(m_pPlayerTransform->Get_StateInfo(CTransform::STATE_POSITION));
	if (fDistance < m_pBalrog->Get_AttackRage())
	{
		m_pBalrog->Change_State(L"Idle");
	}
	return 0;
}

HRESULT CBalrog_Chase::End()
{
	return S_OK;
}

HRESULT CBalrog_Chase::Ready_State(CBalrog * pBalrog)
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

CBalrog_Chase * CBalrog_Chase::Create(CBalrog * pBalrog)
{
	CBalrog_Chase*		pInstance = new CBalrog_Chase();

	if (FAILED(pInstance->Ready_State(pBalrog)))
	{
		_MSG_BOX(L"CBalrog_Chase Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBalrog_Chase::Free()
{
}


