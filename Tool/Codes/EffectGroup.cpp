#include "stdafx.h"
#include "..\Headers\EffectGroup.h"

#include "Management.h"

#include "EffectTab.h"
_USING(Client)

CEffectGroup::CEffectGroup(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
	, m_pTransformCom(nullptr)
	,m_pShaderCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_bActivate(false)
{
}

CEffectGroup::CEffectGroup(const CEffectGroup & rhs)
	:CGameObject(rhs)
	, m_pTransformCom(nullptr)
	, m_pShaderCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_bActivate(false)
{
}

HRESULT CEffectGroup::Ready_Prototype()
{
	return S_OK;
}

HRESULT CEffectGroup::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

_int CEffectGroup::Update_GameObject(const _float & fTimeDelta)
{
	if (!m_bActivate)
		return 0;

	m_fTimeAcc += fTimeDelta;
	if (m_fTimeAcc > m_EffectrGroup_Data.fLifeTime)
	{
		m_fTimeAcc = 0.f;
		m_bActivate = false;
	}

	for (auto& pEffectMesh : m_vecEffectMesh)
	{
		if (pEffectMesh != nullptr)
			pEffectMesh->Update_Effect(fTimeDelta);
	}
	for (auto& pEffectRect : m_vecEffectRect)
	{
		if (pEffectRect != nullptr)
			pEffectRect->Update_Effect(fTimeDelta);
	}
	for (auto& pEffectParticle : m_vecEffectParticle)
	{
		if (pEffectParticle != nullptr)
			pEffectParticle->Update_Effect(fTimeDelta);
	}
	return _int();
}

_int CEffectGroup::LastUpdate_GameObject(const _float & fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA,this);

	for (auto& pEffectMesh : m_vecEffectMesh)
	{
		if (pEffectMesh != nullptr)
			pEffectMesh->LastUpdate_Effect(fTimeDelta);
	}
	for (auto& pEffectRect : m_vecEffectRect)
	{
		if (pEffectRect != nullptr)
			pEffectRect->LastUpdate_Effect(fTimeDelta);
	}
	for (auto& pEffectParticle : m_vecEffectParticle)
	{
		if (pEffectParticle != nullptr)
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
		if(pEffectMesh != nullptr)
			pEffectMesh->Render_Effect(pEffect);
	}


	for (auto& pEffectRect : m_vecEffectRect)
	{
		if (pEffectRect != nullptr)
			pEffectRect->Render_Effect(pEffect);
	}


	for (auto& pEffectParticle : m_vecEffectParticle)
	{
		if (pEffectParticle != nullptr)
			pEffectParticle->Render_Effect(pEffect);
	}

	pEffect->End();
}

void CEffectGroup::Set_Activate(_bool bActivate)
{
	m_bActivate = bActivate;

	for (auto& pEffectMesh : m_vecEffectMesh)
	{
		pEffectMesh->Set_Activate(true);
		pEffectMesh->SetUp_Position_Interpolation();
		pEffectMesh->SetUp_Scale_Interpolation();
		pEffectMesh->Initalize_Alpha();
	}

	for (auto& pEffectRect : m_vecEffectRect)
	{
		pEffectRect->Set_Activate(true);
		pEffectRect->SetUp_Position_Interpolation();
		pEffectRect->SetUp_Scale_Interpolation();
		pEffectRect->Initalize_Alpha();
	}

	for (auto& pEffectParticle : m_vecEffectParticle)
	{
		pEffectParticle->Set_Animation();
		pEffectParticle->Initalize_Alpha();
	}
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

void CEffectGroup::Save_EffectData(HANDLE & hFile, _ulong & dwByte)
{
	//Tag
	WriteFile(hFile, &m_EffectrGroup_Data, sizeof(EFFECTGROUP_DATA), &dwByte, nullptr);

	_int iEffectNum = m_vecEffectMesh.size();
	WriteFile(hFile, &iEffectNum, sizeof(_int), &dwByte, nullptr);
	for (auto& pEffectMesh : m_vecEffectMesh)
	{
		pEffectMesh->Save_EffectData(hFile, dwByte);
	}

	iEffectNum = m_vecEffectRect.size();
	WriteFile(hFile, &iEffectNum, sizeof(_int), &dwByte, nullptr);
	for (auto& pEffectRect : m_vecEffectRect)
	{
		pEffectRect->Save_EffectData(hFile, dwByte);
	}

	iEffectNum = m_vecEffectParticle.size();
	WriteFile(hFile, &iEffectNum, sizeof(_int), &dwByte, nullptr);
	for (auto& pEffectParticle : m_vecEffectParticle)
	{
		pEffectParticle->Save_EffectData(hFile, dwByte);
	}
}

void CEffectGroup::Load_EffectDataFromFile(HANDLE & hFile, _ulong & dwByte, CEffectTab* pEffectTab)
{

	_int iEffectNum = m_vecEffectMesh.size();
	ReadFile(hFile, &iEffectNum, sizeof(_int), &dwByte, nullptr);
	for(_int i = 0 ; i< iEffectNum; ++i)
	{
		CEffect_Mesh* pEffectMesh =  CEffect_Mesh::Create(m_pDevice);
		pEffectMesh->Load_EffectDataFromFile(hFile, dwByte, pEffectTab);
		m_vecEffectMesh.push_back(pEffectMesh);

		pEffectTab->Add_EffectMesh_List();
	}

	iEffectNum = m_vecEffectRect.size();
	ReadFile(hFile, &iEffectNum, sizeof(_int), &dwByte, nullptr);
	for (_int i = 0; i < iEffectNum; ++i)
	{
		CEffect_Rect* pEffectRect = CEffect_Rect::Create(m_pDevice);
		pEffectRect->Load_EffectDataFromFile(hFile, dwByte, pEffectTab);
		m_vecEffectRect.push_back(pEffectRect);

		pEffectTab->Add_EffectRect_List();
	}

	iEffectNum = m_vecEffectParticle.size();
	ReadFile(hFile, &iEffectNum, sizeof(_int), &dwByte, nullptr);
	for (_int i = 0; i < iEffectNum; ++i)
	{
		CEffect_Particle* pEffectParticle = CEffect_Particle::Create(m_pDevice);
		pEffectParticle->Load_EffectDataFromFile(hFile, dwByte,pEffectTab);
		m_vecEffectParticle.push_back(pEffectParticle);

		pEffectTab->Add_EffectParticle_List();
	}
}

void CEffectGroup::Remove_EffectMesh(const _uint & iIndex)
{
	auto& iter = m_vecEffectMesh.begin();
	for (_uint i = 0; i < iIndex; ++i)
		iter++;

	Safe_Release(*iter);
	m_vecEffectMesh.erase(iter);
}

void CEffectGroup::Remove_EffectRect(const _uint & iIndex)
{
	auto& iter = m_vecEffectRect.begin();
	for (_uint i = 0; i < iIndex; ++i)
		iter++;

	Safe_Release(*iter);
	m_vecEffectRect.erase(iter);
}

void CEffectGroup::Remove_EffectParticle(const _uint & iIndex)
{
	auto& iter = m_vecEffectParticle.begin();
	for (_uint i = 0; i < iIndex; ++i)
		iter++;

	Safe_Release(*iter);
	m_vecEffectParticle.erase(iter);
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
		if (FAILED(Add_Component(L"m_pRendererCom", m_pRendererCom)))
			return E_FAIL;

		//For. Shader;
		m_pShaderCom = dynamic_cast<CShader*>(pManagement->Clone_Component(SCENE_EDIT, L"Component_Shader_Effect"));
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

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);

	CGameObject::Free();
}
