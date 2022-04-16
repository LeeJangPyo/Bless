#include "stdafx.h"
#include "..\Headers\EffectGroup.h"

#include "Management.h"
#include "Collision_Manager.h"

_USING(Client)

CEffectGroup::CEffectGroup(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
	, m_pTransformCom(nullptr)
	, m_pShaderCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_bActivate(false)
	, m_bMove(false)
	, m_fTimeAcc(0.f)
	, m_pColliderCom(nullptr)
	, m_bDeadObject(false)
	, m_iMeshEffectNum(0)
	, m_iRectEffectNum(0)
	, m_iParticleEffectNum(0)
	, m_iEffecType(0)
	, m_bHitOnce(false)
	, m_fCollision_StartTime(0.f)
	, m_fCollision_EndTime(0.f)

{
	ZeroMemory(&m_Damage, sizeof(DAMAGE));
	D3DXMatrixIdentity(&m_matParentMatrix);
}

CEffectGroup::CEffectGroup(const CEffectGroup & rhs)
	:CGameObject(rhs)
	, m_pTransformCom(nullptr)
	, m_pShaderCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_bActivate(false)
	, m_bMove(false)
	, m_fTimeAcc(0.f)
	, m_pColliderCom(nullptr)
	, m_bDeadObject(false)
	, m_iMeshEffectNum(rhs.m_iMeshEffectNum)
	, m_iRectEffectNum(rhs.m_iRectEffectNum)
	, m_iParticleEffectNum(rhs.m_iParticleEffectNum)
	, m_EffectrGroup_Data(rhs.m_EffectrGroup_Data)
	, m_iEffecType(0)
	, m_bHitOnce(false)
	, m_Damage(rhs.m_Damage)
{
	D3DXMatrixIdentity(&m_matParentMatrix);
	m_vecMeshData.assign(rhs.m_vecMeshData.begin(), rhs.m_vecMeshData.end());
	m_vecRectData.assign(rhs.m_vecRectData.begin(), rhs.m_vecRectData.end());
	m_vecParticleData.assign(rhs.m_vecParticleData.begin(), rhs.m_vecParticleData.end());

}

HRESULT CEffectGroup::Ready_Prototype()
{
	return S_OK;
}

HRESULT CEffectGroup::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	for (_int i = 0; i < m_iMeshEffectNum; ++i)
	{

		CEffect_Mesh* pEffectMesh = CEffect_Mesh::Create(m_pDevice);
		pEffectMesh->SetUp_EffectData(m_vecMeshData[i].Effect_Data, m_vecMeshData[i].Effect_MeshData);
		m_vecEffectMesh.push_back(pEffectMesh);
	}

	for (_int i = 0; i < m_iRectEffectNum; ++i)
	{
		CEffect_Rect* pEffectRect = CEffect_Rect::Create(m_pDevice);
		pEffectRect->SetUp_EffectData(m_vecRectData[i].Effect_Data, m_vecRectData[i].Effect_RectData);
		m_vecEffectRect.push_back(pEffectRect);
	}

	for (_int i = 0; i < m_iParticleEffectNum; ++i)
	{
		CEffect_Particle* pEffectParticle = CEffect_Particle::Create(m_pDevice);
		pEffectParticle->SetUp_ParticleData(m_vecParticleData[i].Effect_Data, m_vecParticleData[i].Effect_ParticleData,m_vecParticleData[i].bReCycle);
		m_vecEffectParticle.push_back(pEffectParticle);
	}

	return S_OK;
}

