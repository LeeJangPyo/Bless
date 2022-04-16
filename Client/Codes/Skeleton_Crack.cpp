#include "stdafx.h"
#include "Skeleton_Crack.h"

#include "Skeleton.h"
#include "Management.h"
#include "Sound_Manager.h"

#include "EffectGroup.h"
#include "Camera_Static.h"

_USING(Client)

CSkeleton_Crack::CSkeleton_Crack()
	:m_bPlayOnce(false)
{
}

HRESULT CSkeleton_Crack::Begin()
{

	m_bPlayOnce = false;

	m_pSkeleton->Set_AnimationSet(0);

	m_pSkeleton->Set_FrameConst(0.5f);
	

	//GetCamera;
	m_pCamera_Static = dynamic_cast<CCamera_Static*>(CManagement::GetInstance()->Get_CameraMain());
	if (m_pCamera_Static == nullptr)
		return E_FAIL;

	return S_OK;
}

_int CSkeleton_Crack::Update(const _float & fTimeDelta)
{
	if (m_pSkeleton->Get_CurrentHp() < 0)
		m_pSkeleton->Change_State(L"Dead");

	if(m_pSkeleton->Get_IsAnimationPassRadio(0.55f))
	{
		if (!m_bPlayOnce)
		{
			if(m_pCamera_Static != nullptr)
				m_pCamera_Static->Set_CameraShaking(0.3f);
			m_pEffect_Crack->Set_Position(m_pSkeleton->Get_WeaponCollider_Position());
			m_pEffect_Crack->Set_Activate(true);

			CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::BOSS);
			CSound_Manager::GetInstance()->Play_Sound(L"Skeleton_Crack.mp3", CSound_Manager::BOSS);
		}
		m_bPlayOnce = true;
	}

	return 0;
}

_int CSkeleton_Crack::LastUpdate(const _float & fTimeDelta)
{
	if (m_pSkeleton->Get_IsAnimationSetEnd())
	{
		m_pSkeleton->Change_State(L"Idle");
	}

	return _int();
}

HRESULT CSkeleton_Crack::End()
{
	m_pSkeleton->Set_FrameConst(1.f);
	m_pEffect_Crack->Set_Activate(false);

	return S_OK;
}

HRESULT CSkeleton_Crack::Ready_State(CSkeleton * pSkeleton)
{
	m_pSkeleton  = pSkeleton;

	m_Damage.iDamage = 1000;
	m_Damage.iDamage_Range = 500;

	m_pEffect_Crack = dynamic_cast<CEffectGroup*>
		(CManagement::GetInstance()->Get_GameObject(SCENE_STAGE, L"Layer_Effect_Skeleton", 1));
	if (m_pEffect_Crack == nullptr)
		return E_FAIL;

	//Collider Setup
	m_pEffect_Crack->SetUp_ColliderInfo(5.f, COL_PLAYER, 0.f,1.f);
	m_pEffect_Crack->Set_Damage(&m_Damage);


	return S_OK;
}

CSkeleton_Crack * CSkeleton_Crack::Create(CSkeleton* pSkeleton)
{
	CSkeleton_Crack*		pInstance = new CSkeleton_Crack();

	if (FAILED(pInstance->Ready_State(pSkeleton)))
	{
		_MSG_BOX(L"CSkeleton_Crack Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkeleton_Crack::Free()
{
}


