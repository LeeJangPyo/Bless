#include "stdafx.h"
#include "..\Headers\Effect_Mesh.h"

#include "Management.h"

#include <iostream>
_USING(Client)

CEffect_Mesh::CEffect_Mesh(LPDIRECT3DDEVICE9 pDevice)
	:CEffect(pDevice)
	, m_pMeshCom(nullptr)	
{
	ZeroMemory(&m_EffectMesh_Data, sizeof(EFFECTMESH_DATA));
}



HRESULT CEffect_Mesh::SetUp_EffectInfo(const _tchar * pComponentMeshTag, CEffectTab * pEffectTab)
{
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();
	{
		lstrcpy(m_EffectMesh_Data.szMeshTag, pComponentMeshTag);

		// Set MeshTab Pointer;
		m_pEffectTab = pEffectTab;

		//For. Mesh
		m_pMeshCom = dynamic_cast<CMesh_Static*>(pManagement->Clone_Component(SCENE_EDIT, pComponentMeshTag));
		if(m_pMeshCom == nullptr)
			return E_FAIL;

	}
	Safe_Release(pManagement);

	return S_OK;
}


HRESULT CEffect_Mesh::Ready_Effect()
{
	if (FAILED(CEffect::Ready_Effect()))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

_int CEffect_Mesh::Update_Effect(const _float & fTimeDelta)
{
	//활성화되지 않으면 업데이트 하지 않는다.
	if (!m_bIsActivate)
		return 0;

	m_fTimeAcc += fTimeDelta;
	if (m_fTimeAcc > m_EffectData.fEndTime)
	{
		m_fAlpha = 1.f;
		m_fTimeAcc = 0.f;
		m_bIsActivate = false;
		SetUp_Position_Interpolation();
		SetUp_Scale_Interpolation();
	}

	//시작시간이 지나면 시작
	if (m_fTimeAcc > m_EffectData.fStartTime)
	{
		cout << m_fAlpha << endl;

		//FadeIn - Out
		if (m_fTimeAcc < m_EffectData.fStartTime + m_EffectData.fFadeIn)
		{
			m_fAlpha += m_EffectData.fAlpha * fTimeDelta / m_EffectData.fFadeIn;
		}
		else if (m_fTimeAcc > m_EffectData.fEndTime - m_EffectData.fFadeOut)
		{
			m_fAlpha -= m_EffectData.fAlpha * fTimeDelta / m_EffectData.fFadeOut;
		}
		else
		{
			m_fAlpha = m_EffectData.fAlpha;
		}


		//이미지 프레임 업데이트
		m_fFrame += m_EffectData.iAlpha_Width *m_EffectData.iAlpha_Height * fTimeDelta;
		if (m_EffectData.iAlpha_Width *m_EffectData.iAlpha_Height <= m_fFrame)
			m_fFrame = 0.f;

		m_iAnimationX = (_int)m_fFrame % m_EffectData.iAlpha_Width;
		m_iAnimationY = (_int)m_fFrame / m_EffectData.iAlpha_Width;

		
		//if (m_EffectData.iPassIndex > 2)
		//{
		//	m_fFrame += m_EffectData.iBase_Width *m_EffectData.iBase_Height * fTimeDelta;
		//	if (m_EffectData.iBase_Width *m_EffectData.iBase_Height <= m_fFrame)
		//		m_fFrame = 0.f;

		//	m_iWidth = (_int)m_fFrame / m_EffectData.iBase_Width;
		//	m_iHeight = (_int)m_fFrame / m_EffectData.iBase_Height;

		//	m_fU_Interval = 1.f / m_EffectData.iBase_Width;
		//	m_fV_Interval = 1.f / m_EffectData.iBase_Height;
		//}

		//애니메이션 보간
		if (!m_pPosition_Interploation->Interpolate(fTimeDelta))
			m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, m_pPosition_Interploation->GetValue());

		if (!m_pScale_Interploation->Interpolate(fTimeDelta))
			m_pTransformCom->Set_ScaleInfo(m_pScale_Interploation->GetValue());

		//m_pRotation_Interploation->Interpolate(fTimeDelta);
		_vec3 vRot = m_EffectData.vStart_Rotation - m_EffectData.vEnd_Rotation;

		m_pTransformCom->Rotation_X(D3DXToRadian(vRot.x), fTimeDelta);
		m_pTransformCom->Rotation_Y(D3DXToRadian(vRot.y), fTimeDelta);
		m_pTransformCom->Rotation_Z(D3DXToRadian(vRot.z), fTimeDelta);

	}
	
	return _int();
}