_int CEffectGroup::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bIsDead)
		return DEAD_OBJ;

	if (!m_bActivate)
		return 0;

	//Update Collider
	if (m_pColliderCom != nullptr)
	{
		m_pColliderCom->Update_Collider_Sphere();
	}

	//Update Move
	if(m_bMove)
	{
		m_pTransformCom->Go_Direction(&m_vDirection, m_fMoveSpeed, fTimeDelta);

		if (m_pColliderCom != nullptr)
			m_pColliderCom->Set_Center_Position(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION));
	}

	if (m_pParentBoneMatrix != nullptr || m_pParentWorldMatrix != nullptr)
	{
		m_matParentMatrix = (*m_pParentBoneMatrix) * (*m_pParentWorldMatrix);
	}
	else
	{
		m_matParentMatrix = *m_pTransformCom->Get_WorldMatrix();
	}


	m_fTimeAcc += fTimeDelta;
	if(m_fTimeAcc > m_EffectrGroup_Data.fLifeTime)
	{
		Initalize_EffectGroup();
	}

	for (auto& pEffectMesh : m_vecEffectMesh)
	{
		pEffectMesh->Update_Effect(fTimeDelta);
	}
	for (auto& pEffectRect : m_vecEffectRect)
	{
		pEffectRect->Update_Effect(fTimeDelta);
	}
	for (auto& pEffectParticle : m_vecEffectParticle)
	{
		pEffectParticle->Update_Effect(fTimeDelta);
	}



	return _int();
}

_int CEffectGroup::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (!m_bActivate)
		return 0;

	//Collision Check
	if(m_fTimeAcc > m_fCollision_StartTime && m_fTimeAcc < m_fCollision_EndTime)
	{
		if (m_pColliderCom != nullptr)
		{
			CCollision_Manager::GetInstance()->
				Check_CollisionAndHit(m_eTarget, m_pColliderCom, m_Damage.iDamage + rand() % m_Damage.iDamage_Range, m_iEffecType);
		}
			
	}
	
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);

	for (auto& pEffectMesh : m_vecEffectMesh)
	{
		pEffectMesh->LastUpdate_Effect(fTimeDelta);
	}
	for (auto& pEffectRect : m_vecEffectRect)
	{
		pEffectRect->LastUpdate_Effect(fTimeDelta);
	}
	for (auto& pEffectParticle : m_vecEffectParticle)
	{
		pEffectParticle->LastUpdate_Effect(fTimeDelta);
	}

	return _int();
}

void CEffectGroup::Render_GameObject()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;

	pEffect->Begin(nullptr, 0);

	for (auto& pEffectMesh : m_vecEffectMesh)
	{
		pEffectMesh->Render_Effect(pEffect, &m_matParentMatrix);
	}


	for (auto& pEffectRect : m_vecEffectRect)
	{
		pEffectRect->Render_Effect(pEffect, &m_matParentMatrix);
	}


	for (auto& pEffectParticle : m_vecEffectParticle)
	{
		pEffectParticle->Render_Effect(pEffect, &m_matParentMatrix);
	}

	if (g_bDebug)
	{
		pEffect->BeginPass(0);

		if (m_pColliderCom != nullptr)
		{
			m_pColliderCom->SetUp_ConstantTable(pEffect, CCollider_Sphere::COLOR_GREEN);
			pEffect->CommitChanges();
			m_pColliderCom->Render_Collider_Sphere();
		}

		pEffect->EndPass();
	}

	pEffect->End();
}

void CEffectGroup::Set_Activate(_bool bActivate)
{
	if (bActivate)
	{
		m_bActivate = bActivate;

		for (auto& pEffectMesh : m_vecEffectMesh)
		{
			pEffectMesh->Initalize_Effect();
		}

		for (auto& pEffectRect : m_vecEffectRect)
		{
			pEffectRect->Initalize_Effect();
		}

		for (auto& pEffectParticle : m_vecEffectParticle)
		{
			pEffectParticle->Initalize_Effect();
		}
	}
		else
	{
		if (m_bDeadObject)
			m_bIsDead = true;

		m_bActivate = false;
		
		Initalize_EffectGroup();


	}
}



HRESULT CEffectGroup::Set_ParentMatrix(const _tchar * pLayerTag, const _char * FrameTag)
{
	CMesh_Dynamic*	pPlayerMeshCom = dynamic_cast<CMesh_Dynamic*>(CManagement::GetInstance()->Get_Component(SCENE_STAGE, pLayerTag, L"Com_Mesh"));
	if (pPlayerMeshCom == nullptr)
		return E_FAIL;

	const Engine::D3DXFRAME_DERIVED*		pFrame = pPlayerMeshCom->Get_FrameByName(FrameTag);

	m_pParentBoneMatrix = &pFrame->CombinedTransformMatrix;

	CTransform*		pPlayerTransCom = dynamic_cast<CTransform*>(CManagement::GetInstance()->Get_Component(SCENE_STAGE, pLayerTag, L"Com_Transform"));
	if (pPlayerTransCom == nullptr)
		return E_FAIL;

	m_pParentWorldMatrix = pPlayerTransCom->Get_WorldMatrix();

	return S_OK;
}

