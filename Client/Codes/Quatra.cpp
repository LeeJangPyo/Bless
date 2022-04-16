#include "stdafx.h"
#include "..\Headers\Quatra.h"


#include "Management.h"
#include "Collision_Manager.h"

////State
#include "Quatra_Wait.h"
#include "Quatra_Ready_Combat.h"
#include "Quatra_Breath.h"
#include "Quatra_Breath_Start.h"
#include "Quatra_Breath_End.h"



_USING(Client)

CQuatra::CQuatra(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject_Monster(pDevice)

{

}

CQuatra::CQuatra(const CQuatra & rhs)
	: CGameObject_Monster(rhs)

{

}

HRESULT CQuatra::Ready_Prototype()
{
	if (FAILED(CGameObject_Monster::Ready_Prototype()))
		return E_FAIL;

	return S_OK;
}

inline HRESULT CQuatra::Ready_GameObject()
{
	if (FAILED(CGameObject_Monster::Ready_GameObject()))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(30.f, 0.f, 10.f));
	m_pTransformCom->Set_ScaleInfo(0.1f,0.1f,0.1f);


	return S_OK;
}

_int CQuatra::Update_GameObject(const _float & fTimeDelta)
{

	m_pCurrentState->Update(fTimeDelta);

	return CGameObject_Monster::Update_GameObject(fTimeDelta);
}

_int CQuatra::LastUpdate_GameObject(const _float & fTimeDelta)
{
	m_pCurrentState->LastUpdate(fTimeDelta);

	return CGameObject_Monster::LastUpdate_GameObject(fTimeDelta);
}

void CQuatra::Render_GameObject()
{
	CGameObject_Monster::Render_GameObject();
}

HRESULT CQuatra::SetUp_GameObjectInfo(const _matrix * pMatrix)
{
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();
	{
		//Set World Matrix
		m_pTransformCom->Set_WorldMatrix(pMatrix);

	}
	Safe_Release(pManagement);
	return S_OK;
}

void CQuatra::Damaged(const _int& iDamage, const _uint& iEffect_Type, const _vec3* vEffect_Position)
{
	//맞고 힛딜레이면 그냥 빠져나감
	if (m_fHitTimeAcc < m_fHitDelay)
		return;

	m_fHitTimeAcc = 0.f;

	//if (Compare_State(L"Hit"))
	//	return;

	//Change_State(L"Hit");

}

HRESULT CQuatra::Ready_Component()
{
	// Set Component
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();
	{
		//For. Mesh
		m_pMeshCom = dynamic_cast<CMesh_Dynamic*>(pManagement->Clone_Component(SCENE_STAGE, L"Component_Mesh_Quatra"));
		if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
			return E_FAIL;

	}
	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CQuatra::Ready_State()
{
	CState*	pState = CQuatra_Wait::Create(this);
	m_mapState.emplace(L"Wait", pState);

	pState = CQuatra_Ready_Combat::Create(this);
	m_mapState.emplace(L"Ready_Combat", pState);

	pState = CQuatra_Breath::Create(this);
	m_mapState.emplace(L"Breath", pState);

	pState = CQuatra_Breath_Start::Create(this);
	m_mapState.emplace(L"Breath_Start", pState);
	
	pState = CQuatra_Breath_End::Create(this);
	m_mapState.emplace(L"Breath_End", pState);

	Change_State(L"Wait");

	return S_OK;
}


CQuatra * CQuatra::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CQuatra*		pInstance = new CQuatra(pDevice);

	if (FAILED(pInstance->Ready_Prototype()))
	{
		_MSG_BOX(L"CQuatra Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CQuatra::Clone_GameObject()
{
	CQuatra*		pInstance = new CQuatra(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX(L"CQuatra Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQuatra::Free()
{
	CGameObject_Monster::Free();
}

