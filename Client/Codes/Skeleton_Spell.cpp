#include "stdafx.h"
#include "Skeleton_Spell.h"

#include "Skeleton.h"
#include "Sound_Manager.h"

#include "Management.h"

#include "EffectGroup.h"

_USING(Client)

CSkeleton_Spell::CSkeleton_Spell()
	: m_fTimeAcc(0.f)
	, m_fSpellAccTime(0.f)
{
}

HRESULT CSkeleton_Spell::Begin()
{
	m_pSkeleton->Set_AnimationSet(1);

	//CEffectGroup*	pEffectGroup = dynamic_cast<CEffectGroup*>
	//	(CManagement::GetInstance()->Get_GameObject(SCENE_STAGE, L"Layer_Effect_Skeleton",0));
	//if (pEffectGroup == nullptr)
	//	return E_FAIL;

	//pEffectGroup->SetUp_EffectPosition(&_vec3(30.f, 1.f, 20.f));
	//pEffectGroup->Set_DeadObject(true);
	//pEffectGroup->Set_Activate(true);

	m_fTimeAcc = 0.f;
	m_fSpellAccTime = 0.f;

	return S_OK;
}

_int CSkeleton_Spell::Update(const _float & fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	m_fSpellAccTime += fTimeDelta;
	if (m_fSpellAccTime < 0.25f)
		return 0;
	m_fSpellAccTime = 0.f;
	

	CEffectGroup*	pEffectGroup;

	CManagement::GetInstance()->Add_Layer(SCENE_STAGE, L"Layer_Effect_Skeleton", L"GameObject_Effect_Rock", (CGameObject**)&pEffectGroup);

	_int iRand = rand() % 2;
	
	_int iPosRandX = rand() % 15 - 5;
	_int iPosRandZ = rand() % 15 - 5;
	_vec3 vPosition;
	if (iRand == 0)
	{
		vPosition = *m_pSire_Transfrom->Get_StateInfo(CTransform::STATE_POSITION);
		vPosition.x += iPosRandX;
		vPosition.y += 0.1f;
		vPosition.z += iPosRandZ;

		pEffectGroup->Set_DeadObject(true);
		pEffectGroup->SetUp_ColliderInfo(2.f, COLLISION_TARGET::COL_PLAYER,3.f,3.5f);
		pEffectGroup->Set_Damage(&m_Damage);
		pEffectGroup->Set_Position(&vPosition);
		pEffectGroup->Set_Activate(true);

		CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::BOSS);
		CSound_Manager::GetInstance()->Play_Sound(L"Skeleton_Spell.wav", CSound_Manager::BOSS);
	}
	else
	{
		vPosition = *m_pLups_Transfrom->Get_StateInfo(CTransform::STATE_POSITION);
		vPosition.x += iPosRandX;
		vPosition.y += 0.1f;
		vPosition.z += iPosRandZ;

		pEffectGroup->Set_DeadObject(true);
		pEffectGroup->SetUp_ColliderInfo(2.f, COLLISION_TARGET::COL_PLAYER, 3.f, 3.5f);
		pEffectGroup->Set_Damage(&m_Damage);
		pEffectGroup->Set_Position(&vPosition);
		pEffectGroup->Set_Activate(true);

		CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::BOSS);
		CSound_Manager::GetInstance()->Play_Sound(L"Skeleton_Spell.wav", CSound_Manager::BOSS);

	}
	
	
	return 0;
}

_int CSkeleton_Spell::LastUpdate(const _float & fTimeDelta)
{
	if (m_fTimeAcc > 5.f)
		m_pSkeleton->Change_State(L"Idle");

	return _int();
}

HRESULT CSkeleton_Spell::End()
{
	return S_OK;
}

HRESULT CSkeleton_Spell::Ready_State(CSkeleton * pSkeleton)
{
	m_pSkeleton  = pSkeleton;

	m_Damage.iDamage = 250;
	m_Damage.iDamage_Range = 100;

	//Get Player's Transform;
	m_pLups_Transfrom = dynamic_cast<CTransform*>
		(CManagement::GetInstance()->Get_Component(SCENE_STAGE, L"Layer_Lups", L"Com_Transform"));
	if (m_pLups_Transfrom == nullptr)
		return E_FAIL;

	//Get Player's Transform;
	m_pSire_Transfrom = dynamic_cast<CTransform*>
		(CManagement::GetInstance()->Get_Component(SCENE_STAGE, L"Layer_Sire", L"Com_Transform"));
	if (m_pSire_Transfrom == nullptr)
		return E_FAIL;

	return S_OK;
}

CSkeleton_Spell * CSkeleton_Spell::Create(CSkeleton* pSkeleton)
{
	CSkeleton_Spell*		pInstance = new CSkeleton_Spell();

	if (FAILED(pInstance->Ready_State(pSkeleton)))
	{
		_MSG_BOX(L"CSkeleton_Spell Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkeleton_Spell::Free()
{
}


