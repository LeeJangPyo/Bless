#include "stdafx.h"
#include "..\Headers\Ghost_Mage.h"


#include "Management.h"
#include "Collision_Manager.h"
#include "Sound_Manager.h"

//State
#include "Ghost_Mage_Wait.h"
#include "Ghost_Mage_Ready_Combat.h"
#include "Ghost_Mage_Idle.h"
#include "Ghost_Mage_Chase.h"
#include "Ghost_Mage_Attack.h"
#include "Ghost_Mage_Dead.h"
#include "Ghost_Mage_Hit.h"
#include "Ghost_Mage_IceBolt.h"
#include "Ghost_Mage_DownAndUp.h"

//Damage Effect
#include "Damage.h"
#include "Blood.h"
#include "HitFire.h"
#include "ExclamationMark.h"

//HpBar
#include "Monster_HpBar.h"

_USING(Client)

CGhost_Mage::CGhost_Mage(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject_Monster(pDevice)
{

}

CGhost_Mage::CGhost_Mage(const CGhost_Mage & rhs)
	: CGameObject_Monster(rhs)
{

}


HRESULT CGhost_Mage::Ready_Prototype()
{
	if (FAILED(CGameObject_Monster::Ready_Prototype()))
		return E_FAIL;


	return S_OK;
}

inline HRESULT CGhost_Mage::Ready_GameObject()
{
	if (FAILED(CGameObject_Monster::Ready_GameObject()))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	Ready_StatsInfo();

	if (FAILED(Ready_HpBar()))
		return E_FAIL;

	for (auto& Collider : m_ColliderList)
		Collider->Set_Parnet_World_Matrix(m_pTransformCom->Get_WorldMatrix());

	//Setup Collider
	CCollision_Manager::GetInstance()->SetUp_MonsterList(this);

	return S_OK;
}

_int CGhost_Mage::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bIsDead)
	{
		m_pMonster_HpBar->Set_IsDead();
		return DEAD_OBJ;
	}


	//Hit Delay
	m_fHitTimeAcc += fTimeDelta;
	

	m_pCurrentState->Update(fTimeDelta);

	return CGameObject_Monster::Update_GameObject(fTimeDelta);
}

_int CGhost_Mage::LastUpdate_GameObject(const _float & fTimeDelta)
{
	m_pCurrentState->LastUpdate(fTimeDelta);

	//HpBar Position Update;
	if (m_pMonster_HpBar != nullptr)
		m_pMonster_HpBar->Set_Position(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION));


	return CGameObject_Monster::LastUpdate_GameObject(fTimeDelta);
}

void CGhost_Mage::Render_GameObject()
{

	m_pCollider_Weapon->Update_Collider_Sphere();

	CGameObject_Monster::Render_GameObject();
}

HRESULT CGhost_Mage::SetUp_GameObjectInfo(const _matrix * pMatrix)
{
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();
	{
		//Set World Matrix
		m_pTransformCom->Set_WorldMatrix(pMatrix);

		if (FAILED(CGameObject_Dynamic::Load_ColliderInfoFromFile(L"../Bin/Resources/Data/Ghost_Mage.col")))
			return E_FAIL;

		if (FAILED(Load_WeaponCollier_FromFile(L"../Bin/Resources/Data/Ghost_Mage_Weapon.col")))
			return E_FAIL;

		//Compute NaviMesh Index;
		if (FAILED(m_pNaviMeshCom->Compute_Position_OnNaviMeshIndex(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION), m_dwCurrentNaviIndex)))
		{
			return E_FAIL;
		}
	}
	Safe_Release(pManagement);
	return S_OK;
}

