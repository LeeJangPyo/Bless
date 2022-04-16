#include "stdafx.h"
#include "..\Headers\GameObject_Dynamic.h"


#include "Management.h"
#include "Collision_Manager.h"
#include "GameEvent_Manager.h"
#include "Light_Manager.h"

#include "Camera_Dynamic.h"
#include "State.h"

_USING(Client)


CGameObject_Dynamic::CGameObject_Dynamic(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
	, m_pTransformCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pCalculatorCom(nullptr)
	, m_pMeshCom(nullptr)
	, m_pShaderCom(nullptr)
	, m_pNaviMeshCom(nullptr)
	, m_fFrameConst(1.f)
	, m_fHitTimeAcc(0.f)
	, m_fHitDelay(0.2f)
	, m_fLimRightWidth(g_fDefaultRimLight)

{
	ZeroMemory(&m_BaseStats, sizeof(BASE_STATS));
	ZeroMemory(&m_Damage, sizeof(DAMAGE));
}

CGameObject_Dynamic::CGameObject_Dynamic(const CGameObject_Dynamic & rhs)
	:CGameObject(rhs)
	, m_pTransformCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pCalculatorCom(nullptr)
	, m_pMeshCom(nullptr)
	, m_pShaderCom(nullptr)
	, m_pNaviMeshCom(nullptr)
	, m_BaseStats(rhs.m_BaseStats)
	, m_Damage(rhs.m_Damage)
	, m_fFrameConst(1.f)
	, m_fHitTimeAcc(0.f)
	, m_fHitDelay(0.3f)
	, m_fLimRightWidth(g_fDefaultRimLight)
{
	
}

void CGameObject_Dynamic::Set_AnimationSet(const _uint & iIndex, _bool bResetAnimation)
{
	m_pMeshCom->Set_AnimationSet(iIndex, bResetAnimation);
}

void CGameObject_Dynamic::Reset_AnimationSet()
{
	m_pMeshCom->Reset_AnimationSet();
}

list<CCollider_Sphere*>* CGameObject_Dynamic::Get_ColliderList()
{
	if(m_ColliderList.size() <= 0)
		return nullptr;

	return &m_ColliderList;
}

_float CGameObject_Dynamic::Get_LongestRadius()
{
	_float fLognestRadius = 0;

	for(auto& Collider :m_ColliderList)
	{
		_float fRadius = *Collider->Get_Radius();
		if(fRadius > fLognestRadius)
		{
			fLognestRadius = fRadius;
		}
	}

	return fLognestRadius;
}

CTransform * CGameObject_Dynamic::Get_Transform()
{
	return m_pTransformCom;
}


_bool CGameObject_Dynamic::Get_IsAnimationSetEnd()
{
	return m_pMeshCom->Is_AnimationSetEnd();
}

_bool CGameObject_Dynamic::Get_IsAnimationRange(const _float & fStart, const _float & fEnd)
{
	return m_pMeshCom->Is_AnimationInRange(fStart,fEnd);
}

_bool CGameObject_Dynamic::Get_IsAnimationPassRadio(const _float & fRadio)
{
	return m_pMeshCom->Is_AnimationPassRadio(fRadio);
}


HRESULT Client::CGameObject_Dynamic::Ready_Prototype()
{


	return S_OK;
}

HRESULT Client::CGameObject_Dynamic::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_dwCurrentNaviIndex = 0;

	return S_OK;
}

Engine::_int Client::CGameObject_Dynamic::Update_GameObject(const _float& fTimeDelta)
{
	m_pMeshCom->Play_AnimationSet(fTimeDelta * m_fFrameConst);

	for (auto& Colldier : m_ColliderList)
	{
		Colldier->Update_Collider_Sphere();
	}


	return 0;
}

Engine::_int Client::CGameObject_Dynamic::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if (m_pRendererCom == nullptr || m_pOptimizationCom == nullptr)
		return 0;

	m_bDraw = m_pOptimizationCom->Is_InFrustum_ForObject(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION),2.5f);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONEALPHA, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOW, this);

	return 0;
}

