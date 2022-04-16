#include "stdafx.h"
#include "..\Headers\Effect_Particle.h"

#include "Management.h"

//#include <iostream>

_USING(Client)

CEffect_Particle::CEffect_Particle(LPDIRECT3DDEVICE9 pDevice)
	:CEffect(pDevice)
	, m_bRecycle(false)
{
}


void CEffect_Particle::Set_Animation()
{
	m_bIsActivate = true;

	for (_int i = 0; i <m_ParticleData.iParticleNum; ++i)
	{
		Set_Particle(i);
	}
}



void CEffect_Particle::SetUp_ParticleInfo(_int iParicleNum)
{
	m_ParticleData.iParticleNum = iParicleNum;

	m_pParticle = new PARTICLE[m_ParticleData.iParticleNum];
	for (_int i = 0; i < m_ParticleData.iParticleNum; ++i)
	{
		m_pParticle[i].pBufferCom = CBuffer_RcTex::Create(m_pDevice);
		m_pParticle[i].pTransformCom = CTransform::Create(m_pDevice);
		m_pParticle[i].fAlpha = 1.f;
	}
}

void CEffect_Particle::SetUp_ParticleData(const EFFECTDATA & EffectData, const EFFECTPARTICLE_DATA & Effect_ParticleData, _bool bRecycle)
{
	m_EffectData = EffectData;
	m_ParticleData = Effect_ParticleData;
	m_bRecycle = bRecycle;

	m_pTransformCom->Set_WorldMatrix(&m_EffectData.matTransform);

	SetUp_ParticleInfo(m_ParticleData.iParticleNum);
	SetUp_AlphaTexutre(m_EffectData.szAlphaTextureTag);
	SetUp_BaseTexture(m_EffectData.szBaseTextureTag);
	SetUp_NoiseTexture(m_EffectData.szNoiseTextureTag);


	for (_int i = 0; i < m_ParticleData.iParticleNum; ++i)
	{
		m_pParticle[i].pTransformCom->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());
	}
}



HRESULT CEffect_Particle::SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _matrix* pmatParent, const  _int& iIndex)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();
	{
	//pEffect->SetFloat("g_fBrightness", 1.0f);

	pEffect->SetFloat("g_fTimeAcc", m_fTimeAcc);
	pEffect->SetFloat("g_fUSpeed", m_EffectData.fUSpeed);
	pEffect->SetFloat("g_fVSpeed", m_EffectData.fVSpeed);

	pEffect->SetInt("g_iWidth", m_EffectData.iAlpha_Width);
	pEffect->SetInt("g_iHeight", m_EffectData.iAlpha_Height);

	if (m_bRecycle)
	{
		//Animation Setting
		pEffect->SetInt("g_iAnimationX", m_pParticle[iIndex].iAnimationX);
		pEffect->SetInt("g_iAnimationY", m_pParticle[iIndex].iAnimationY);
		//Alhpa Setting
		pEffect->SetVector("g_vAlpha", &_vec4(m_pParticle[iIndex].fAlpha, m_pParticle[iIndex].fAlpha, m_pParticle[iIndex].fAlpha, m_pParticle[iIndex].fAlpha));
	}
	else
	{
		//Animation Setting
		pEffect->SetInt("g_iAnimationX", m_iAnimationX);
		pEffect->SetInt("g_iAnimationY", m_iAnimationY);

		//Alpha Setting
		pEffect->SetVector("g_vAlpha", &_vec4(m_fAlpha, m_fAlpha, m_fAlpha, m_fAlpha));
	}

	if (FAILED(m_pParticle[iIndex].pTransformCom->
		SetUp_OnShader_MultiplyParnetMatrix_BillBoadMatrix(pEffect, pmatParent, "g_matWorld")))
		return E_FAIL;


	if (m_EffectData.iPassIndex == 1 || m_EffectData.iPassIndex == 3)
	{
		if (FAILED(m_pAlphaTextureCom->SetUp_OnShader(pEffect, "g_AlphaTexture" /*_uint(m_fFrame)*/)))
			return E_FAIL;
	}
	if (m_EffectData.iPassIndex == 2)
	{
		if (FAILED(m_pAlphaTextureCom->SetUp_OnShader(pEffect, "g_AlphaTexture")))
			return E_FAIL;
		if (FAILED(m_pBaseTextureCom->SetUp_OnShader(pEffect, "g_BaseTexture" /*_uint(m_fFrame)*/)))
			return E_FAIL;
	}


	pEffect->SetVector("g_vAlpha", &_vec4(m_pParticle[iIndex].fAlpha, m_pParticle[iIndex].fAlpha, m_pParticle[iIndex].fAlpha, m_pParticle[iIndex].fAlpha));

	pEffect->SetVector("g_vMaterial", &_vec4(1.f, 1.f, 1.f, 1.f));
	
	_matrix matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	}
	Safe_Release(pEffect);
	return S_OK;
}



