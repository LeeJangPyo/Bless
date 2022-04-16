#include "stdafx.h"
#include "..\Headers\Flogas.h"


#include "Management.h"
#include "Collision_Manager.h"
#include "Sound_Manager.h"

////State
#include "Flogas_Wait.h"
#include "Flogas_Ready_Combat.h"
#include "Flogas_Idle.h"
#include "Flogas_Chase.h"
#include "Flogas_Dead.h"
#include "Flogas_Attack.h"
#include "Flogas_Earthquake.h"
#include "Flogas_FireSpurt.h"
#include "Flogas_Summon.h"
#include "Flogas_Stab.h"

#include "Damage.h"

#include "UI_Frame.h"

#include "Blood.h"



_USING(Client)

CFlogas::CFlogas(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject_Monster(pDevice)
	, m_pUI_HpBar(nullptr)
	, m_fAttackRange(0.f)

{

}

CFlogas::CFlogas(const CFlogas & rhs)
	: CGameObject_Monster(rhs)
	, m_pUI_HpBar(nullptr)
	, m_fAttackRange(0.f)

{

}

void CFlogas::Set_HpBar_Activate(_bool bActivate)
{
	m_pUI_HpBar->Set_Activate(bActivate);
}




HRESULT CFlogas::Ready_Prototype()
{
	if (FAILED(CGameObject_Monster::Ready_Prototype()))
		return E_FAIL;

	return S_OK;
}

inline HRESULT CFlogas::Ready_GameObject()
{
	if (FAILED(CGameObject_Monster::Ready_GameObject()))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	Ready_Stats();

	for (auto& Collider : m_ColliderList)
		Collider->Set_Parnet_World_Matrix(m_pTransformCom->Get_WorldMatrix());

	//Setup Collider
	CCollision_Manager::GetInstance()->SetUp_MonsterList(this);


	////Get UI HpBar;
	m_pUI_HpBar = dynamic_cast<CUI_Frame*>(CManagement::GetInstance()->Get_GameObject(SCENE_STAGE, L"Layer_UI_Flogas"));
	if (m_pUI_HpBar == nullptr)
		return E_FAIL;
	
	return S_OK;
}

_int CFlogas::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bIsDead)
	{
		m_pUI_HpBar->Set_Activate(false);
		return DEAD_OBJ;
	}

	//Hit Delay
	m_fHitTimeAcc += fTimeDelta;
	if (m_fHitTimeAcc > m_fHitDelay)
		Set_RimLight(g_fDefaultRimLight);
	
	m_pCurrentState->Update(fTimeDelta);

	return CGameObject_Monster::Update_GameObject(fTimeDelta);
}

_int CFlogas::LastUpdate_GameObject(const _float & fTimeDelta)
{
	m_pCurrentState->LastUpdate(fTimeDelta);

	return CGameObject_Monster::LastUpdate_GameObject(fTimeDelta);
}

void CFlogas::Render_GameObject()
{
//	m_pCollider_Weapon->Update_Collider_Sphere();

	CGameObject_Monster::Render_GameObject();

}

HRESULT CFlogas::SetUp_GameObjectInfo(const _matrix * pMatrix)
{
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();
	{
		//Set World Matrix
		m_pTransformCom->Set_WorldMatrix(pMatrix);

		if (FAILED(CGameObject_Dynamic::Load_ColliderInfoFromFile(L"../Bin/Resources/Data/Flogas.col")))
			return E_FAIL;

		if (FAILED(Load_WeaponCollier_FromFile(L"../Bin/Resources/Data/Flogas_Weapon.col")))
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

void CFlogas::Damaged(const _int& iDamage, const _uint& iEffect_Type, const _vec3* pEffect_Position)
{
	//맞고 힛딜레이면 그냥 빠져나감
	if (m_fHitTimeAcc < m_fHitDelay)
	{
		return;
	}

	m_fHitTimeAcc = 0.f;
	Set_RimLight(1.f);

	//Create Effect
	if (iEffect_Type == EFFECT_BLOOD)
	{
		CBlood* pBlood;
		CManagement::GetInstance()->Add_Layer(SCENE_STAGE, L"Layer_Blood", L"GameObject_Blood", (CGameObject**)&pBlood);
		pBlood->Set_Position(pEffect_Position);

		//Set Damage Font

		wstring cstrFileName = L"Sword_Hit";
		_tchar szInt[MIN_PATH];
		_itow_s(rand() % 6, szInt, 10);

		cstrFileName += szInt;
		cstrFileName += L".mp3";
		CSound_Manager::GetInstance()->Stop_Sound(CSound_Manager::MONSTER);
		CSound_Manager::GetInstance()->Play_Sound(cstrFileName.c_str(), CSound_Manager::MONSTER);
	}

	_vec3 vDamagePos = *m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
	vDamagePos.y += 3.f;

	CDamage* pDamage;
	CManagement::GetInstance()->Add_Layer(SCENE_STAGE, L"Layer_Damage", L"GameObject_Damage", (CGameObject**)&pDamage);
	pDamage->SetUp_DamageInfo(iDamage, m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION), CDamage::WHITE);

	m_BaseStats.iCurrentHp -= iDamage;
	m_pUI_HpBar->Set_Radio((_float)m_BaseStats.iCurrentHp / m_BaseStats.iMaxHp, L"BossMask");
}

HRESULT CFlogas::Ready_Component()
{
	// Set Component
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();
	{
		//For. Mesh
		m_pMeshCom = dynamic_cast<CMesh_Dynamic*>(pManagement->Clone_Component(SCENE_STAGE, L"Component_Mesh_Flogas"));
		if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
			return E_FAIL;

	}
	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CFlogas::Ready_State()
{
	CState*	pState = CFlogas_Wait::Create(this);
	m_mapState.emplace(L"Wait", pState);

	pState = CFlogas_Ready_Combat::Create(this);
	m_mapState.emplace(L"Ready_Combat", pState);

	pState = CFlogas_Idle::Create(this);
	m_mapState.emplace(L"Idle", pState);

	pState = CFlogas_Chase::Create(this);
	m_mapState.emplace(L"Chase", pState);
	
	pState = CFlogas_Dead::Create(this);
	m_mapState.emplace(L"Dead", pState);

	pState = CFlogas_Attack::Create(this);
	m_mapState.emplace(L"Attack", pState);

	pState = CFlogas_Earthquake::Create(this);
	m_mapState.emplace(L"Earthquake", pState);

	pState = CFlogas_FireSpurt::Create(this);
	m_mapState.emplace(L"FireSpurt", pState);

	pState = CFlogas_Summon::Create(this);
	m_mapState.emplace(L"Summon", pState);

	pState = CFlogas_Stab::Create(this);
	m_mapState.emplace(L"Stab", pState);

	Change_State(L"Wait");

	return S_OK;
}

void CFlogas::Ready_Stats()
{
	m_BaseStats.iMaxHp = m_BaseStats.iCurrentHp = 25000;
	m_BaseStats.fMoveSpeed = 5.f;

	m_fAttackRange = 6.5f;


	m_Damage.iDamage = 500;
	m_Damage.iDamage_Range = 250;
}


CFlogas * CFlogas::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CFlogas*		pInstance = new CFlogas(pDevice);

	if (FAILED(pInstance->Ready_Prototype()))
	{
		_MSG_BOX(L"CFlogas Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CFlogas::Clone_GameObject()
{
	CFlogas*		pInstance = new CFlogas(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX(L"CFlogas Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFlogas::Free()
{
	Safe_Release(m_pCollider_Weapon);

	CGameObject_Monster::Free();
}