void Client::CGameObject_Dynamic::Render_GameObject()
{
	if (m_bDraw == false ||
		m_pMeshCom == nullptr ||
		m_pShaderCom == nullptr)
		return;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;

	pEffect->AddRef();

	if (FAILED(SetUp_ConstantTable(pEffect)))
	{
		Safe_Release(pEffect);
		return;
	}

	if (m_pRendererCom->Get_RenderGroup() == CRenderer::RENDER_NONEALPHA)
	{
		pEffect->Begin(nullptr, 0);

		//3 : Normal + Spec;
		//5 + Rim
		pEffect->BeginPass(5);

		m_pMeshCom->Render_Mesh(pEffect);

		pEffect->EndPass();
	}
	if (m_pRendererCom->Get_RenderGroup() == CRenderer::RENDER_SHADOW)
	{
		pEffect->Begin(nullptr, 0);

		//ShawdowMapping 4
		pEffect->BeginPass(4);

		m_pMeshCom->Render_Mesh(pEffect);

		pEffect->EndPass();
	}
	////////////////////////////////////////////////////////////////////////
	//if (g_bDebug)
	//{
	//	pEffect->BeginPass(1);

	//	for (auto& Colldier : m_ColliderList)
	//	{
	//		Colldier->SetUp_ConstantTable(pEffect, CCollider_Sphere::COLOR_GREEN);

	//		pEffect->CommitChanges();
	//		Colldier->Render_Collider_Sphere();
	//	}


	//	pEffect->EndPass();
	//}

	pEffect->End();

	Safe_Release(pEffect);
}
void CGameObject_Dynamic::Go_Target(const _vec3 * pTargetPos, const _float & fTimeDelta)
{
	m_pTransformCom->Go_Target(pTargetPos, m_BaseStats.fMoveSpeed, fTimeDelta);
}

_bool CGameObject_Dynamic::Check_Collision(CCollider_Sphere* pOtherCollider, _vec3* Collision_Position)
{
	for(auto& pCollider : m_ColliderList)
	{
		_float fDistnace;
		_vec3 fDifference = *pCollider->Get_Center() - *pOtherCollider->Get_Center();

		fDistnace = D3DXVec3Length(&fDifference);

		if (fDistnace <= *pCollider->Get_Radius() + *pOtherCollider->Get_Radius())
		{
			if (Collision_Position != nullptr)
				*Collision_Position = (*pCollider->Get_Center() + *pOtherCollider->Get_Center()) *0.5f;

			return true;
		}
	}
	return false;
	
}

_bool CGameObject_Dynamic::Check_CollsionAndPush(CGameObject_Dynamic * pOtherObject)
{
	for (auto& pCollider : m_ColliderList)
	{
		for(auto& pOtherCollider : pOtherObject->m_ColliderList)
		{
			_float fDistnace;
			_vec3 vDir = *pOtherCollider->Get_Center() - *pCollider->Get_Center();
			fDistnace = D3DXVec3Length(&vDir);
			if (fDistnace <= *pCollider->Get_Radius() + *pOtherCollider->Get_Radius())
			{
				_vec3 vPos = *pOtherObject->m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
				vDir.y = 0.f;
				D3DXVec3Normalize(&vDir, &vDir);
				
				pOtherObject->m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(vPos + vDir *fDistnace * 0.1f));
				return true;
			}
		}
	}
	return false;
}

_vec3 CGameObject_Dynamic::Move_OnNaviMesh(const _vec3 * pTargetPos, const _float& fTimeDelta)
{
	_vec3 vPosition = *m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
	_vec3 vTagetDir = *pTargetPos - vPosition;
	D3DXVec3Normalize(&vTagetDir, &vTagetDir);
	
	return m_pNaviMeshCom->Move_OnNaviMesh(&vPosition, &_vec3(vTagetDir * m_BaseStats.fMoveSpeed * fTimeDelta),m_dwCurrentNaviIndex);
}

_vec3 CGameObject_Dynamic::Stand_OnNaviMesh(const _vec3 * pPosition)
{
	return m_pNaviMeshCom->Stand_OnNaviMesh(pPosition,m_dwCurrentNaviIndex);
}