void CEffectGroup::Set_Move(_bool bMove, const _float & fMoveSpeed, const _vec3* vDirection)
{
	m_bMove = bMove;
	m_fMoveSpeed = fMoveSpeed;
	m_vDirection = *vDirection;
}

void CEffectGroup::Set_Position(const _vec3 * pPosition)
{
	//m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, pPosition);
	//	memcpy(&m_matParentMatrix.m[3][0], pPosition, sizeof(_vec3));
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, pPosition);
	if (m_pColliderCom != nullptr)
		m_pColliderCom->Set_Center_Position(pPosition);
}

void CEffectGroup::RotationX(const _float & fRotation)
{
	m_pTransformCom->Rotation_X(fRotation, 1.f);
}

void CEffectGroup::RotationY(const _float & fRotation)
{
	m_pTransformCom->Rotation_Y(fRotation, 1.f);
}

void CEffectGroup::RotationY(const _float & fRotation, const _float & fTimeDelta)
{
	m_pTransformCom->Rotation_Y(fRotation, fTimeDelta);
}

void CEffectGroup::RotationZ(const _float & fRotation)
{
	m_pTransformCom->Rotation_Z(fRotation, 1.f);
}

void CEffectGroup::Set_RotationY(const _float & fRadian)
{
	m_pTransformCom->Set_RotationY(fRadian);
}

void CEffectGroup::Initalize_EffectRotation()
{
	m_pTransformCom->Set_RotationX(0.f);
	m_pTransformCom->Set_RotationY(0.f);
	m_pTransformCom->Set_RotationZ(0.f);
}

void CEffectGroup::Initalize_EffectGroup()
{
	m_fTimeAcc = 0.f;
	m_bActivate = false;
	Initalize_EffectRotation();
}

HRESULT CEffectGroup::Add_Effect_Mesh(CEffect_Mesh * pEffect_Mesh)
{
	if (pEffect_Mesh == nullptr)
		return E_FAIL;

	m_vecEffectMesh.push_back(pEffect_Mesh);

	return S_OK;
}

HRESULT CEffectGroup::Add_Effect_Rect(CEffect_Rect * pEffect_Rect)
{
	if (pEffect_Rect == nullptr)
		return E_FAIL;

	m_vecEffectRect.push_back(pEffect_Rect);

	return S_OK;
}

HRESULT CEffectGroup::Add_Effect_Paritcle(CEffect_Particle * pEffect_Particle)
{
	if (pEffect_Particle == nullptr)
		return E_FAIL;

	m_vecEffectParticle.push_back(pEffect_Particle);

	return S_OK;
}

CEffect_Mesh * CEffectGroup::Get_Effect_Mesh(_uint iIndex)
{
	if (m_vecEffectMesh.size() < iIndex)
		return nullptr;

	return m_vecEffectMesh[iIndex];
}

CEffect_Rect * CEffectGroup::Get_Effect_Rect(_uint iIndex)
{
	if (m_vecEffectRect.size() < iIndex)
		return nullptr;

	return m_vecEffectRect[iIndex];
}

CEffect_Particle * CEffectGroup::Get_Effect_Particle(_uint iIndex)
{
	if (m_vecEffectParticle.size() < iIndex)
		return nullptr;

	return m_vecEffectParticle[iIndex];
}

