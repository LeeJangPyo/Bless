#include "stdafx.h"
#include "..\Headers\Effect.h"

#include "Management.h"
#include "Linear_Interpolation.h"

_USING(Client)

CEffect::CEffect(LPDIRECT3DDEVICE9 pDevice)
	: m_pDevice(pDevice)
	, m_pTransformCom(nullptr)
	, m_pPosition_Interploation(nullptr)
	, m_pScale_Interploation(nullptr)
	, m_bIsActivate(false)
	, m_pAlphaTextureCom(nullptr)
	, m_pBaseTextureCom(nullptr)
	, m_pNoiseTextureCom(nullptr)
	, m_fTimeAcc(0.f)
	, m_fAlpha(1.f)
	, m_fFrame(0.f)
{
	m_pDevice->AddRef();
	ZeroMemory(&m_EffectData, sizeof(EFFECTDATA));
}

void CEffect::Set_Position(const _vec3 * pPosition)
{
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, pPosition);
}

void CEffect::Set_Scale(const _float & fScaleX, const _float & fScaleY, const _float & fScaleZ)
{
	m_pTransformCom->Set_ScaleInfo(fScaleX, fScaleY, fScaleZ);
}

void CEffect::Set_Rotation(const _float & fRotationX, const _float & fRotationY, const _float & fRotationZ)
{
	m_vRotation.x += fRotationX;
	m_vRotation.y += fRotationY;
	m_vRotation.z += fRotationZ;

	m_pTransformCom->Rotation_X(D3DXToRadian(fRotationX),1.f);
	m_pTransformCom->Rotation_Y(D3DXToRadian(fRotationY),1.f);
	m_pTransformCom->Rotation_Z(D3DXToRadian(fRotationZ),1.f);
}

void CEffect::Get_Position(_float & fPositionX, _float & fPositionY, _float & fPositionZ)
{
	const _vec3* pPosition = m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
	fPositionX = pPosition->x;
	fPositionY = pPosition->y;
	fPositionZ = pPosition->z;
}

void CEffect::Get_Scale(_float & fScaleX, _float & fScaleY, _float & fScaleZ)
{
	fScaleX = m_pTransformCom->Get_ScaleInfo(CTransform::STATE_RIGHT);
	fScaleY = m_pTransformCom->Get_ScaleInfo(CTransform::STATE_UP);
	fScaleZ = m_pTransformCom->Get_ScaleInfo(CTransform::STATE_LOOK);
}

void CEffect::Get_Rotation(_float & fRotationX, _float & fRotationY, _float & fRotationZ)
{
	fRotationX = m_vRotation.x;
	fRotationY = m_vRotation.y;
	fRotationZ = m_vRotation.x;
}

_vec3 CEffect::Set_StartPosition()
{
	return m_EffectData.vStart_Position = *m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
}

void CEffect::Set_StartPosition(const _vec3 * pPosition)
{
	m_EffectData.vStart_Position = *pPosition;
}

_vec3 CEffect::Set_EndPosition()
{
	return m_EffectData.vEnd_Position = *m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
}

void CEffect::Set_EndPosition(const _vec3 * pPosition)
{
	m_EffectData.vEnd_Position = *pPosition;
}

_vec3 CEffect::Set_StartRotation()
{
	return m_EffectData.vStart_Rotation = m_vRotation;
}

_vec3 CEffect::Set_EndRotation()
{
	return m_EffectData.vEnd_Rotation = m_vRotation;
}

_vec3 CEffect::Set_StartScale()
{
	m_EffectData.vStart_Scale.x = m_pTransformCom->Get_ScaleInfo(CTransform::STATE_RIGHT);
	m_EffectData.vStart_Scale.y = m_pTransformCom->Get_ScaleInfo(CTransform::STATE_UP);
	m_EffectData.vStart_Scale.z = m_pTransformCom->Get_ScaleInfo(CTransform::STATE_LOOK);

	return m_EffectData.vStart_Scale;
}

_vec3 CEffect::Set_EndScale()
{
	m_EffectData.vEnd_Scale.x = m_pTransformCom->Get_ScaleInfo(CTransform::STATE_RIGHT);
	m_EffectData.vEnd_Scale.y = m_pTransformCom->Get_ScaleInfo(CTransform::STATE_UP);
	m_EffectData.vEnd_Scale.z = m_pTransformCom->Get_ScaleInfo(CTransform::STATE_LOOK);

	return m_EffectData.vEnd_Scale;
}