_bool CGameObject_Dynamic::Compute_Position_OnNaviMeshIndex()
{
	if (m_pNaviMeshCom == nullptr)
		return false;

	if (m_pNaviMeshCom->Compute_Position_OnNaviMeshIndex(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION), m_dwCurrentNaviIndex))
	{
		return true;
	}
	return 	false;
}

HRESULT CGameObject_Dynamic::Change_NavigationMesh()
{
	Safe_Release(m_pNaviMeshCom);
	CGameObject::Remove_Component(L"Com_Navi");

	//For. NaviMesh
	m_pNaviMeshCom = dynamic_cast<CMesh_Navigation*>(CManagement::GetInstance()->Get_Component(SCENE_STAGE, L"Layer_Terrain_Stage2", L"Com_Navi"));
	if (FAILED(Add_Component(L"Com_Navi", m_pNaviMeshCom)))
		return E_FAIL;
	m_pNaviMeshCom->AddRef();

	return S_OK;
}

HRESULT CGameObject_Dynamic::Change_State(const _tchar * pStateTag)
{
	if (pStateTag == nullptr)
		return E_FAIL;

	auto& iter = find_if(m_mapState.begin(), m_mapState.end(), CFinder_Tag(pStateTag));
	if (iter == m_mapState.end())
		return E_FAIL;

	//End
	if(m_pCurrentState != nullptr)
		m_pCurrentState->End();

	//Change
	m_pCurrentState = iter->second;
	m_pCurrentState->Begin();

	return S_OK;
}

_bool CGameObject_Dynamic::Compare_State(const _tchar * pStateTag)
{
	if (pStateTag == nullptr)
		return false;
	
	auto& iter = find_if(m_mapState.begin(), m_mapState.end(), CFinder_Tag(pStateTag));
	if (iter == m_mapState.end())
		return false;

	if (iter->second == m_pCurrentState)
		return true;
	
	return false;
}

