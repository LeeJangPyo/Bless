#include "stdafx.h"
#include "..\Headers\GameObject_Monster.h"

#include "Management.h"

_USING(Client)
CGameObject_Monster::CGameObject_Monster(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject_Dynamic(pDevice)
	, m_fTimeDelta(0.f)
	, m_eTargetPlayerType(PLAYER_END)
	, m_pCollider_Weapon(nullptr)

{
}

CGameObject_Monster::CGameObject_Monster(const CGameObject_Monster & rhs)
	: CGameObject_Dynamic(rhs)
	, m_fTimeDelta(0.f)
	, m_eTargetPlayerType(PLAYER_END)
	, m_pCollider_Weapon(nullptr)
{
}

HRESULT CGameObject_Monster::Ready_Prototype()
{
	if (FAILED(CGameObject_Dynamic::Ready_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGameObject_Monster::Ready_GameObject()
{
	if (FAILED(CGameObject_Dynamic::Ready_GameObject()))
		return E_FAIL;

	_int iRand = rand() % 2;

	if (iRand == 0)
		m_eTargetPlayerType = PLAYER_LUPS;
	else
		m_eTargetPlayerType = PLAYER_SIRE;

	return S_OK;
}

_int CGameObject_Monster::Update_GameObject(const _float & fTimeDelta)
{
	//Animation Frame;
	m_fTimeDelta = fTimeDelta * m_fFrameConst;

	return _int();
}

_int CGameObject_Monster::LastUpdate_GameObject(const _float & fTimeDelta)
{
	return CGameObject_Dynamic::LastUpdate_GameObject(fTimeDelta);
}

void CGameObject_Monster::Render_GameObject()
{
	if (m_pMeshCom == nullptr ||
		m_pShaderCom == nullptr)
		return;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;


	if (FAILED(CGameObject_Dynamic::SetUp_ConstantTable(pEffect)))
	{
		Safe_Release(pEffect);
		return;
	}


	pEffect->AddRef();
	{
		if (m_pRendererCom->Get_RenderGroup() == CRenderer::RENDER_NONEALPHA)
		{
			//Animation Update
			m_pMeshCom->Play_AnimationSet(m_fTimeDelta);

			//Collider Update
			for (auto& Collider : m_ColliderList)
				Collider->Update_Collider_Sphere();

			if (m_pCollider_Weapon != nullptr)
				m_pCollider_Weapon->Update_Collider_Sphere();

			pEffect->Begin(nullptr, 0);

			//3 : Normal + Spec;
			//5 : Normal + Spec + RIM;  
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

		//	//Weapon
		//	if (m_pCollider_Weapon != nullptr)
		//	{
		//		m_pCollider_Weapon->SetUp_ConstantTable(pEffect, CCollider_Sphere::COLOR_GREEN);

		//		pEffect->CommitChanges();
		//		m_pCollider_Weapon->Render_Collider_Sphere();

		//	}
		//	pEffect->EndPass();
		//}

	}
	pEffect->End();

	Safe_Release(pEffect);
}


HRESULT CGameObject_Monster::SetUp_GameObjectInfo(const _matrix * pMatrix)
{
	return S_OK;
}

const _vec3 * CGameObject_Monster::Get_WeaponCollider_Position()
{
	if (m_pCollider_Weapon == nullptr)
		return nullptr;
	return m_pCollider_Weapon->Get_Center();
}

HRESULT CGameObject_Monster::Load_WeaponCollier_FromFile(const _tchar * pFilePath)
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

		_vec3 vCenterPos;
		ReadFile(hFile, &vCenterPos, sizeof(_vec3), &dwByte, nullptr);

		const Engine::D3DXFRAME_DERIVED*		pFrame = m_pMeshCom->Get_FrameByName(szBoneTagBuffer);
		if (pFrame == nullptr)
			return E_FAIL;

		pCollider->SetUp_Collider_Spehre(&pFrame->CombinedTransformMatrix, m_pTransformCom->Get_WorldMatrix(), &vCenterPos, fRadius);

		m_pCollider_Weapon = pCollider;
	}

	CloseHandle(hFile);
	return S_OK;
}

void CGameObject_Monster::Free()
{
	Safe_Release(m_pCollider_Weapon);
	CGameObject_Dynamic::Free();
}

