#include "stdafx.h"
#include "Ghost_Mage_IceBolt.h"

#include "Management.h"
#include "Collision_Manager.h"
#include "Player_Manager.h"
#include "Sound_Manager.h"

#include "Ghost_Mage.h"

#include "IceBolt.h"

#include "EffectGroup.h"

_USING(Client)

CGhost_Mage_IceBolt::CGhost_Mage_IceBolt()
{

}

HRESULT CGhost_Mage_IceBolt::Begin()
{
	m_pGhost_Mage->Set_AnimationSet(13);

	m_pEffect_MagicSquare->Set_Position(m_pTransform->Get_StateInfo(CTransform::STATE_POSITION));
	m_pEffect_MagicSquare->Set_Activate(true);

	m_pTarget_Position = CPlayer_Manager::GetInstance()->Get_Player_Position(m_pGhost_Mage->Get_TargetPlayerType());

	return S_OK;
}

_int CGhost_Mage_IceBolt::Update(const _float & fTimeDelta)
{
	if (m_pGhost_Mage->Get_CurrentHp() < 0)
		m_pGhost_Mage->Change_State(L"Dead");


	m_pTransform->LookAt_Target(m_pTarget_Position, fTimeDelta);

	//CCollision_Manager::GetInstance()->Check_CollisionAndPush(CCollision_Manager::COL_MONSTER, m_pGhost_Mage);


	return 0;
}

_int CGhost_Mage_IceBolt::LastUpdate(const _float & fTimeDelta)
{

	if (m_pGhost_Mage->Get_IsAnimationSetEnd())
	{
		m_pGhost_Mage->Change_State(L"Idle");
	}

	return 0;
}

HRESULT CGhost_Mage_IceBolt::End()
{
	CIceBolt*	pIceBolt;
	if (FAILED(CManagement::GetInstance()->Add_Layer(SCENE_STAGE, L"Layer_IceBolt", L"GameObject_IceBolt", (CGameObject**)&pIceBolt)))
		return E_FAIL;


	CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::MONSTER);
	CSound_Manager::GetInstance()->Play_Sound(L"Ghost_Mgae_IceBolt.wav", CSound_Manager::MONSTER);

	_vec3 vPosition = *m_pTransform->Get_StateInfo(CTransform::STATE_POSITION);
	vPosition.y += 2.5f;
	pIceBolt->SetUp_IceBoltInfo(m_pTarget_Position, &vPosition);

	m_pEffect_MagicSquare->Set_Activate(false);

	return S_OK;
}

HRESULT CGhost_Mage_IceBolt::Ready_State(CGhost_Mage * pGhost_Mage)
{
	m_pGhost_Mage = pGhost_Mage;

	m_pTransform = m_pGhost_Mage->Get_Transform();

	CManagement::GetInstance()->Add_Layer(SCENE_STAGE, L"Layer_Effect_Ghost_Mage", L"GameObject_Effect_Ghost_Mage_MagicSqure",
		(CGameObject**)&m_pEffect_MagicSquare);
	if (m_pEffect_MagicSquare == nullptr)
		return E_FAIL;

	return S_OK;;
}

CGhost_Mage_IceBolt * CGhost_Mage_IceBolt::Create(CGhost_Mage * pGhost_Mage)
{
	CGhost_Mage_IceBolt*		pInstance = new CGhost_Mage_IceBolt();

	if (FAILED(pInstance->Ready_State(pGhost_Mage)))
	{
		_MSG_BOX(L"CGhost_Mage_IceBolt Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGhost_Mage_IceBolt::Free()
{
}


