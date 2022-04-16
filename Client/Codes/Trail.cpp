#include "stdafx.h"
#include "..\Headers\Trail.h"

#include "Management.h"

#include "GameObject_Weapon.h"
_USING(Client)

CTrail::CTrail(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, m_pTexutre(nullptr)
	, m_pShader(nullptr)
	, m_pRenderer(nullptr)
	, m_iTrailIndex(0)
	, m_dwVtxSize(0)
	, m_dwVtxCnt(0)
	, m_dwVtxFVF(0)
	, m_bIsActivate(false)
	, m_fTimeAcc(0.f)
	, m_fDelay(0.f)
{
	D3DXMatrixIsIdentity(&m_matWorld);
}

CTrail::CTrail(const CTrail & rhs)
	:CGameObject(rhs)
	, m_iTrailIndex(0)
	, m_dwVtxSize(0)
	, m_dwVtxCnt(0)
	, m_dwVtxFVF(0)
	, m_bIsActivate(false)
	, m_fTimeAcc(0.f)
	, m_fDelay(0.f)
{
	
}


HRESULT CTrail::Ready_Prototype()
{
	return S_OK;
}

HRESULT CTrail::Ready_GameObject()
{
	D3DXMatrixIsIdentity(&m_matWorld);
	ZeroMemory(&m_matWorld, sizeof(_matrix));
	for (_int i = 0; i < 4; ++i)
		m_matWorld.m[i][i] = 1.f;

	// Set Component
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();
	{

	//For. Texture
	m_pTexutre = dynamic_cast<CTexture*>(pManagement->Clone_Component(SCENE_STAGE, L"Component_Texture_Trail"));
	if (m_pTexutre == nullptr)
		return E_FAIL;

	//For. Shader
	m_pShader = dynamic_cast<CShader*>(pManagement->Clone_Component(SCENE_STAGE, L"Component_Shader_Trail"));
	if (m_pTexutre == nullptr)
		return E_FAIL;

	//For. Renderer
	m_pRenderer = dynamic_cast<CRenderer*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Renderer"));
	if (m_pRenderer == nullptr)
		return E_FAIL;

	}
	Safe_Release(pManagement);


	//Trail Array Setting
	m_iMaxTrail = 1000;
	m_parrTrailPos = new TRAILPOS[m_iMaxTrail / 2];
	m_parrInterpolation_TrailPos = new TRAILPOS[m_iMaxTrail];

	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = m_iMaxTrail * 2;
	m_dwVtxFVF = VTXFVF_TEX;

	if (FAILED(m_pDevice->CreateVertexBuffer(
		m_dwVtxSize * m_dwVtxCnt,		// 정점 버퍼의 사이즈(바이트 단위)
		0,				// 정점 사용방식 : 버퍼를 만들 때 어떤 버퍼를 만들 것인가?(0인 경우 정적 버퍼)
		m_dwVtxFVF,						// 사용하는 버퍼 스타일
		D3DPOOL_MANAGED,				// 메모리 풀의 사용 방식
		&m_pVB,							// 버텍스 버퍼의 접근권한을 갖고 있는 객체 주소값
		NULL)))							// 버퍼 만들 때 예약상태
		return E_FAIL;

	return S_OK;
}

_int CTrail::Update_GameObject(const _float & fTimeDelta)
{
	if (!m_bIsActivate)
		return 0;

	if (m_iTrailIndex > m_iMaxTrail)
		return 0;

	TRAILPOS	trailPos;
	trailPos.vNearPosition = *m_pTrail_Weapon->Get_NearPosition();
	trailPos.vFarPosition = *m_pTrail_Weapon->Get_FarPosition();

	m_parrTrailPos[m_iTrailIndex++] = trailPos;

	_int iIndex = 0;

	for (_int i = 0; i < m_iTrailIndex; ++i)
	{
		for (_int j = 0; j < 10; ++j)
		{
			TRAILPOS interpolation_TrailPos;

			_int a = i - 1 <= 0 ? 0 : i - 1;
			_int b = i + 0;
			_int c = i + 1 > m_iTrailIndex - 1 ? m_iTrailIndex - 1 : i + 1;
			_int d = i + 2 > m_iTrailIndex - 1 ? m_iTrailIndex - 1 : i + 2;

			D3DXVec3CatmullRom(&interpolation_TrailPos.vNearPosition,
				&m_parrTrailPos[a].vNearPosition,
				&m_parrTrailPos[b].vNearPosition,
				&m_parrTrailPos[c].vNearPosition,
				&m_parrTrailPos[d].vNearPosition, j / 5.f);

			D3DXVec3CatmullRom(&interpolation_TrailPos.vFarPosition,
				&m_parrTrailPos[a].vFarPosition,
				&m_parrTrailPos[b].vFarPosition,
				&m_parrTrailPos[c].vFarPosition,
				&m_parrTrailPos[d].vFarPosition, j / 5.f);

			m_parrInterpolation_TrailPos[iIndex++] = interpolation_TrailPos;
		}
	}

	VTXTEX*		pVtxTex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);
	for (_int i = 0; i < iIndex; i += 2)
	{
		pVtxTex[i].vPos = m_parrInterpolation_TrailPos[i].vNearPosition;
		pVtxTex[i].vNormal = _vec3(0.f, 1.f, 0.f);
		pVtxTex[i].vTex = _vec2((_float)i / iIndex, 0.f);

		pVtxTex[i + 1].vPos = m_parrInterpolation_TrailPos[i].vFarPosition;
		pVtxTex[i + 1].vNormal = _vec3(0.f, 1.f, 0.f);
		pVtxTex[i + 1].vTex = _vec2((_float)i / iIndex, 1.f);
	}

	m_pVB->Unlock();

	//List Clear
	//Interpolation_TrailPosList.clear();

	return _int();
}

