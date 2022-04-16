#include "stdafx.h"
#include "Ghost_Soldier_Dead.h"

#include "Management.h"
#include "Sound_Manager.h"

#include "Ghost_Soldier.h"

_USING(Client)

CGhost_Soldier_Dead::CGhost_Soldier_Dead()
	:m_fWaitTime(1.f),
	m_fTimeAcc(0.f),
	m_bIsAnimationEnd(false)
{

}

HRESULT CGhost_Soldier_Dead::Begin()
{
	m_pGhost_Solider->Set_AnimationSet(14);

	m_pGhost_Solider->Set_FrameConst(1.5);

	
	return S_OK;
}

_int CGhost_Soldier_Dead::Update(const _float & fTimeDelta)
{
	//애니메이션이 끝나면 검사
	if (m_pGhost_Solider->Get_IsAnimationSetEnd())
	{
		m_pGhost_Solider->Set_AnimationSet(15);
		m_bIsAnimationEnd = true;
	}

	if (m_bIsAnimationEnd)
	{
		m_fTimeAcc += fTimeDelta;
		if (m_fTimeAcc > m_fWaitTime)
		{
			//사라짐
			m_pGhost_Solider->Set_IsDead();
		}
	}


	return 0;
}

_int CGhost_Soldier_Dead::LastUpdate(const _float & fTimeDelta)
{
	return _int();
}

HRESULT CGhost_Soldier_Dead::End()
{
	m_pGhost_Solider->Set_FrameConst(1.f);
	return S_OK;
}

HRESULT CGhost_Soldier_Dead::Ready_State(CGhost_Soldier * pGhost_Soldier)
{
	m_pGhost_Solider = pGhost_Soldier;


	//Get Player's Transform;
	m_pPlayerTransform = dynamic_cast<CTransform*>
		(CManagement::GetInstance()->Get_Component(SCENE_STAGE, L"Layer_Lups", L"Com_Transform"));
	if (m_pPlayerTransform == nullptr)
		return E_FAIL;


	m_pTransform = m_pGhost_Solider->Get_Transform();

	//Get
	m_fColliderRadius = m_pGhost_Solider->Get_LongestRadius();


	m_fWaitTime = 1.f;
	m_bIsAnimationEnd = false;
	return S_OK;;
}

CGhost_Soldier_Dead * CGhost_Soldier_Dead::Create(CGhost_Soldier * pGhost_Soldier)
{
	CGhost_Soldier_Dead*		pInstance = new CGhost_Soldier_Dead();

	if (FAILED(pInstance->Ready_State(pGhost_Soldier)))
	{
		_MSG_BOX(L"CGhost_Soldier_Dead Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGhost_Soldier_Dead::Free()
{
}