void CEffectGroup::Load_EffectDataFromFile(HANDLE & hFile, _ulong & dwByte)
{


	ReadFile(hFile, &m_iMeshEffectNum, sizeof(_int), &dwByte, nullptr);
	for(_int i = 0 ; i< m_iMeshEffectNum; ++i)
	{
		MESHDATA MeshData;
		//Effect 기본 데이터
		ReadFile(hFile, &MeshData.Effect_Data, sizeof(EFFECTDATA), &dwByte, nullptr);

		//EffectMesh 데이터
		ReadFile(hFile, &MeshData.Effect_MeshData, sizeof(EFFECTMESH_DATA), &dwByte, nullptr);

		m_vecMeshData.push_back(MeshData);

	}

	m_iRectEffectNum = m_vecEffectRect.size();
	ReadFile(hFile, &m_iRectEffectNum, sizeof(_int), &dwByte, nullptr);
	for (_int i = 0; i < m_iRectEffectNum; ++i)
	{
		RECTDATA	RectData;
		ReadFile(hFile, &RectData.Effect_Data, sizeof(EFFECTDATA), &dwByte, nullptr);

		m_vecRectData.push_back(RectData);
	}

	m_iParticleEffectNum = m_vecEffectParticle.size();
	ReadFile(hFile, &m_iParticleEffectNum, sizeof(_int), &dwByte, nullptr);
	for (_int i = 0; i < m_iParticleEffectNum; ++i)
	{

		PARTICLEDATA	ParticleData;
		//Effect 기본 데이터
		ReadFile(hFile, &ParticleData.Effect_Data, sizeof(EFFECTDATA), &dwByte, nullptr);

		//EffectParticle 데이터z
		ReadFile(hFile, &ParticleData.Effect_ParticleData, sizeof(EFFECTPARTICLE_DATA), &dwByte, nullptr);

		ReadFile(hFile, &ParticleData.bReCycle, sizeof(_bool), &dwByte, nullptr);

		m_vecParticleData.push_back(ParticleData);
	}
}

HRESULT CEffectGroup::SetUp_ColliderInfo(const _float & fRadius, COLLISION_TARGET eTarget, const _float& CollisionStartTime, const _float& CollisionEndTime)
{
	m_pColliderCom = dynamic_cast<CCollider_Sphere*>(CManagement::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Collider_Sphere"));
	if (FAILED(Add_Component(L"Com_Collider", m_pColliderCom)))
		return E_FAIL;

	m_eTarget = eTarget;

	m_pColliderCom->Set_Radius(fRadius);
	m_fCollision_StartTime = CollisionStartTime;
	m_fCollision_EndTime = CollisionEndTime;

	return S_OK;
}



_float CEffectGroup::Get_ProgressRadio()
{
	return m_fTimeAcc / m_EffectrGroup_Data.fLifeTime;
}

const _vec3 * CEffectGroup::Get_Position()
{
	return m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);	
}

CTransform * CEffectGroup::Get_Transform()
{
	return 	m_pTransformCom;	
}



HRESULT CEffectGroup::Ready_Component()
{
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();
	{
		//For. Renderer
		m_pRendererCom = dynamic_cast<CRenderer*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Renderer"));
		if (FAILED(Add_Component(L"Com_Renderer", m_pRendererCom)))
			return E_FAIL;

		//For. Shader;
		m_pShaderCom = dynamic_cast<CShader*>(pManagement->Clone_Component(SCENE_STAGE, L"Component_Shader_Effect"));
		if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
			return E_FAIL;

		//For. Transform;
		m_pTransformCom = dynamic_cast<CTransform*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Transform"));
		if (FAILED(Add_Component(L"Com_Transform", m_pTransformCom)))
			return E_FAIL;
	}
	Safe_Release(pManagement);

	return S_OK;
}

CGameObject * CEffectGroup::Clone_GameObject()
{
	CEffectGroup*	pInstance = new CEffectGroup(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX(L"CEffectGroup Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CEffectGroup * CEffectGroup::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CEffectGroup*	pInstance = new CEffectGroup(pDevice);

	if (FAILED(pInstance->Ready_Prototype()))
	{
		_MSG_BOX(L"CEffectGroup Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffectGroup::Free()
{
	for (auto& pEffectMesh : m_vecEffectMesh)
		Safe_Release(pEffectMesh);
	m_vecEffectMesh.clear();

	for (auto& pEffectRect : m_vecEffectRect)
		Safe_Release(pEffectRect);
	m_vecEffectRect.clear();

	for (auto& pEffectParticle : m_vecEffectParticle)
		Safe_Release(pEffectParticle);
	m_vecEffectParticle.clear();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pColliderCom);

	m_vecMeshData.clear();
	m_vecRectData.clear();
	m_vecParticleData.clear();

	CGameObject::Free();
}
