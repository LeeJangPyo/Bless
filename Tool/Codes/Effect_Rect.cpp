#include "stdafx.h"
#include "..\Headers\Effect_Rect.h"

#include "Management.h"

#include  <iostream>
_USING(Client)

CEffect_Rect::CEffect_Rect(LPDIRECT3DDEVICE9 pDevice)
	:CEffect(pDevice)
{

}



HRESULT CEffect_Rect::Ready_Effect()
{
	if (FAILED(CEffect::Ready_Effect()))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;


	return S_OK;
}

_int CEffect_Rect::Update_Effect(const _float & fTimeDelta)
{
	//Ȱ��ȭ���� ������ ������Ʈ ���� �ʴ´�.
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


	//���۽ð��� ������ ����
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


		//�̹��� ������ ����
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

_int CEffect_Rect::LastUpdate_Effect(const _float & fTimeDelta)
{
	/*if (m_pRendererCom == nullptr)
		return -1;

	Compute_ViewZ(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION));

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this);*/

	return _int();
}

void CEffect_Rect::Render_Effect(LPD3DXEFFECT pEffect)
{
	if (nullptr == m_pBufferCom)
		return;

	if (nullptr == pEffect)
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
		m_pBufferCom->Render_Buffer();

		pEffect->EndPass();

	}
	Safe_Release(pEffect);
}


void CEffect_Rect::Save_EffectData(HANDLE & hFile, _ulong & dwByte)
{
	m_EffectData.matTransform = *m_pTransformCom->Get_WorldMatrix();

	//Effect �⺻ ������
	WriteFile(hFile, &m_EffectData, sizeof(EFFECTDATA), &dwByte, nullptr);

	//EffectMesh ������
	//WriteFile(hFile, &m_EffectRect_Data, sizeof(EFFECTRECT_DATA), &dwByte, nullptr);
}

void CEffect_Rect::Load_EffectDataFromFile(HANDLE & hFile, _ulong & dwByte, CEffectTab* pEffectTab)
{
	//Effect �⺻ ������
	ReadFile(hFile, &m_EffectData, sizeof(EFFECTDATA), &dwByte, nullptr);
	
	m_pTransformCom->Set_WorldMatrix(&m_EffectData.matTransform);

	//EffectRect ������
	//ReadFile(hFile, &m_EffectRect_Data, sizeof(EFFECTRECT_DATA), &dwByte, nullptr);
	SetUp_AlphaTexutre(m_EffectData.szAlphaTextureTag);
	SetUp_BaseTexture(m_EffectData.szBaseTextureTag);
	SetUp_NoiseTexture(m_EffectData.szNoiseTextureTag);

}

HRESULT CEffect_Rect::Ready_Component()
{

	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();

	m_pBufferCom = dynamic_cast<CBuffer_RcTex*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Buffer_RcTex"));
	if(m_pBufferCom == nullptr)
		return E_FAIL;

	Safe_Release(pManagement);

	return S_OK;
}

CEffect_Rect * CEffect_Rect::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CEffect_Rect*	pInstance = new CEffect_Rect(pDevice);

	if (FAILED(pInstance->Ready_Effect()))
	{
		_MSG_BOX(L"CEffect_Rect Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Rect::Free()
{
	Safe_Release(m_pBufferCom);

	CEffect::Free();
}
