#include "stdafx.h"
#include "Shield.h"

#include "Management.h"
#include "Camera_Dynamic.h"

_USING(Client)

Client::CShield::CShield(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject_Weapon(pDevice)

{

}

Client::CShield::CShield(const CShield& rhs)
	: CGameObject_Weapon(rhs)

{

}

HRESULT Client::CShield::Ready_Prototype()
{

	return S_OK;
}

HRESULT Client::CShield::Ready_GameObject()
{

	if (FAILED(CGameObject_Weapon::Ready_GameObject()))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Set_RotationY(D3DXToRadian(180));


	return S_OK;
}

Engine::_int Client::CShield::Update_GameObject(const _float& fTimeDelta)
{
	if (!m_bIsEquiped)
		return 0;

	if (m_pParentBoneMatrix != nullptr || m_pParentWorldMatrix != nullptr)
	{
		_matrix matParent = (*m_pParentBoneMatrix) * (*m_pParentWorldMatrix);
		m_matCombineWorld = *m_pTransformCom->Get_WorldMatrix() * matParent;
	}

	return CGameObject_Weapon::Update_GameObject(fTimeDelta);
}

Engine::_int Client::CShield::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if (!m_bIsEquiped)
		return 0;

	return CGameObject_Weapon::LastUpdate_GameObject(fTimeDelta);
}

void Client::CShield::Render_GameObject()
{
	if (m_pShaderCom == nullptr ||
		m_pMeshCom == nullptr)
		return;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;

	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->Begin(nullptr, 0);

	pEffect->BeginPass(3);
	{
		m_pMeshCom->Render_Mesh(pEffect);
	}
	pEffect->EndPass();

	pEffect->End();
}

HRESULT Client::CShield::SetUp_ShieldInfo()
{
	// Set Component
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();

	//For. Mesh
	m_pMeshCom = dynamic_cast<CMesh_Static*>(pManagement->Clone_Component(SCENE_STAGE, L"Component_Mesh_Shield"));
	if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;

	////For. Collider
	////Collider Clone Ready is Need Mesh Information 
	//m_pColliderCom = dynamic_cast<CCollider_Sphere*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Collider_Sphere"));
	//if (m_pColliderCom == nullptr)
	//	return E_FAIL;

	//m_pColliderCom->SetUp_Collider_Spehre(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION), 3.f);


	//Parnet Info
	Safe_Release(pManagement);


	//Parent Matrix Setup
	Set_ParentMatrix(L"Layer_Lups", L"Bip01-L-Hand");

	return S_OK;
}

HRESULT Client::CShield::Ready_Component()
{

	return S_OK;
}

HRESULT CShield::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

HRESULT CShield::Set_ParentMatrix(const _tchar * pLayerTag, const _tchar * FrameTag)
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

	return S_OK;
}

Engine::CGameObject* Client::CShield::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CShield*		pInstance = new CShield(pDevice);

	if (FAILED(pInstance->Ready_Prototype()))
	{
		_MSG_BOX(L"CShield Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CGameObject* Client::CShield::Clone_GameObject()
{
	CShield*		pInstance = new CShield(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX(L"CShield Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Client::CShield::Free()
{
	CGameObject_Weapon::Free();
}