void CEffect_Particle::Set_Inital_ParticleInfo(EFFECTPARTICLE_DATA& ParticleData)
{
	m_ParticleData.fMax_Theta				=ParticleData.fMax_Theta;
	m_ParticleData.fMin_Theta				=ParticleData.fMin_Theta;
	m_ParticleData.fMin_Phi					=ParticleData.fMin_Phi;
	m_ParticleData.fMax_Phi					=ParticleData.fMax_Phi;
	m_ParticleData.fInit_Speed				=ParticleData.fInit_Speed;
	m_ParticleData.fSpeed_Range				=ParticleData.fSpeed_Range;
	m_ParticleData.fInit_LifeTime			=ParticleData.fInit_LifeTime;
	m_ParticleData.fLifeTime_Range			=ParticleData.fLifeTime_Range;
	m_ParticleData.fFrame_Const				=ParticleData.fFrame_Const;
	m_ParticleData.vInital_Acceleration		=ParticleData.vInital_Acceleration;
	Set_Animation();
}

void CEffect_Particle::Get_Inital_ParticleInfo(EFFECTPARTICLE_DATA& ParticleData)
{
	ParticleData.fMax_Theta					=m_ParticleData.fMax_Theta;
	ParticleData.fMin_Theta					=m_ParticleData.fMin_Theta;
	ParticleData.fMin_Phi					=m_ParticleData.fMin_Phi;
	ParticleData.fMax_Phi					=m_ParticleData.fMax_Phi;
	ParticleData.fInit_Speed				=m_ParticleData.fInit_Speed;
	ParticleData.fSpeed_Range				=m_ParticleData.fSpeed_Range;
	ParticleData.fInit_LifeTime				=m_ParticleData.fInit_LifeTime;
	ParticleData.fLifeTime_Range			=m_ParticleData.fLifeTime_Range;
	ParticleData.fFrame_Const				=m_ParticleData.fFrame_Const;
	ParticleData.vInital_Acceleration		=m_ParticleData.vInital_Acceleration;
	
}

