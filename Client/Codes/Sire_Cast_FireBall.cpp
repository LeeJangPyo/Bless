#include "stdafx.h"
#include "Sire_Cast_FireBall.h"

#include "Management.h"
#include "Collision_Manager.h"
#include "Sound_Manager.h"

#include "Sire.h"
#include "Staff.h"

//UI
#include "UI_Frame.h"

//Effect
#include "EffectGroup.h"

_USING(Client)

CSire_Cast_FireBall::CSire_Cast_FireBall()
{
	ZeroMemory(&m_Damage, sizeof(DAMAGE));
}

HRESULT CSire_Cast_FireBall::Begin()
{
	m_pSire->Set_AnimationSet(15);

	_vec3 vPosition = *m_pSire->Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION);
	vPosition.y += 3.f;

	m_pEffect_FireBall->Set_Position(&vPosition);

	_float fRadian = m_pSire->Get_Transform()->Get_RotationInfo(CTransform::STATE_UP);
	m_pEffect_FireBall->RotationY(fRadian);

	_vec3 vDir = _vec3(0.f, 0.f, 1.f);
	_matrix matRot;
	D3DXMatrixRotationY(&matRot, fRadian);
	D3DXVec3TransformNormal(&vDir, &vDir, &matRot);
	
	m_pEffect_FireBall->Set_Move(true, 10.f, &vDir);
	m_pEffect_FireBall->Set_EffectType(EFFECT_FIRE);
	m_pEffect_FireBall->Set_Activate(true);
	

	CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::SIRE);
	CSound_Manager::GetInstance()->Play_Sound(L"Sire_FireBall.mp3", CSound_Manager::SIRE);

	return S_OK;
}

_int CSire_Cast_FireBall::Update(const _float & fTimeDelta)
{
	

	return 0;
}

_int CSire_Cast_FireBall::LastUpdate(const _float & fTimeDelta)
{

	if (m_pSire->Get_IsAnimationSetEnd())
	{
		m_pSire->Change_State(L"Idle");
	}

	return 0;
}

HRESULT CSire_Cast_FireBall::End()
{

//	m_pEffect_FireBall->Set_Activate(false);
	return S_OK;
}

HRESULT CSire_Cast_FireBall::Ready_State(CSire* pSire)
{
	m_pSire = pSire;

	m_Damage.iDamage = 2000;
	m_Damage.iDamage_Range = 1000;

	//FireBall ¾ò¾î¿È
	m_pEffect_FireBall = dynamic_cast<CEffectGroup*>
		(CManagement::GetInstance()->Get_GameObject(SCENE_STAGE, L"Layer_Effect_Sire", 1));
	if (m_pEffect_FireBall == nullptr)
		return E_FAIL;

	//Collider Setup
	m_pEffect_FireBall->SetUp_ColliderInfo(3.f, COL_MONSTER, 0.f,10.f);
	m_pEffect_FireBall->Set_Damage(&m_Damage);

	return S_OK;;
}

CSire_Cast_FireBall * CSire_Cast_FireBall::Create(CSire* pSire)
{
	CSire_Cast_FireBall*		pInstance = new CSire_Cast_FireBall();

	if (FAILED(pInstance->Ready_State(pSire)))
	{
		_MSG_BOX(L"CSire_Cast_FireBall Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSire_Cast_FireBall::Free()
{
}


