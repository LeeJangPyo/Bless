#include "stdafx.h"
#include "Quatra_Breath.h"

#include "Management.h"

#include "Quatra.h"
#include "Player_Manager.h"
#include "Sound_Manager.h"

#include "Effect_QuatraBreath.h"

_USING(Client)

CQuatra_Breath::CQuatra_Breath()
{
}

HRESULT CQuatra_Breath::Begin()
{
	m_pQuatra->Set_AnimationSet(3);

	m_pQuatra_Transform->Set_StateInfo(CTransform::STATE_POSITION, 128.f, 5.f, 10.f);

	m_fTimeAcc = 0.f;

	CManagement::GetInstance()->Add_Layer(SCENE_STAGE, L"Layer_Breath", L"GameObject_Quatra_Breath", (CGameObject**)&m_pEffect_Breath);
	
	m_pEffect_Breath->Set_Activate(true);

	return S_OK;
}

_int CQuatra_Breath::Update(const _float & fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	const _vec3* pPlayer_Position = CPlayer_Manager::GetInstance()->Get_Player_Position();

	m_pQuatra_Transform->LookAt_Target(pPlayer_Position, fTimeDelta);
	m_pQuatra_Transform->Go_Target(pPlayer_Position,4.f,fTimeDelta,4.5f);

	_vec3 vPosition = *m_pQuatra_Transform->Get_StateInfo(CTransform::STATE_POSITION);
	vPosition.y += 5.f;
	m_pEffect_Breath->Set_Position(&vPosition);

	_float fRadian = m_pQuatra_Transform->Get_RotationInfo(CTransform::STATE_UP);
	m_pEffect_Breath->Set_Rotation(fRadian);
	
	if (m_fTimeAcc > 2.5f)
	{
		CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::BOSS);
		CSound_Manager::GetInstance()->Play_Sound(L"Quatra_Breath.mp3", CSound_Manager::BOSS);
		m_fTimeAcc = 0.f;
	}

	return 0;
}

_int CQuatra_Breath::LastUpdate(const _float & fTimeDelta)
{

	if (m_pQuatra->Get_IsAnimationSetEnd())
		m_pQuatra->Set_AnimationSet(3);

	return _int();
}

HRESULT CQuatra_Breath::End()
{
	m_pEffect_Breath->Set_Activate(false);

	return S_OK;
}

HRESULT CQuatra_Breath::Ready_State(CQuatra * pQuatra)
{
	m_pQuatra  = pQuatra;

	m_pQuatra_Transform = m_pQuatra->Get_Transform();



	return S_OK;
}

CQuatra_Breath * CQuatra_Breath::Create(CQuatra* pQuatra)
{
	CQuatra_Breath*		pInstance = new CQuatra_Breath();

	if (FAILED(pInstance->Ready_State(pQuatra)))
	{
		_MSG_BOX(L"CQuatra_Breath Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQuatra_Breath::Free()
{
}