void CEffect_Particle::Set_Particle(_int iIndex)
{
	PARTICLE*	pParticle = &m_pParticle[iIndex];

	_float	fTheta;		// ���� ��
	_float	fPhi;		// ���� ��
	_float	fSpeedSize;		// �ӵ� ũ��
	_float	fLife;

						// �ʱ� ���ӵ�
	pParticle->vInital_Acceleration = m_ParticleData.vInital_Acceleration;

	//�ʱ� �ӵ��� ��ġ�� �����ϱ� ���� ����
	if (m_ParticleData.fMax_Theta == 0.f)
		m_ParticleData.fMax_Theta = 1.f;
	fTheta = _float(rand() % (_int)m_ParticleData.fMax_Theta);
	fTheta += m_ParticleData.fMin_Theta;

	if (m_ParticleData.fMax_Phi == 0.f)
		m_ParticleData.fMax_Phi = 1.f;
	fPhi = _float(rand() % (_int)m_ParticleData.fMax_Phi);
	fPhi += m_ParticleData.fMin_Phi;

	if (m_ParticleData.fSpeed_Range == 0.f)
		m_ParticleData.fSpeed_Range = 1.f;
	fSpeedSize = m_ParticleData.fInit_Speed + rand() % (_int)m_ParticleData.fSpeed_Range;
	fSpeedSize *= 0.1f;

	// �������� ����
	fTheta = D3DXToRadian(fTheta);
	fPhi = D3DXToRadian(fPhi);

	// �ʱ� �ӵ�
	pParticle->vInital_Velocity.x = fSpeedSize * sinf(fTheta) * sinf(fPhi);
	pParticle->vInital_Velocity.y = fSpeedSize * cosf(fTheta);
	pParticle->vInital_Velocity.z = fSpeedSize * sinf(fTheta) * cosf(fPhi);
	
	// �ʱ� ��ġ		
	//pParticle->vInital_Position.x = 0.f;
	//pParticle->vInital_Position.y = 0.f;
	//pParticle->vInital_Position.z = 0.f;
	pParticle->vInital_Position = *m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);

	// ź�� ��� ����
	pParticle->fElst = (50 + rand() % 51)*0.01f;

	// �������� ���
	pParticle->fDamp = (100 + rand() % 101)*0.00001F;

	// �ʱ� ��ġ, �ӵ�, ���ӵ��� ������ ������ �ʱ� ������ ����
	pParticle->vCurrent_Position = pParticle->vInital_Position;
	pParticle->vCurrent_Velocity = pParticle->vInital_Velocity;
	pParticle->vCurrent_Acceleration = pParticle->vCurrent_Acceleration;


	// ������ ���� ���	
	pParticle->bLive = true;

	if (m_ParticleData.fLifeTime_Range == 0.f)
		m_ParticleData.fLifeTime_Range = 1.f;
	fLife = m_ParticleData.fInit_LifeTime + rand() % (_int)m_ParticleData.fLifeTime_Range;
	fLife *= 0.1f;
	pParticle->fMaxLife = pParticle->fLife = fLife;

	pParticle->fAlpha = 1.f;
	pParticle->fFrame = 0.f;

}


HRESULT CEffect_Particle::Ready_Effect()
{
	if (FAILED(CEffect::Ready_Effect()))
		return E_FAIL;


	m_ParticleData.iParticleNum = 0;
	m_pParticle = nullptr;

	m_ParticleData.fMax_Theta = 60.f;
	m_ParticleData.fMin_Theta = 10.f;
	m_ParticleData.fMin_Phi = 1.f;
	m_ParticleData.fMax_Phi = 360.f;
	m_ParticleData.fInit_Speed = 1.f;
	m_ParticleData.fSpeed_Range = 5.f;
	m_ParticleData.fInit_LifeTime = 10.f;
	m_ParticleData.fLifeTime_Range = 10.f;
	m_ParticleData.fFrame_Const = 0.5f;
	m_ParticleData.vInital_Acceleration = _vec3(0.f, -1.f, 0.f);

	return S_OK;
}

