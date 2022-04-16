#include "stdafx.h"
#include "Flogas_FireSpurt.h"

#include "Flogas.h"
#include "Management.h"
#include "EffectGroup.h"
#include "Sound_Manager.h"

#include "Camera_Static.h"

_USING(Client)

CFlogas_FireSpurt::CFlogas_FireSpurt()
{
}

HRESULT CFlogas_FireSpurt::Begin()
{
	m_pFlogas->Set_AnimationSet(9);

	m_pFlogas->Set_FrameConst(1.f);
	
	m_bPlayOnce = false;

	//GetCamera;
	m_pCamera_Static = dynamic_cast<CCamera_Static*>(CManagement::GetInstance()->Get_CameraMain());
	if (m_pCamera_Static == nullptr)
		return E_FAIL;
	


	return S_OK;
}

_int CFlogas_FireSpurt::Update(const _float & fTimeDelta)
{
	if (m_pFlogas->Get_IsAnimationPassRadio(0.515f))
	{
		if (!m_bPlayOnce)
		{
			if (m_pCamera_Static != nullptr)
				m_pCamera_Static->Set_CameraShaking(0.3f);
			m_pEffect_FireSpurt->Set_Position(m_pFlogas->Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION));
			m_pEffect_FireSpurt->Set_Activate(true);

			CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::BOSS);
			CSound_Manager::GetInstance()->Play_Sound(L"Flogas_Skill_FireSpurt.mp3", CSound_Manager::BOSS);
		}
		m_bPlayOnce = true;
	}

	return 0;
}

_int CFlogas_FireSpurt::LastUpdate(const _float & fTimeDelta)
{
	if (m_pFlogas->Get_IsAnimationSetEnd())
	{
		m_pFlogas->Change_State(L"Idle");
	}

	return _int();
}

HRESULT CFlogas_FireSpurt::End()
{
	m_pFlogas->Set_FrameConst(1.f);
	m_pEffect_FireSpurt->Set_Activate(false);

	return S_OK;
}

HRESULT CFlogas_FireSpurt::Ready_State(CFlogas * pFlogas)
{
	m_pFlogas  = pFlogas;

	m_Damage.iDamage = 1000;
	m_Damage.iDamage_Range = 250;

	m_pEffect_FireSpurt = dynamic_cast<CEffectGroup*>
		(CManagement::GetInstance()->Get_GameObject(SCENE_STAGE, L"Layer_Effect_Flogas", 1));
	if (m_pEffect_FireSpurt == nullptr)
		return E_FAIL;

	////Collider Setup
	m_pEffect_FireSpurt->SetUp_ColliderInfo(15.f, COL_PLAYER, 0.15f, 0.5f);
	m_pEffect_FireSpurt->Set_Damage(&m_Damage);


	return S_OK;
}

CFlogas_FireSpurt * CFlogas_FireSpurt::Create(CFlogas* pFlogas)
{
	CFlogas_FireSpurt*		pInstance = new CFlogas_FireSpurt();

	if (FAILED(pInstance->Ready_State(pFlogas)))
	{
		_MSG_BOX(L"CFlogas_FireSpurt Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFlogas_FireSpurt::Free()
{
}


