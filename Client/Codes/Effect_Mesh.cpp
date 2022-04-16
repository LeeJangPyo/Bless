#include "stdafx.h"
#include "..\Headers\Effect_Mesh.h"

#include "Management.h"

_USING(Client)

CEffect_Mesh::CEffect_Mesh(LPDIRECT3DDEVICE9 pDevice)
	:CEffect(pDevice)
	, m_pMeshCom(nullptr)	
{
	ZeroMemory(&m_EffectMesh_Data, sizeof(EFFECTMESH_DATA));
}



HRESULT CEffect_Mesh::SetUp_EffectInfo(const _tchar * pComponentMeshTag)
{
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();
	{
		lstrcpy(m_EffectMesh_Data.szMeshTag, pComponentMeshTag);

		//For. Mesh
		m_pMeshCom = dynamic_cast<CMesh_Static*>(pManagement->Clone_Component(SCENE_STAGE, pComponentMeshTag));
		if(m_pMeshCom == nullptr)
			return E_FAIL;

	}
	Safe_Release(pManagement);

	return S_OK;
}

void CEffect_Mesh::SetUp_EffectData(const EFFECTDATA & EffectData, const EFFECTMESH_DATA & Effect_MeshData)
{
	m_EffectData = EffectData;
	m_EffectMesh_Data = Effect_MeshData;

	m_pTransformCom->Set_WorldMatrix(&m_EffectData.matTransform);

	SetUp_EffectInfo(m_EffectMesh_Data.szMeshTag);
	SetUp_AlphaTexutre(m_EffectData.szAlphaTextureTag);
	SetUp_BaseTexture(m_EffectData.szBaseTextureTag);
	SetUp_NoiseTexture(m_EffectData.szNoiseTextureTag);
}


HRESULT CEffect_Mesh::Ready_Effect()
{
	if (FAILED(CEffect::Ready_Effect()))
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
		m_fAlpha = m_EffectData.fInit_Alpha;
		m_fTimeAcc = 0.f;
		m_bIsActivate = false;
		SetUp_Position_Interpolation();
		SetUp_Scale_Interpolation();
	}


	//시작시간이 지나면 시작
	if (m_fTimeAcc > m_EffectData.fStartTime)
	{
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


	return 0;
}

_int CEffect_Mesh::LastUpdate_Effect(const _float & fTimeDelta)
{
	return _int();
}

void CEffect_Mesh::Render_Effect(LPD3DXEFFECT pEffect, const _matrix* pmatParent)
{
	if (m_pMeshCom == nullptr || pEffect == nullptr)
		return;

	pEffect->AddRef();
	{
		pEffect->BeginPass(m_EffectData.iPassIndex);

		if (FAILED(SetUp_ConstantTable(pEffect, pmatParent)))
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

void CEffect_Mesh::Initalize_Effect()
{
	//m_pTransformCom->Set_WorldMatrix(&m_EffectData.matTransform);

	Set_Activate(true);
	SetUp_Position_Interpolation();
	SetUp_Scale_Interpolation();
	Initalize_Alpha();
	m_fFrame = 0.f;
	m_fTimeAcc = 0.f;
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
