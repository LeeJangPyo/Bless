#include "stdafx.h"
#include "..\Headers\IceBolt.h"

#include "Management.h"
#include "Collision_Manager.h"


#include "EffectGroup.h"
#include "Sound_Manager.h"

_USING(Client)

CIceBolt::CIceBolt(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
	, m_pTransformCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pMeshCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pShaderCom(nullptr)
	, m_pColliderCom(nullptr)
	, m_fScale(0.25f)
	, m_fMoveSpeed(10.f)
	, m_fLifeTime(0.f)
	, m_fTimeAcc(0.f)
{
	ZeroMemory(&m_Damage, sizeof(m_Damage));
}

CIceBolt::CIceBolt(const CIceBolt & rhs)
	:CGameObject(rhs)
	, m_pColliderCom(nullptr)
	, m_fScale(rhs.m_fScale)
	, m_fMoveSpeed(rhs.m_fMoveSpeed)
	, m_Damage(rhs.m_Damage)
	, m_fLifeTime(0.f)
	, m_fTimeAcc(0.f)
{
}


//void CIceBolt::Set_TargetPosition(const _vec3 * pTargetPosition)
//{
//	//m_vTargetPosition = *pTargetPosition;
//}

//void CIceBolt::Set_Position(const _vec3 * pPosition)
//{
//	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, pPosition);
//
//}

HRESULT CIceBolt::Ready_Prototype()
{
	m_Damage.iDamage = 300;
	m_Damage.iDamage_Range = 100;

	return S_OK;
}

HRESULT CIceBolt::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;



	m_pTransformCom->Set_ScaleInfo(m_fScale, m_fScale, m_fScale);

	m_fLifeTime = 10.f;

	m_fTimeAcc = 0.f;

	CManagement::GetInstance()->Add_Layer(SCENE_STAGE, L"Layer_Effect_Ghost_Mage", L"GameObject_Effect_Ice_Particle",
		(CGameObject**)&m_pEffect_IceParticle);
	if (m_pEffect_IceParticle == nullptr)
		return E_FAIL;



	return S_OK;
}

_int CIceBolt::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bIsDead)
	{
		m_pEffect_IceParticle->Set_Position(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION));
		m_pEffect_IceParticle->Set_DeadObject(true);
		m_pEffect_IceParticle->Set_Activate(true);
		return DEAD_OBJ;
	}

	m_fTimeAcc += fTimeDelta;
	if (m_fTimeAcc > m_fLifeTime)
	{
		m_bIsDead = true;
		return DEAD_OBJ;
	}


	m_pTransformCom->Go_Direction(&m_vTargetDirection, m_fMoveSpeed, fTimeDelta);
	m_pColliderCom->Set_Center_Position(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION));


	return 0;
}

_int CIceBolt::LastUpdate_GameObject(const _float & fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONEALPHA, this);

	if(CCollision_Manager::GetInstance()->
		Check_CollisionAndHit(COL_PLAYER, m_pColliderCom, m_Damage.iDamage + rand()%m_Damage.iDamage_Range))
	{
		m_bIsDead = true;
	}

	return 0;
}

void CIceBolt::Render_GameObject()
{
	if (m_pMeshCom == nullptr ||
		m_pShaderCom ==nullptr)
		return;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;

	pEffect->AddRef();


	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(1);

	if (FAILED(SetUp_ConstantTable(pEffect)))
	{
		Safe_Release(pEffect);
		return;
	}

	pEffect->CommitChanges();

	m_pMeshCom->Render_Mesh();

	pEffect->EndPass();

	//if(g_bDebug)
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

HRESULT CIceBolt::Ready_Component()
{
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();
	{
	// For. Static Component
	m_pRendererCom = dynamic_cast<CRenderer*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Renderer"));
	if (FAILED(Add_Component(L"Com_Renderer", m_pRendererCom)))
		return E_FAIL;

	m_pTransformCom = dynamic_cast<CTransform*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Transform"));
	if (FAILED(Add_Component(L"Com_Transform", m_pTransformCom)))
		return E_FAIL;


	//////////////////////////////////////////////////////////////////////////
	// Stage Component
	m_pShaderCom = dynamic_cast<CShader*>(pManagement->Clone_Component(SCENE_STAGE, L"Component_Shader_Effect"));
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;

	m_pMeshCom = dynamic_cast<CMesh_Static*>(pManagement->Clone_Component(SCENE_STAGE, L"Component_Mesh_IceBolt"));
	if (FAILED(Add_Component(L"Com_Mesh", m_pMeshCom)))
		return E_FAIL;

	m_pTextureCom = dynamic_cast<CTexture*>(pManagement->Clone_Component(SCENE_STAGE, L"Component_Texture_FX_ICE_001_TEX_PHS"));
	if (FAILED(Add_Component(L"Com_Texture", m_pMeshCom)))
		return E_FAIL;

	//Collier
	m_pColliderCom = dynamic_cast<CCollider_Sphere*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Collider_Sphere"));
	if (FAILED(Add_Component(L"Com_Collider", m_pColliderCom)))
		return E_FAIL;
	}
	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CIceBolt::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (pEffect == nullptr)
		return E_FAIL;

	pEffect->AddRef();

	if (FAILED(m_pTextureCom->SetUp_OnShader(pEffect, "g_AlphaTexture")))
		return E_FAIL;

	if (FAILED(m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld")))
		return E_FAIL;

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

HRESULT CIceBolt::SetUp_IceBoltInfo(const _vec3 * pTargetPosition, const _vec3 * pPosition)
{
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, pPosition);

	m_vTargetDirection = *pTargetPosition - *pPosition;
	m_vTargetDirection.y = 0.f;
	D3DXVec3Normalize(&m_vTargetDirection, &m_vTargetDirection);
	m_pTransformCom->LookAt_Target(pTargetPosition, 1.f, &_vec3(1.f, 0.f, 0.f));



	_byte*	vertices;
	LPD3DXMESH pMesh = m_pMeshCom->Get_Mesh(); 
	pMesh->LockVertexBuffer(0, (void**)&vertices);

	_float fRadius;
	_vec3 vCenter;
	if (FAILED(D3DXComputeBoundingSphere((_vec3*)vertices, 
											pMesh->GetNumVertices(),
											D3DXGetFVFVertexSize(pMesh->GetFVF()),
											&vCenter,
											&fRadius)))
		return E_FAIL;


	m_pColliderCom->SetUp_Collider_Spehre(&vCenter, fRadius * m_fScale);



	return S_OK;
}

CGameObject * CIceBolt::Clone_GameObject()
{
	CIceBolt*		pInstance = new CIceBolt(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX(L"CIceBolt Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CIceBolt * CIceBolt::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CIceBolt*		pInstance = new CIceBolt(pDevice);

	if (FAILED(pInstance->Ready_Prototype()))
	{
		_MSG_BOX(L"CIceBolt Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIceBolt::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pColliderCom);

	CGameObject::Free();
}