_int CEffect_Mesh::LastUpdate_Effect(const _float & fTimeDelta)
{
	//if (!m_bIsActivate)
	//	return 0;
	//m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONEALPHA, this);


	return _int();
}

void CEffect_Mesh::Render_Effect(LPD3DXEFFECT pEffect)
{
	if (m_pMeshCom == nullptr || pEffect == nullptr)
		return;

	pEffect->AddRef();
	{
		pEffect->BeginPass(m_EffectData.iPassIndex);

		if (FAILED(SetUp_ConstantTable(pEffect)))
		{
			Safe_Release(pEffect);
			return;
		}

		pEffect->CommitChanges();

		m_pMeshCom->Render_Mesh();

		pEffect->EndPass();
	}
	Safe_Release(pEffect);
}

void CEffect_Mesh::Save_EffectData(HANDLE & hFile, _ulong & dwByte)
{

	m_EffectData.matTransform = *m_pTransformCom->Get_WorldMatrix();
	//Effect 기본 데이터
	WriteFile(hFile, &m_EffectData, sizeof(EFFECTDATA), &dwByte, nullptr);

	//EffectMesh 데이터
	WriteFile(hFile, &m_EffectMesh_Data, sizeof(EFFECTMESH_DATA), &dwByte, nullptr);
}

void CEffect_Mesh::Load_EffectDataFromFile(HANDLE & hFile, _ulong & dwByte, CEffectTab* pEffectTab)
{
	//Effect 기본 데이터
	ReadFile(hFile, &m_EffectData, sizeof(EFFECTDATA), &dwByte, nullptr);

	//EffectMesh 데이터
	ReadFile(hFile, &m_EffectMesh_Data, sizeof(EFFECTMESH_DATA), &dwByte, nullptr);

	m_pTransformCom->Set_WorldMatrix(&m_EffectData.matTransform);

	SetUp_EffectInfo(m_EffectMesh_Data.szMeshTag, pEffectTab);
	SetUp_AlphaTexutre(m_EffectData.szAlphaTextureTag);
	SetUp_BaseTexture(m_EffectData.szBaseTextureTag);
	SetUp_NoiseTexture(m_EffectData.szNoiseTextureTag);
}

HRESULT CEffect_Mesh::SetUp_EffectDataFromFile(EFFECTDATA & EffectData, EFFECTMESH_DATA & EffectMesh_Data, CEffectTab* pEffectTab)
{
	//CEffect::SetUp_EffectDataFromFile(EffectData);

	//m_EffectMesh_Data.fUSpeed = EffectMesh_Data.fUSpeed;
	//m_EffectMesh_Data.fVSpeed = EffectMesh_Data.fVSpeed;
	//m_EffectMesh_Data.iPassIndex = EffectMesh_Data.iPassIndex;

	//if (!lstrcmp(EffectMesh_Data.szAlphaTextureTag, L""))
	//{
	//	SetUp_AlphaTexutre(EffectMesh_Data.szAlphaTextureTag);
	//}

	//if (!lstrcmp(EffectMesh_Data.szBaseTextureTag, L""))
	//{
	//	SetUp_AlphaTexutre(EffectMesh_Data.szBaseTextureTag);
	//}

	//SetUp_EffectInfo(EffectMesh_Data.szGameObjectTag, EffectMesh_Data.szMeshTag, pEffectTab);

	return S_OK;
}


HRESULT CEffect_Mesh::Ready_Component()
{
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();
	{

	}
	Safe_Release(pManagement);

	return S_OK;
}



CEffect_Mesh * CEffect_Mesh::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CEffect_Mesh*	pInstance = new CEffect_Mesh(pDevice);

	if (FAILED(pInstance->Ready_Effect()))
	{
		_MSG_BOX(L"CEffect_Mesh Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Mesh::Free()
{
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pAlphaTextureCom);
	Safe_Release(m_pBaseTextureCom);

	CEffect::Free();
}
