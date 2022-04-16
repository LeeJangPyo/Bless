#include "stdafx.h"
#include "..\Headers\Balrog.h"


#include "Management.h"
#include "Collision_Manager.h"

//State
#include "Balrog_Wait.h"
#include "Balrog_Ready_Combat.h"
#include "Balrog_Idle.h"
#include "Balrog_Chase.h"
#include "Balrog_Dead.h"
#include "Balrog_Breath.h"
#include "Balrog_Electric_Whip.h"
#include "Balrog_Fire_Ring.h"
#include "Balrog_Kick.h"

#include "Damage.h"




_USING(Client)

CBalrog::CBalrog(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject_Dynamic(pDevice)
{

}

CBalrog::CBalrog(const CBalrog & rhs)
	: CGameObject_Dynamic(rhs)
{

}


HRESULT CBalrog::Ready_Prototype()
{
	if (FAILED(CGameObject_Dynamic::Ready_Prototype()))
		return E_FAIL;

	return S_OK;
}

inline HRESULT CBalrog::Ready_GameObject()
{
	if (FAILED(CGameObject_Dynamic::Ready_GameObject()))
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


	m_fAttackRange = 10.f;


	return S_OK;
}

_int CBalrog::Update_GameObject(const _float & fTimeDelta)
{

	for (auto& Collider : m_ColliderList)
	{
		Collider->Update_Collider_Sphere();
	}

	m_pCurrentState->Update(fTimeDelta);

	return 0;
}

_int CBalrog::LastUpdate_GameObject(const _float & fTimeDelta)
{
	m_pCurrentState->LastUpdate(fTimeDelta);

	return CGameObject_Dynamic::LastUpdate_GameObject(fTimeDelta);
}

void CBalrog::Render_GameObject()
{
	//Animation Update
	//m_pMeshCom->Play_AnimationSet(m_fTimeDelta);

	////Collider Update
	//for (auto& Collider : m_ColliderList)
	//	Collider->Update_Collider_Sphere();


	//if (nullptr == m_pMeshCom ||
	//	nullptr == m_pShaderCom)
	//	return;

	//LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	//if (pEffect == nullptr)
	//	return;

	//pEffect->AddRef();


	//if (FAILED(SetUp_ConstantTable(pEffect)))
	//{
	//	Safe_Release(pEffect);
	//	return;
	//}

	//pEffect->Begin(nullptr, 0);
	//pEffect->BeginPass(0);

	//m_pMeshCom->Render_Mesh();

	//pEffect->EndPass();

	//////////////////////////////////////////////////////////////////////////


	//if (g_bDebug)
	//{
	//	pEffect->BeginPass(1);

	//	//Hit
	//	for (auto& Collider : m_ColliderList)
	//	{
	//		Collider->SetUp_ConstantTable(pEffect, CCollider_Sphere::COLOR_GREEN);

	//		pEffect->CommitChanges();
	//		Collider->Render_Collider_Sphere();
	//	}

	//	pEffect->EndPass();
	//}


	//pEffect->End();

	//Safe_Release(pEffect);

}

HRESULT CBalrog::SetUp_GameObjectInfo(const _matrix * pMatrix)
{
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();
	{
		//Set World Matrix
		m_pTransformCom->Set_WorldMatrix(pMatrix);

		if (FAILED(CGameObject_Dynamic::Load_ColliderInfoFromFile(L"../Bin/Resources/Data/Balrog.col")))
			return E_FAIL;

		//Compute NaviMesh Index;
		if (FAILED(m_pNaviMeshCom->Compute_Position_OnNaviMeshIndex(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION), m_dwCurrentNaviIndex)))
		{
			return E_FAIL;
		}

		Safe_Release(pManagement);
	}
	return S_OK;
}

void CBalrog::Damaged(const _int& iDamage, const _uint& iEffect_Type, const _vec3* pEffect_Position)
{
	if (Compare_State(L"Hit"))
		return;

	Change_State(L"Hit");

	CDamage* pDamage;
	CManagement::GetInstance()->Add_Layer(SCENE_STAGE, L"Layer_Damage", L"GameObject_Damage", (CGameObject**)&pDamage);
	pDamage->SetUp_DamageInfo(iDamage, m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION),CDamage::WHITE);
}


HRESULT CBalrog::Ready_Component()
{
	// Set Component
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();
	{
		//For. Mesh
		m_pMeshCom = dynamic_cast<CMesh_Dynamic*>(pManagement->Clone_Component(SCENE_STAGE, L"Component_Mesh_Balrog"));
		if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
			return E_FAIL;

	}
	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CBalrog::Ready_State()
{
	CState*	pState = CBalrog_Wait::Create(this);
	m_mapState.emplace(L"Wait", pState);

	pState = CBalrog_Ready_Combat::Create(this);
	m_mapState.emplace(L"CutScene", pState);

	pState = CBalrog_Idle::Create(this);
	m_mapState.emplace(L"Idle", pState);

	pState = CBalrog_Chase::Create(this);
	m_mapState.emplace(L"Chase", pState);
	
	pState = CBalrog_Dead::Create(this);
	m_mapState.emplace(L"Dead", pState);

	pState = CBalrog_Electric_Whip::Create(this);
	m_mapState.emplace(L"Electric_Whip", pState);

	pState = CBalrog_Breath::Create(this);
	m_mapState.emplace(L"Breath", pState);

	pState = CBalrog_Fire_Ring::Create(this);
	m_mapState.emplace(L"Fire_Ring", pState);

	pState = CBalrog_Kick::Create(this);
	m_mapState.emplace(L"Kick", pState);

	Change_State(L"Idle");

	return S_OK;
}

void CBalrog::Ready_Stats()
{
	m_BaseStats.iMaxHp = m_BaseStats.iCurrentHp = 10000;
	m_BaseStats.fMoveSpeed = 10.f;
}


CBalrog * CBalrog::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBalrog*		pInstance = new CBalrog(pDevice);

	if (FAILED(pInstance->Ready_Prototype()))
	{
		_MSG_BOX(L"CBalrog Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBalrog::Clone_GameObject()
{
	CBalrog*		pInstance = new CBalrog(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX(L"CBalrog Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBalrog::Free()
{
	CGameObject_Dynamic::Free();
}