void CGhost_Mage::Damaged(const _int& iDamage, const _uint& iEffect_Type, const _vec3* pEffect_Position)
{
	
	//맞고 힛딜레이면 그냥 빠져나감
	if (m_fHitTimeAcc < m_fHitDelay|| Compare_State(L"Dead") || Compare_State(L"DownAndUp"))
		return;
	m_fHitTimeAcc = 0.f;

	if (iEffect_Type == EFFECT_NULL)
	{
		//Set Damage Font
		CDamage* pDamage;
		CManagement::GetInstance()->Add_Layer(SCENE_STAGE, L"Layer_Damage", L"GameObject_Damage", (CGameObject**)&pDamage);
		pDamage->SetUp_DamageInfo(iDamage, m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION), CDamage::WHITE);

		m_BaseStats.iCurrentHp -= iDamage;
		m_pMonster_HpBar->Set_HpRadio((float)m_BaseStats.iCurrentHp / m_BaseStats.iMaxHp);

		if (iDamage > 1000)
			Change_State(L"DownAndUp");
		else
			Change_State(L"Hit");
	}

	//Create Effect
	if (iEffect_Type == EFFECT_BLOOD)
	{
		CBlood* pBlood;
		CManagement::GetInstance()->Add_Layer(SCENE_STAGE, L"Layer_Blood", L"GameObject_Blood", (CGameObject**)&pBlood);
		pBlood->Set_Position(pEffect_Position);

		//Set Damage Font
		CDamage* pDamage;
		CManagement::GetInstance()->Add_Layer(SCENE_STAGE, L"Layer_Damage", L"GameObject_Damage", (CGameObject**)&pDamage);
		pDamage->SetUp_DamageInfo(iDamage, m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION), CDamage::WHITE);

		m_BaseStats.iCurrentHp -= iDamage;
		m_pMonster_HpBar->Set_HpRadio((float)m_BaseStats.iCurrentHp / m_BaseStats.iMaxHp);

		wstring cstrFileName = L"Sword_Hit";
		_tchar szInt[MIN_PATH];
		_itow_s(rand() % 6, szInt, 10);

		cstrFileName += szInt;
		cstrFileName += L".mp3";
		CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::MONSTER);
		CSound_Manager::GetInstance()->Play_Sound(cstrFileName.c_str(), CSound_Manager::MONSTER);

		if (iDamage > 1000)
			Change_State(L"DownAndUp");
		else
			Change_State(L"Hit");
	}
	if (iEffect_Type == EFFECT_FIRE)
	{
		CHitFire* pHitFire;
		CManagement::GetInstance()->Add_Layer(SCENE_STAGE, L"Layer_HitFire", L"GameObject_HitFire", (CGameObject**)&pHitFire);
		pHitFire->Set_Position(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION));

		//Set Damage Font
		CDamage* pDamage;
		CManagement::GetInstance()->Add_Layer(SCENE_STAGE, L"Layer_Damage", L"GameObject_Damage", (CGameObject**)&pDamage);
		pDamage->SetUp_DamageInfo(iDamage, m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION), CDamage::WHITE);

		m_BaseStats.iCurrentHp -= iDamage;
		m_pMonster_HpBar->Set_HpRadio((float)m_BaseStats.iCurrentHp / m_BaseStats.iMaxHp);

		Change_State(L"DownAndUp");
	}

	if (iEffect_Type == EFFECT_EXCLAMATIONMARK)
	{
		CExclamationMark* pExclamationMark;
		CManagement::GetInstance()->Add_Layer(SCENE_STAGE, L"Layer_ExclamationMark", L"GameObject_ExclamationMark", (CGameObject**)&pExclamationMark);
		pExclamationMark->Set_Transform(m_pTransformCom);

		Set_TargetPlayerType(PLAYER_LUPS);

		Change_State(L"Idle");
	}
}



HRESULT CGhost_Mage::Ready_Component()
{
	// Set Component
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();
	{
		//For. Mesh
		m_pMeshCom = dynamic_cast<CMesh_Dynamic*>(pManagement->Clone_Component(SCENE_STAGE, L"Component_Mesh_Ghost_Mage"));
		if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
			return E_FAIL;

	}
	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CGhost_Mage::Ready_State()
{
	CState*	pState = CGhost_Mage_Wait::Create(this);
	m_mapState.emplace(L"Wait", pState);

	pState = CGhost_Mage_Ready_Combat::Create(this);
	m_mapState.emplace(L"Ready_Combat", pState);

	pState = CGhost_Mage_Idle::Create(this);
	m_mapState.emplace(L"Idle", pState);

	pState = CGhost_Mage_Chase::Create(this);
	m_mapState.emplace(L"Chase", pState);

	pState = CGhost_Mage_Attack::Create(this);
	m_mapState.emplace(L"Attack", pState);

	pState = CGhost_Mage_Dead::Create(this);
	m_mapState.emplace(L"Dead", pState);

	pState = CGhost_Mage_Hit::Create(this);
	m_mapState.emplace(L"Hit", pState);

	pState = CGhost_Mage_IceBolt::Create(this);
	m_mapState.emplace(L"IceBolt", pState);

	pState = CGhost_Mage_DownAndUp::Create(this);
	m_mapState.emplace(L"DownAndUp", pState);


	Change_State(L"Wait");
	
	return S_OK;
}

void CGhost_Mage::Ready_StatsInfo()
{
	m_BaseStats.iMaxHp = m_BaseStats.iCurrentHp = 7500;
	m_BaseStats.fMoveSpeed = 4.f; 

	m_fAttackRange = 15.f;

	m_Damage.iDamage = 100;
	m_Damage.iDamage_Range = 100;

}
HRESULT CGhost_Mage::Ready_HpBar()
{
	CManagement::GetInstance()->Add_Layer(SCENE_STAGE,
		L"Layer_Monster_HpBar", L"GameObject_Monster_HpBar", (CGameObject**)&m_pMonster_HpBar);
	//m_pMonster_HpBar->AddRef();

	m_pMonster_HpBar->Set_HpRadio(1.f);
	m_pMonster_HpBar->Set_Position(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION));

	return S_OK;
}


CGhost_Mage * CGhost_Mage::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CGhost_Mage*		pInstance = new CGhost_Mage(pDevice);

	if (FAILED(pInstance->Ready_Prototype()))
	{
		_MSG_BOX(L"CGhost_Mage Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CGhost_Mage::Clone_GameObject()
{
	CGhost_Mage*		pInstance = new CGhost_Mage(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX(L"CGhost_Mage Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGhost_Mage::Free()
{

	CGameObject_Monster::Free();
}

