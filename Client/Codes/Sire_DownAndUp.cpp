#include "stdafx.h"
#include "Sire_DownAndUp.h"

#include "Management.h"
#include "Collision_Manager.h"

#include "Sire.h"
#include "Damage.h"

_USING(Client)

CSire_DownAndUp::CSire_DownAndUp()
	:m_iAnimationIndex(0)
{

}

HRESULT CSire_DownAndUp::Begin()
{
	m_iAnimationIndex = 58;
	m_pSire->Set_AnimationSet(m_iAnimationIndex,true);
	
	m_pSire->Set_FrameConst(0.75f);

	return S_OK;
}

_int CSire_DownAndUp::Update(const _float & fTimeDelta)
{

	return 0;
}

_int CSire_DownAndUp::LastUpdate(const _float & fTimeDelta)
{
	if (m_pSire->Get_IsAnimationPassRadio(0.5f) && m_iAnimationIndex == 58)
	{
		//m_pSire->Change_State(L"Idle");
		m_iAnimationIndex = 56;
		m_pSire->Set_AnimationSet(m_iAnimationIndex);
	}
	if (m_pSire->Get_IsAnimationPassRadio(0.5f) && m_iAnimationIndex == 56)
	{
		//m_pSire->Change_State(L"Idle");
		m_iAnimationIndex = 55;
		m_pSire->Set_AnimationSet(m_iAnimationIndex);
	}
	if (m_pSire->Get_IsAnimationPassRadio(0.5f) && m_iAnimationIndex == 55)
	{
		m_pSire->Change_State(L"Idle");
	}

	return 0;
}

HRESULT CSire_DownAndUp::End()
{
	m_pSire->Set_FrameConst(1.f);

	return S_OK;
}

HRESULT CSire_DownAndUp::Ready_State(CSire * pSire)
{
	m_pSire = pSire;

	return S_OK;;
}

CSire_DownAndUp * CSire_DownAndUp::Create(CSire * pSire)
{
	CSire_DownAndUp*		pInstance = new CSire_DownAndUp();

	if (FAILED(pInstance->Ready_State(pSire)))
	{
		_MSG_BOX(L"CSire_DownAndUp Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSire_DownAndUp::Free()
{
}