HRESULT CEffect::SetUp_AlphaTexutre(const _tchar * pComponentTextureTag)
{
	if (!lstrcmp(pComponentTextureTag, L""))
		return E_FAIL;

	lstrcpy(m_EffectData.szAlphaTextureTag, pComponentTextureTag);

	//For. Texture;
	m_pAlphaTextureCom = dynamic_cast<CTexture*>(CManagement::GetInstance()->Clone_Component(SCENE_STAGE, pComponentTextureTag));
	if (m_pAlphaTextureCom == nullptr)
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect::SetUp_BaseTexture(const _tchar * pComponentTextureTag)
{
	if (!lstrcmp(pComponentTextureTag, L""))
		return E_FAIL;

	lstrcpy(m_EffectData.szBaseTextureTag, pComponentTextureTag);

	//For. Texture;
	m_pBaseTextureCom = dynamic_cast<CTexture*>(CManagement::GetInstance()->Clone_Component(SCENE_STAGE, pComponentTextureTag));
	if (m_pBaseTextureCom == nullptr)
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect::SetUp_NoiseTexture(const _tchar * pComponentTextureTag)
{
	if (!lstrcmp(pComponentTextureTag, L""))
		return E_FAIL;

	lstrcpy(m_EffectData.szNoiseTextureTag, pComponentTextureTag);

	//For. Texture;
	m_pNoiseTextureCom = dynamic_cast<CTexture*>(CManagement::GetInstance()->Clone_Component(SCENE_STAGE, pComponentTextureTag));
	if (m_pNoiseTextureCom == nullptr)
		return E_FAIL;

	return S_OK;
}

void CEffect::SetUp_Position_Interpolation()
{
	m_pPosition_Interploation->Setup(&m_EffectData.vStart_Position, &m_EffectData.vEnd_Position,
		m_EffectData.fEndTime - m_EffectData.fFadeOut - m_EffectData.fStartTime);
}

void CEffect::SetUp_Rotation_Interpolation()
{

}

void CEffect::SetUp_Scale_Interpolation()
{
	m_pScale_Interploation->Setup(&m_EffectData.vStart_Scale, &m_EffectData.vEnd_Scale,
		m_EffectData.fEndTime - m_EffectData.fFadeOut - m_EffectData.fStartTime);
}



HRESULT CEffect::Ready_Effect()
{
	if(FAILED(Ready_Component()))
	{
		return E_FAIL;
	}

	m_pPosition_Interploation = CLinear_Interpolation::Create();
	m_pScale_Interploation = CLinear_Interpolation::Create();
	m_vRotation = _vec3(0.f, 0.f, 0.f);

	return S_OK;
}

_int CEffect::Update_Effect(const _float & fTimeDelta)
{



	return 0;
}

_int CEffect::LastUpdate_Effect(const _float & fTimeDelta)
{
	

	return 0;
}

void CEffect::Render_Effect(LPD3DXEFFECT pEffect, const _matrix* pmatParent)
{

}

void CEffect::SetUp_EffectDataFromFile(const EFFECTDATA & EffectData)
{
	m_EffectData = EffectData;
}

HRESULT CEffect::Ready_Component()
{
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();
	{
		//For. Transform;
		m_pTransformCom = dynamic_cast<CTransform*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Transform"));
		if(m_pTransformCom == nullptr)
			return E_FAIL;
	}
	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CEffect::SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _matrix* pmatParent)
{
	if (pEffect == nullptr)
		return E_FAIL;

 	pEffect->AddRef();

	pEffect->SetFloat("g_fTimeAcc", m_fTimeAcc);
	pEffect->SetFloat("g_fUSpeed", m_EffectData.fUSpeed);
	pEffect->SetFloat("g_fVSpeed", m_EffectData.fVSpeed);


	pEffect->SetInt("g_iWidth", m_EffectData.iAlpha_Width);
	pEffect->SetInt("g_iHeight", m_EffectData.iAlpha_Height);
	pEffect->SetInt("g_iAnimationX", m_iAnimationX);
	pEffect->SetInt("g_iAnimationY", m_iAnimationY);

	pEffect->SetVector("g_vAlpha", &_vec4(m_fAlpha, m_fAlpha, m_fAlpha, m_fAlpha));

	if (FAILED(m_pTransformCom->SetUp_OnShader_MultiplyParentMatrix(pEffect, pmatParent, "g_matWorld")))
		return E_FAIL;


	if (m_EffectData.iPassIndex == 1 || m_EffectData.iPassIndex == 3)
	{
		if (FAILED(m_pAlphaTextureCom->SetUp_OnShader(pEffect, "g_AlphaTexture")))
			return E_FAIL;
	}
	if (m_EffectData.iPassIndex == 2)
	{
		if (FAILED(m_pAlphaTextureCom->SetUp_OnShader(pEffect, "g_AlphaTexture")))
			return E_FAIL;
		if (FAILED(m_pBaseTextureCom->SetUp_OnShader(pEffect, "g_BaseTexture")))
			return E_FAIL;
	}

	pEffect->SetVector("g_vMaterial", &_vec4(1.f, 1.f, 1.f, 1.f));

	_matrix matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);


	Safe_Release(pEffect);

	return S_OK;
}


void CEffect::Free()
{
	
	Safe_Release(m_pPosition_Interploation);
	Safe_Release(m_pScale_Interploation);

	Safe_Release(m_pAlphaTextureCom);
	Safe_Release(m_pBaseTextureCom);
	Safe_Release(m_pNoiseTextureCom);
	Safe_Release(m_pTransformCom);

	Safe_Release(m_pDevice);

}