HRESULT CGameObject_Dynamic::Load_ColliderInfoFromFile(const _tchar* pFilePath)
{

	HANDLE		hFile = CreateFile(pFilePath
		, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFile == INVALID_HANDLE_VALUE)
		return E_FAIL;

	_int	iColliderSize;
	_int	iStrSize;
	_tchar	szKeyBuffer[MIN_PATH];
	_char	szBoneTagBuffer[MIN_PATH];

	_ulong dwByte = 0;

	//Collider Info Save;
	ReadFile(hFile, &iColliderSize, sizeof(_int), &dwByte, nullptr);
	for (_int i = 0; i < iColliderSize; ++i)
	{
		CCollider_Sphere* pCollider = dynamic_cast<CCollider_Sphere*>(CManagement::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Collider_Sphere"));
		if (pCollider == nullptr)
			return E_FAIL;

		//Map Key Size;
		ReadFile(hFile, &iStrSize, sizeof(_int), &dwByte, nullptr);
		ReadFile(hFile, szKeyBuffer, sizeof(_tchar)* iStrSize, &dwByte, nullptr);


		//Collier Info Save
		//BoneString
		ReadFile(hFile, &iStrSize, sizeof(_int), &dwByte, nullptr);
		ReadFile(hFile, szBoneTagBuffer, sizeof(_char)* iStrSize, &dwByte, nullptr);

		//Riadus
		_float fRadius;
		ReadFile(hFile, &fRadius, sizeof(_float), &dwByte, nullptr);

		//Position;
		_vec3 vCenterPos;
		ReadFile(hFile, &vCenterPos, sizeof(_vec3), &dwByte, nullptr);

		const Engine::D3DXFRAME_DERIVED*		pFrame = m_pMeshCom->Get_FrameByName(szBoneTagBuffer);
		if (pFrame == nullptr)
			return E_FAIL;

		pCollider->SetUp_Collider_Spehre(&pFrame->CombinedTransformMatrix, m_pTransformCom->Get_WorldMatrix(), &vCenterPos, fRadius);

		m_ColliderList.push_back(pCollider);
	}
	CloseHandle(hFile);
	return S_OK;
}


HRESULT CGameObject_Dynamic::Ready_Component()
{
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();

	//////////////////////////////////////////////////////////////////////////
	//Static Component
	m_pRendererCom = dynamic_cast<CRenderer*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Renderer"));
	if (FAILED(Add_Component(L"Com_Renderer", m_pRendererCom)))
		return E_FAIL;

	m_pTransformCom = dynamic_cast<CTransform*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Transform"));
	if (FAILED(Add_Component(L"Com_Transform", m_pTransformCom)))
		return E_FAIL;

	m_pCalculatorCom = dynamic_cast<CCalculator*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Calculator"));
	if (FAILED(Add_Component(L"Com_Calculator", m_pCalculatorCom)))
		return E_FAIL;

	m_pOptimizationCom = dynamic_cast<COptimization*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Optimization"));
	if (FAILED(Add_Component(L"Com_Optimization", m_pOptimizationCom)))
		return E_FAIL;


	//////////////////////////////////////////////////////////////////////////
	//Stage Component
	//For. Shader
	m_pShaderCom = dynamic_cast<CShader*>(pManagement->Clone_Component(SCENE_STAGE, L"Component_Shader_Mesh"));
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;


	if (CGameEvent_Manager::GetInstance()->Get_Stage() == LOGO)
	{
		//For. NaviMesh
		m_pNaviMeshCom = dynamic_cast<CMesh_Navigation*>(CManagement::GetInstance()->Get_Component(SCENE_STAGE, L"Layer_Terrain", L"Com_Navi"));
		if (FAILED(Add_Component(L"Com_Navi", m_pNaviMeshCom)))
			return E_FAIL;
		m_pNaviMeshCom->AddRef();
	}
	else
	{
		//For. NaviMesh
		m_pNaviMeshCom = dynamic_cast<CMesh_Navigation*>(CManagement::GetInstance()->Get_Component(SCENE_STAGE, L"Layer_Terrain_Stage2", L"Com_Navi"));
		if (FAILED(Add_Component(L"Com_Navi", m_pNaviMeshCom)))
			return E_FAIL;
		m_pNaviMeshCom->AddRef();
	}


	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CGameObject_Dynamic::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (pEffect == nullptr)
		return E_FAIL;

	pEffect->AddRef();

	if (FAILED(m_pTransformCom->SetUp_OnShader(pEffect, "g_matWorld")))
		return E_FAIL;

	if (m_pRendererCom->Get_RenderGroup() == CRenderer::RENDER_NONEALPHA)
	{
		CCamera* pCamera = CManagement::GetInstance()->Get_CameraMain();
		if (pCamera == nullptr)
		{
			Safe_Release(pEffect);
			return E_FAIL;
		}

		pCamera->SetUp_OnShader_View(pEffect, "g_matView");
		pCamera->SetUp_OnShader_Projection(pEffect, "g_matProj");
		pCamera->SetUp_OnShader_FarPlane(pEffect, "g_fFarPlane");
		pCamera->SetUp_OnShader_WorldCameraPos(pEffect, "g_vCameraPos");

		pEffect->SetFloat("g_fLimRightWidth", m_fLimRightWidth);
	}

	if (m_pRendererCom->Get_RenderGroup() == CRenderer::RENDER_SHADOW)
	{
	
		_matrix matPorj;
		D3DXMatrixIdentity(&matPorj);
		m_pDevice->GetTransform(D3DTS_PROJECTION, &matPorj);

		pEffect->SetMatrix("g_matView", CLight_Manager::GetInstance()->Get_LightViewMatrix());
		pEffect->SetMatrix("g_matProj", &matPorj);
		pEffect->SetFloat("g_fFarPlane", 1000.f);

	}

	Safe_Release(pEffect);

	return S_OK;
}

HRESULT CGameObject_Dynamic::SetUp_GameObjectInfo(const _matrix * pMatrix)
{

	return S_OK;
}


void Client::CGameObject_Dynamic::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pCalculatorCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pNaviMeshCom);
	Safe_Release(m_pOptimizationCom);

	for (auto& Colldier : m_ColliderList)
		Safe_Release(Colldier);
	m_ColliderList.clear();
	
	for (auto& Pair : m_mapState)
		Safe_Release(Pair.second);
	m_mapState.clear();

	CGameObject::Free();
}