_int CEffect_Particle::Update_Effect(const _float & fTimeDelta)
{
	//Ȱ��ȭ���� ������ ������Ʈ ���� �ʴ´�.
	if (!m_bIsActivate)
		return 0;

	//���� ����
	if (!m_bRecycle)
	{
		m_fTimeAcc += fTimeDelta;
		if (m_fTimeAcc > m_EffectData.fEndTime)
		{
			m_fAlpha = m_EffectData.fAlpha;
			m_fTimeAcc = 0.f;
			m_bIsActivate = false;
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
				//else if (m_fTimeAcc > m_EffectData.m_fEndTime)
			{
				m_fAlpha -= m_EffectData.fAlpha * fTimeDelta / m_EffectData.fFadeOut;
			}
			else
			{
				m_fAlpha = m_EffectData.fAlpha;
			}


			//������ ������Ʈ
			_float fFrameTime = fTimeDelta * m_ParticleData.fFrame_Const;
			m_fFrame += m_EffectData.iAlpha_Width *m_EffectData.iAlpha_Height * fFrameTime;
			if (m_EffectData.iAlpha_Width *m_EffectData.iAlpha_Height <= m_fFrame)
				m_fFrame = 0.f;

			m_iAnimationX = (_int)m_fFrame % m_EffectData.iAlpha_Width;
			m_iAnimationY = (_int)m_fFrame / m_EffectData.iAlpha_Width;


			for (_int i = 0; i < m_ParticleData.iParticleNum; ++i)
			{
				PARTICLE*	pParticle = &m_pParticle[i];
				if (pParticle->bLive == FALSE)
					continue;

				//���������� ���Ѵ�.
				_vec3		vAriResistance = pParticle->vCurrent_Velocity;	//���������� ���⺤��
				_float		fLenV = D3DXVec3LengthSq(&vAriResistance);		//�ӵ��� ����(Vx*Vx + Vy*Vy + Vz*Vz)ũ�� ����

																			// ���������� ���� ���͸� ���Ѵ�.
				D3DXVec3Normalize(&vAriResistance, &vAriResistance);

				//�̵��ӵ��� �ݴ�����
				vAriResistance *= 1.f;

				// �ӷ����� * ���� ���� ����� ����.
				vAriResistance *= fLenV * pParticle->fDamp;

				// 1. ���ӵ��� ���������� ���Ѵ�.
				pParticle->vCurrent_Acceleration = pParticle->vInital_Acceleration + vAriResistance;

				// 2. ���� �ӵ� ����
				pParticle->vCurrent_Velocity += pParticle->vCurrent_Acceleration *fFrameTime; //fTimeDelta;

				 // 3. ���� ��ġ ����
				pParticle->vCurrent_Position += pParticle->vCurrent_Velocity * fFrameTime;// fTimeDelta;

				pParticle->pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &pParticle->vCurrent_Position);

				// 4. ��谪 ����
				if (pParticle->vCurrent_Position.y < 0.f)
				{
					pParticle->bLive = FALSE;
				}
			}


			_matrix matView;
			m_pDevice->GetTransform(D3DTS_VIEW, &matView);

			_vec3 vCamZ(matView._13, matView._23, matView._33);

			for (_int i = 0; i < m_ParticleData.iParticleNum; ++i)
			{
				PARTICLE* pPariticle = &m_pParticle[i];

				_vec3	vCurrnet_Position = pPariticle->vCurrent_Position;

				// ī�޶��� Z��� ��ƼŬ�� ��ġ�� ����
				pPariticle->ViewZ = D3DXVec3Dot(&vCurrnet_Position, &vCamZ);
			}

			// Sorting
			qsort(m_pParticle
				, m_ParticleData.iParticleNum
				, sizeof(PARTICLE)
				, (_int(*) (const void *, const void *)) SortFnc);
		}
	}
	else//������
	{
		_float fFrameTime = fTimeDelta * m_ParticleData.fFrame_Const;

		for (_int i = 0; i < m_ParticleData.iParticleNum; ++i)
		{
			PARTICLE*	pParticle = &m_pParticle[i];
			//if (pParticle->bLive == FALSE)
			//	continue;

			//���� ��ƼŬ ���
			for (_int i = 0; i < m_ParticleData.iParticleNum; ++i)
			{
				PARTICLE* pParticle = &m_pParticle[i];

				if (pParticle->bLive == TRUE)
					continue;

				this->Set_Particle(i);
			}

			//���������� ���Ѵ�.
			_vec3		vAriResistance = pParticle->vCurrent_Velocity;	//���������� ���⺤��
			_float		fLenV = D3DXVec3LengthSq(&vAriResistance);		//�ӵ��� ����(Vx*Vx + Vy*Vy + Vz*Vz)ũ��

																		// ���������� ���� ���͸� ���Ѵ�.
			D3DXVec3Normalize(&vAriResistance, &vAriResistance);

			//�̵��ӵ��� �ݴ�����
			vAriResistance *= 1.f;

			// �ӷ����� * ���� ���� ����� ����.
			vAriResistance *= fLenV * pParticle->fDamp;
			//vAriResistance = _vec3(0.f, 0.f, 0.f);

			// 1. ���ӵ��� ���������� ���Ѵ�.
			pParticle->vCurrent_Acceleration = pParticle->vInital_Acceleration + vAriResistance;

			// 2. ���� �ӵ� ����
			pParticle->vCurrent_Velocity += pParticle->vCurrent_Acceleration *fFrameTime; //fTimeDelta;

																						  // 3. ���� ��ġ ����
			pParticle->vCurrent_Position += pParticle->vCurrent_Velocity * fFrameTime;// fTimeDelta;

			pParticle->pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &pParticle->vCurrent_Position);

		}


		//// 2. ��ƼŬ�� ������ �����Ѵ�.
		for (_int i = 0; i < m_ParticleData.iParticleNum; ++i)
		{
			PARTICLE* pParticle = &m_pParticle[i];

			if (pParticle->bLive == FALSE)
				continue;

			pParticle->fLife -= /*pParticle->fFade*/fFrameTime;
			//Animation
			//������ ������Ʈ
			pParticle->fFrame += m_EffectData.iAlpha_Width *m_EffectData.iAlpha_Height *fFrameTime / pParticle->fMaxLife;
			if (m_EffectData.iAlpha_Width *m_EffectData.iAlpha_Height <= pParticle->fFrame)
				pParticle->fFrame = 0.f;

			pParticle->iAnimationX = (_int)pParticle->fFrame % m_EffectData.iAlpha_Width;
			pParticle->iAnimationY = (_int)pParticle->fFrame / m_EffectData.iAlpha_Height;

			//Alhpa ���
			if (pParticle->fLife < 1.f)
				pParticle->fAlpha = pParticle->fLife;


			if (pParticle->fLife <= 0.f)
			{
				pParticle->bLive = FALSE;
				continue;
			}
		}


		_matrix matView;
		m_pDevice->GetTransform(D3DTS_VIEW, &matView);

		_vec3 vCamZ(matView._13, matView._23, matView._33);

		for (_int i = 0; i < m_ParticleData.iParticleNum; ++i)
		{
			PARTICLE* pPariticle = &m_pParticle[i];

			_vec3	vCurrnet_Position = pPariticle->vCurrent_Position;

			// ī�޶��� Z��� ��ƼŬ�� ��ġ�� ����
			pPariticle->ViewZ = D3DXVec3Dot(&vCurrnet_Position, &vCamZ);
		}

		// Sorting
		qsort(m_pParticle
			, m_ParticleData.iParticleNum
			, sizeof(PARTICLE)
			, (_int(*) (const void *, const void *)) SortFnc);
	}

	return 0;
}

