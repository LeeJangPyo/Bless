#include "stdafx.h"
#include "Staff.h"


#include "Management.h"
#include "Collision_Manager.h"

#include "Camera_Dynamic.h"
#include "GameObject_Dynamic.h"

#include "Sire.h"



_USING(Client)

Client::CStaff::CStaff(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject_Weapon(pDevice)
{
	
}

Client::CStaff::CStaff(const CStaff& rhs)
	: CGameObject_Weapon(rhs)
{

}

HRESULT Client::CStaff::Ready_Prototype()
{

	return S_OK;
}

HRESULT Client::CStaff::Ready_GameObject()
{

	if (FAILED(CGameObject_Weapon::Ready_GameObject()))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_bIsEquiped = false;


	m_pSire = dynamic_cast<CSire*>(CManagement::GetInstance()->Get_GameObject(SCENE_STAGE, L"Layer_Sire"));
	if (m_pSire == nullptr)
		return E_FAIL;

	return S_OK;
}

Engine::_int Client::CStaff::Update_GameObject(const _float& fTimeDelta)
{
	if (!m_bIsEquiped)
		return 0;

	if (m_pParentBoneMatrix != nullptr || m_pParentWorldMatrix != nullptr)
	{
		_matrix matParent = (*m_pParentBoneMatrix) * (*m_pParentWorldMatrix);
		m_matCombineWorld = *m_pTransformCom->Get_WorldMatrix() * matParent;
	}


	//m_pColliderCom->Update_Collider_Sphere();


	////Collision Check
	//if(m_pS->Compare_State(L"Attack"))
	//	CCollision_Manager::GetInstance()->Check_CollisionAndHit(CCollision_Manager::COL_MONSTER, m_pColliderCom);

	return CGameObject_Weapon::Update_GameObject(fTimeDelta);
}

Engine::_int Client::CStaff::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if (!m_bIsEquiped)
		return 0;

	return CGameObject_Weapon::LastUpdate_GameObject(fTimeDelta);
}

void Client::CStaff::Render_GameObject()
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
	pEffect->BeginPass(3);
	{
		m_pMeshCom->Render_Mesh(pEffect);
	}
	pEffect->EndPass();
	pEffect->End();


	Safe_Release(pEffect);
}

HRESULT Client::CStaff::SetUp_StaffInfo()
{
	// Set Component
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();
	{
		//For. Mesh
		m_pMeshCom = dynamic_cast<CMesh_Static*>(pManagement->Clone_Component(SCENE_STAGE, L"Component_Mesh_Staff"));
		if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
			return E_FAIL;
	}
	Safe_Release(pManagement);


	//Parent Matrix Setup
	Set_ParentMatrix(L"Layer_Sire", L"Bip01-R-Finger11");

	return S_OK;
}

HRESULT Client::CStaff::Ready_Component()
{
	// Set Component
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();
	{

	
	////For. Collider
	//m_pColliderCom = dynamic_cast<CCollider_Sphere*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Collider_Sphere"));
	//if (m_pColliderCom == nullptr)
	//	return E_FAIL;
	//Parnet Info
	}
	Safe_Release(pManagement);
	return S_OK;
}



HRESULT CStaff::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (pEffect == nullptr)
		return E_FAIL;

	pEffect->AddRef();
	{

	
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

	}
	Safe_Release(pEffect);

	return S_OK;

}

HRESULT CStaff::Set_ParentMatrix(const _tchar * pLayerTag, const _tchar * FrameTag)
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
	//m_pColliderCom->SetUp_Collider_Spehre(&pFrame->CombinedTransformMatrix, pPlayerTransCom->Get_WorldMatrix(), &_vec3(0.f, 10.f, 0.f), 4.f);

	return S_OK;
}

Engine::CGameObject* Client::CStaff::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CStaff*		pInstance = new CStaff(pDevice);

	if (FAILED(pInstance->Ready_Prototype()))
	{
		_MSG_BOX(L"CStaff Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CGameObject* Client::CStaff::Clone_GameObject()
{
	CStaff*		pInstance = new CStaff(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX(L"CStaff Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Client::CStaff::Free()
{
//	Safe_Release(m_pColliderCom);

	CGameObject_Weapon::Free();
}
