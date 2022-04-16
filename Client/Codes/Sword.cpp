#include "stdafx.h"
#include "Sword.h"


#include "Management.h"
#include "Collision_Manager.h"

#include "Camera_Dynamic.h"
#include "GameObject_Dynamic.h"

#include "Lups.h"

#include "Trail.h"

_USING(Client)

Client::CSword::CSword(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject_Weapon(pDevice)
	, m_pColliderCom(nullptr)
	, m_pLupsDamage(nullptr)
	, m_fTimeAcc(0.f)

{
	
}

Client::CSword::CSword(const CSword& rhs)
	: CGameObject_Weapon(rhs)
	, m_pColliderCom(nullptr)
	, m_pLupsDamage(nullptr)
	, m_fTimeAcc(0.f)
{

}

HRESULT Client::CSword::Ready_Prototype()
{

	return S_OK;
}

HRESULT Client::CSword::Ready_GameObject()
{

	if (FAILED(CGameObject_Weapon::Ready_GameObject()))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_bIsEquiped = false;


	m_pLups = dynamic_cast<CLups*>(CManagement::GetInstance()->Get_GameObject(SCENE_STAGE, L"Layer_Lups"));
	if (m_pLups == nullptr)
		return E_FAIL;
	
	m_pLupsDamage = m_pLups->Get_Damage();
	
	//Create Trail;
	if (FAILED(CManagement::GetInstance()->Add_Layer(SCENE_STAGE, L"Layer_Trail", L"GameObject_Trail",
		(CGameObject**)&m_pTrail)))
		return E_FAIL;

	return S_OK;
}

Engine::_int Client::CSword::Update_GameObject(const _float& fTimeDelta)
{
	if (!m_bIsEquiped)
		return 0;


	if (m_pParentBoneMatrix != nullptr || m_pParentWorldMatrix != nullptr)
	{
		_matrix matParent = (*m_pParentBoneMatrix) * (*m_pParentWorldMatrix);
		m_matCombineWorld = *m_pTransformCom->Get_WorldMatrix() * matParent;
		//Near Position
		m_vTrail_NearPosition = *(_vec3*)&m_matCombineWorld.m[3][0];
	}

	m_pColliderCom->Update_Collider_Sphere();
	m_vTrail_FarPosition = *m_pColliderCom->Get_Center();


	////Collision Check
	//if(m_pLups->Compare_State(L"Attack"))
	//	CCollision_Manager::GetInstance()->
	//	Check_CollisionAndHit(COL_MONSTER, m_pColliderCom, m_pLupsDamage->iDamage + rand()%m_pLupsDamage->iDamage_Range);

	return CGameObject_Weapon::Update_GameObject(fTimeDelta);
}

Engine::_int Client::CSword::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if (!m_bIsEquiped)
		return 0;

	return CGameObject_Weapon::LastUpdate_GameObject(fTimeDelta);
}

void Client::CSword::Render_GameObject()
{
	if (m_pShaderCom == nullptr ||
		m_pMeshCom == nullptr)
		return;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;

	pEffect->AddRef();

	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->Begin(nullptr, 0);
	

	m_pMeshCom->Render_Mesh(pEffect);

	//////////////////////////////////////////////////////////////////////////

	//if (g_bDebug)
	//{

	//	pEffect->BeginPass(1);
	//	m_pColliderCom->SetUp_ConstantTable(pEffect, CCollider_Sphere::COLOR_GREEN);
	//	pEffect->CommitChanges();

	//	m_pColliderCom->Render_Collider_Sphere();

	//	pEffect->EndPass();
	//}

	pEffect->End();

	Safe_Release(pEffect);

}

HRESULT Client::CSword::SetUp_SworldInfo()
{
	// Set Component
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();
	{
		//For. Mesh
		m_pMeshCom = dynamic_cast<CMesh_Static*>(pManagement->Clone_Component(SCENE_STAGE, L"Component_Mesh_Sword"));
		if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
			return E_FAIL;
	}
	Safe_Release(pManagement);


	//Parent Matrix Setup
	Set_ParentMatrix(L"Layer_Lups", L"Bip01-R-Finger21");

	return S_OK;
}

void CSword::Begin_Trail()
{
	m_pTrail->Begin_Trail(this);
}

void CSword::End_Trail()
{
	m_pTrail->End_Trail();
}


HRESULT Client::CSword::Ready_Component()
{
	// Set Component
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();

	//For. Collider
	m_pColliderCom = dynamic_cast<CCollider_Sphere*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Collider_Sphere"));
	if (m_pColliderCom == nullptr)
		return E_FAIL;

	
	//Parnet Info
	Safe_Release(pManagement);
	return S_OK;
}



HRESULT CSword::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (pEffect == nullptr)
		return E_FAIL;

	pEffect->AddRef();

	pEffect->SetMatrix("g_matWorld", &m_matCombineWorld);

	CCamera* pCamera = CManagement::GetInstance()->Get_CameraMain();
	if (pCamera == nullptr)
	{
		Safe_Release(pEffect);
		return E_FAIL;
	}
	pCamera->SetUp_OnShader_View(pEffect, "g_matView");
	pCamera->SetUp_OnShader_Projection(pEffect, "g_matProj");
	pCamera->SetUp_OnShader_FarPlane(pEffect, "g_fFarPlane");

	Safe_Release(pEffect);

	return S_OK;

}

HRESULT CSword::Set_ParentMatrix(const _tchar * pLayerTag, const _tchar * FrameTag)
{
	CMesh_Dynamic*	pPlayerMeshCom = dynamic_cast<CMesh_Dynamic*>(CManagement::GetInstance()->Get_Component(SCENE_STAGE, pLayerTag, L"Com_Mesh"));
	if (pPlayerMeshCom == nullptr)
		return E_FAIL;

	_char	szFrameTag[MIN_PATH];

	WideCharToMultiByte(CP_ACP, 0, FrameTag, -1, szFrameTag, MIN_PATH, NULL, NULL);

	const Engine::D3DXFRAME_DERIVED*		pFrame = pPlayerMeshCom->Get_FrameByName(szFrameTag);

	m_pParentBoneMatrix = &pFrame->CombinedTransformMatrix;

	CTransform*		pPlayerTransCom = dynamic_cast<CTransform*>(CManagement::GetInstance()->Get_Component(SCENE_STAGE, pLayerTag, L"Com_Transform"));
	if (pPlayerTransCom == nullptr)
		return E_FAIL;

	m_pParentWorldMatrix = pPlayerTransCom->Get_WorldMatrix();

	//Collider
	m_pColliderCom->SetUp_Collider_Spehre(&pFrame->CombinedTransformMatrix,
		pPlayerTransCom->Get_WorldMatrix(), &_vec3(0.f, 12.5f, 0.f), 10.f);
	
	return S_OK;
}

Engine::CGameObject* Client::CSword::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CSword*		pInstance = new CSword(pDevice);

	if (FAILED(pInstance->Ready_Prototype()))
	{
		_MSG_BOX(L"CSword Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CGameObject* Client::CSword::Clone_GameObject()
{
	CSword*		pInstance = new CSword(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX(L"CSword Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Client::CSword::Free()
{
	Safe_Release(m_pColliderCom);

	CGameObject_Weapon::Free();
}
