#include "stdafx.h"
#include "Lups_DownAndUp.h"

#include "Management.h"
#include "Collision_Manager.h"

#include "Lups.h"
#include "Damage.h"

_USING(Client)

CLups_DownAndUp::CLups_DownAndUp()
	:m_iAnimationIndex(0)
{

}

HRESULT CLups_DownAndUp::Begin()
{
	m_iAnimationIndex = 48;
	m_pLups->Set_AnimationSet(m_iAnimationIndex,true);
	
	m_pLups->Set_FrameConst(0.75f);

	return S_OK;
}

_int CLups_DownAndUp::Update(const _float & fTimeDelta)
{

	return 0;
}

_int CLups_DownAndUp::LastUpdate(const _float & fTimeDelta)
{
	if (m_pLups->Get_IsAnimationPassRadio(0.5f) && m_iAnimationIndex == 48)
	{
		//m_pLups->Change_State(L"Idle");
		m_iAnimationIndex = 46;
		m_pLups->Set_AnimationSet(m_iAnimationIndex);
	}
	if (m_pLups->Get_IsAnimationPassRadio(0.5f) && m_iAnimationIndex == 46)
	{
		//m_pLups->Change_State(L"Idle");
		m_iAnimationIndex = 45;
		m_pLups->Set_AnimationSet(m_iAnimationIndex);
	}
	if (m_pLups->Get_IsAnimationPassRadio(0.5f) && m_iAnimationIndex == 45)
	{
		m_pLups->Change_State(L"Idle");
	}


	return 0;
}

HRESULT CLups_DownAndUp::End()
{
	m_pLups->Set_FrameConst(1.f);

	return S_OK;
}

HRESULT CLups_DownAndUp::Ready_State(CLups * pLups)
{
	m_pLups = pLups;

	return S_OK;;
}

CLups_DownAndUp * CLups_DownAndUp::Create(CLups * pLups)
{
	CLups_DownAndUp*		pInstance = new CLups_DownAndUp();

	if (FAILED(pInstance->Ready_State(pLups)))
	{
		_MSG_BOX(L"CLups_DownAndUp Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLups_DownAndUp::Free()
{
}