_int CEffect_Particle::LastUpdate_Effect(const _float & fTimeDelta)
{

	return _int();
}

void CEffect_Particle::Render_Effect(LPD3DXEFFECT pEffect, const _matrix* pmatParent)
{
	if (pEffect == nullptr)
		return;

	pEffect->AddRef();

	pEffect->BeginPass(m_EffectData.iPassIndex);
	for (_int i = 0; i < m_ParticleData.iParticleNum; ++i)
	{
		PARTICLE* pPariticle = &m_pParticle[i];
		if(pPariticle->bLive == false)
			continue;

		if (FAILED(SetUp_ConstantTable(pEffect, pmatParent, i)))
		{
			Safe_Release(pEffect);
			return;
		}

		pEffect->CommitChanges();

		m_pParticle[i].pBufferCom->Render_Buffer();
	}

	pEffect->EndPass();

	Safe_Release(pEffect);
}

void CEffect_Particle::Initalize_Effect()
{
	Set_Animation();
	Set_Alpha(0.f);
	m_fTimeAcc = 0.f;
}



CEffect_Particle * CEffect_Particle::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CEffect_Particle*	pInstance = new CEffect_Particle(pDevice);

	if (FAILED(pInstance->Ready_Effect()))
	{
		_MSG_BOX(L"CEffect_Mesh Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Particle::Free()
{
	if (m_pParticle)
	{
		delete[](m_pParticle);
		(m_pParticle) = NULL;
	}

	CEffect::Free();
}
