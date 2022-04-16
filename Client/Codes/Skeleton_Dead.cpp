#include "stdafx.h"
#include "Skeleton_Dead.h"

#include "Skeleton.h"
#include "Sound_Manager.h"

#include "Management.h"
#include "EffectGroup.h"

_USING(Client)

CSkeleton_Dead::CSkeleton_Dead()
	:m_pSkeleton(nullptr)
	, m_fWaitTime(1.f)
	, m_fTimeAcc(0.f)
	, m_bIsAnimationEnd(0.f)
{
}

HRESULT CSkeleton_Dead::Begin()
{
	m_pSkeleton->Set_AnimationSet(17);

	return S_OK;
}

_int CSkeleton_Dead::Update(const _float & fTimeDelta)
{

	return 0;
}

_int CSkeleton_Dead::LastUpdate(const _float & fTimeDelta)
{
	if (m_pSkeleton->Get_IsAnimationSetEnd())
	{
		
		m_pSkeleton->Set_AnimationSet(18);
		m_bIsAnimationEnd = true;

		CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::BOSS);
		CSound_Manager::GetInstance()->Play_Sound(L"Skeleton_Die_0.mp3", CSound_Manager::BOSS);
	}

	if (m_bIsAnimationEnd)
	{
		m_fTimeAcc += fTimeDelta;
		if (m_fTimeAcc > m_fWaitTime)
		{
			m_pSkeleton->Set_IsDead();

			CEffectGroup* pEffect;

			pEffect = dynamic_cast<CEffectGroup*>
				(CManagement::GetInstance()->Get_GameObject(SCENE_STAGE, L"Layer_Effect_Gate", 0));
			if (pEffect == nullptr)
				return E_FAIL;

			pEffect->Set_Position(&_vec3(8.f, 0.f, 110.f));
			pEffect->Set_Activate(true);
		}
	}

	return _int();
}

HRESULT CSkeleton_Dead::End()
{
	return S_OK;
}

HRESULT CSkeleton_Dead::Ready_State(CSkeleton * pSkeleton)
{
	m_pSkeleton  = pSkeleton;
	return S_OK;;
}

CSkeleton_Dead * CSkeleton_Dead::Create(CSkeleton* pSkeleton)
{
	CSkeleton_Dead*		pInstance = new CSkeleton_Dead();

	if (FAILED(pInstance->Ready_State(pSkeleton)))
	{
		_MSG_BOX(L"CSkeleton_Dead Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkeleton_Dead::Free()
{
}


