#include "stdafx.h"
#include "Flogas_Earthquake.h"

#include "Flogas.h"

#include "Management.h"
#include "Sound_Manager.h"

#include "EffectGroup.h"
#include "Camera_Static.h"

_USING(Client)

CFlogas_Earthquake::CFlogas_Earthquake()

{
}

HRESULT CFlogas_Earthquake::Begin()
{
	m_bPlayOnce = false;

	m_pFlogas->Set_AnimationSet(4);

	m_pFlogas->Set_FrameConst(0.5f);
	

	//GetCamera;
	m_pCamera_Static = dynamic_cast<CCamera_Static*>(CManagement::GetInstance()->Get_CameraMain());
	if (m_pCamera_Static == nullptr)
		return E_FAIL;


	_vec3 vEffectPoint = *m_pFlogas->Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION);
	_vec3 vDir = *m_pFlogas->Get_Transform()->Get_StateInfo(CTransform::STATE_LOOK);
	D3DXVec3Normalize(&vDir, &vDir);
	vEffectPoint += 7.5f * vDir;

	m_pEffect_Earthquake->Set_Position(&vEffectPoint);

	return S_OK;
}

_int CFlogas_Earthquake::Update(const _float & fTimeDelta)
{
	
	if (m_pFlogas->Get_IsAnimationPassRadio(0.375f))
	{
		if (!m_bPlayOnce)
		{
			if (m_pCamera_Static != nullptr)
				m_pCamera_Static->Set_CameraShaking(0.3f);
		//	m_pEffect_Earthquake->Set_Position(m_pFlogas->Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION));
			m_pEffect_Earthquake->Set_Activate(true);

			CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::BOSS);
			CSound_Manager::GetInstance()->Play_Sound(L"Flogas_Skill_Earthquake.mp3", CSound_Manager::BOSS);
		}
		m_bPlayOnce = true;
	}
	
	return 0;
}

_int CFlogas_Earthquake::LastUpdate(const _float & fTimeDelta)
{
	if (m_pFlogas->Get_IsAnimationSetEnd())
	{
		m_pFlogas->Change_State(L"Idle");
	}

	return _int();
}

HRESULT CFlogas_Earthquake::End()
{
	return S_OK;
}

HRESULT CFlogas_Earthquake::Ready_State(CFlogas * pFlogas)
{
	m_pFlogas  = pFlogas;

	m_Damage.iDamage = 1000;
	m_Damage.iDamage_Range = 250;

	m_pEffect_Earthquake = dynamic_cast<CEffectGroup*>
		(CManagement::GetInstance()->Get_GameObject(SCENE_STAGE, L"Layer_Effect_Flogas", 0));
	if (m_pEffect_Earthquake == nullptr)
		return E_FAIL;

	//Collider Setup
	m_pEffect_Earthquake->SetUp_ColliderInfo(7.5f, COL_PLAYER, 0.5f,1.f);
	m_pEffect_Earthquake->Set_Damage(&m_Damage);

	return S_OK;
}

CFlogas_Earthquake * CFlogas_Earthquake::Create(CFlogas* pFlogas)
{
	CFlogas_Earthquake*		pInstance = new CFlogas_Earthquake();

	if (FAILED(pInstance->Ready_State(pFlogas)))
	{
		_MSG_BOX(L"CFlogas_Earthquake Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFlogas_Earthquake::Free()
{
}


