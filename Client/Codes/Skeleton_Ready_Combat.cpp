#include "stdafx.h"
#include "Skeleton_Ready_Combat.h"

#include "Skeleton.h"

#include "Sound_Manager.h"

_USING(Client)

CSkeleton_Ready_Combat::CSkeleton_Ready_Combat()
	:m_bPlayOnce(false)
{
}

HRESULT CSkeleton_Ready_Combat::Begin()
{
	m_pSkeleton->Set_AnimationSet(1);

	return S_OK;
}

_int CSkeleton_Ready_Combat::Update(const _float & fTimeDelta)
{
	if (m_pSkeleton->Get_IsAnimationPassRadio(0.5f))
	{
		if (m_bPlayOnce == false)
		{
			CSound_Manager::GetInstance()->Play_Sound(L"Roar.mp3", CSound_Manager::BOSS);
			m_bPlayOnce = true;
		}
	}

	return 0;
}

_int CSkeleton_Ready_Combat::LastUpdate(const _float & fTimeDelta)
{
	return _int();
}

HRESULT CSkeleton_Ready_Combat::End()
{
	m_pSkeleton->Set_HpBar_Activate(true);
	return S_OK;
}

HRESULT CSkeleton_Ready_Combat::Ready_State(CSkeleton * pSkeleton)
{
	m_pSkeleton  = pSkeleton;
	return S_OK;;
}

CSkeleton_Ready_Combat * CSkeleton_Ready_Combat::Create(CSkeleton* pSkeleton)
{
	CSkeleton_Ready_Combat*		pInstance = new CSkeleton_Ready_Combat();

	if (FAILED(pInstance->Ready_State(pSkeleton)))
	{
		_MSG_BOX(L"CSkeleton_Ready_Combat Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkeleton_Ready_Combat::Free()
{
}