_int CTrail::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (!m_bIsActivate)
		return 0;

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHA,this);

	return _int();
}

void CTrail::Render_GameObject()
{
	LPD3DXEFFECT pEffect = m_pShader->Get_EffectHandle();
	if (pEffect == nullptr)
		return;

	pEffect->AddRef();

	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);
	{
		m_pDevice->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
		m_pDevice->SetFVF(m_dwVtxFVF);
		m_pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, m_iTrailIndex * 2 *5 - 2);//m_dwVtxCnt - 2);
	}
	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}



HRESULT CTrail::Begin_Trail(CGameObject_Weapon* pTrailWeapon)
{
	m_bIsActivate = true;

	m_pTrail_Weapon = pTrailWeapon;

	VTXTEX*		pVtxTex = NULL;
	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);
	for (_int i = 0; i < m_iMaxTrail; i += 2)
	{
		pVtxTex[i].vPos = *m_pTrail_Weapon->Get_NearPosition();
		pVtxTex[i].vNormal = _vec3(0.f, 1.f, 0.f);
		pVtxTex[i].vTex = _vec2((_float)i / m_iTrailIndex, 0.f);

		pVtxTex[i + 1].vPos = *m_pTrail_Weapon->Get_FarPosition();
		pVtxTex[i + 1].vNormal = _vec3(0.f, 1.f, 0.f);
		pVtxTex[i + 1].vTex = _vec2((_float)i / m_iTrailIndex, 1.f);
	}

	m_pVB->Unlock();
	return S_OK;
}

HRESULT CTrail::End_Trail()
{
	m_bIsActivate = false;
	m_iTrailIndex = 0;
	return S_OK;
}

HRESULT CTrail::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (pEffect == nullptr)
		return E_FAIL;

	pEffect->AddRef();

	m_pTexutre->SetUp_OnShader(pEffect, "g_BaseTexture");

	pEffect->SetMatrix("g_matWorld", &m_matWorld);

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

CTrail * CTrail::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTrail*		pInstance = new CTrail(pDevice);

	if (FAILED(pInstance->Ready_Prototype()))
	{
		_MSG_BOX(L"CTrail Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTrail::Clone_GameObject()
{
	CTrail*		pInstance = new CTrail(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX(L"CTrail Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTrail::Free()
{
	Safe_Delete_Array(m_parrTrailPos);
	Safe_Delete_Array(m_parrInterpolation_TrailPos);

	Safe_Release(m_pShader);
	Safe_Release(m_pTexutre);
	Safe_Release(m_pRenderer);

	Safe_Release(m_pVB);

	CGameObject::Free();
}
