#include "stdafx.h"
#include "..\Headers\UI_Frame.h"


#include "Management.h"
#include "Input_Device.h"

#include "UI_Mask.h"
#include "UI_Back.h"
#include "UI_Bar.h"
#include "UI_Icon.h"
#include "UI_IconMask.h"
#include "UI_BossBar.h"
#include "UI_BossMask.h"


_USING(Client)

CUI_Frame::CUI_Frame(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
	, m_pRendererCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_pShaderCom(nullptr)
	, m_pInput_Device(CInput_Device::GetInstance())
	, m_fCoolDown(0.f)
	, m_bIsCoolDown(false)
	, m_bActivate(true)
{
	m_pInput_Device->AddRef();
}

CUI_Frame::CUI_Frame(const CUI_Frame & rhs)
	:CGameObject(rhs)
	, m_pInput_Device(rhs.m_pInput_Device)
	, m_fCoolDown(0.f)
	, m_bIsCoolDown(false)
	, m_bActivate(true)
{
	m_pInput_Device->AddRef();
}

HRESULT CUI_Frame::Set_CoolDown(const _float & fCoolDown)
{
	m_fCoolDown = fCoolDown;
	auto& iter = find_if(m_mapUI.begin(), m_mapUI.end(), CCompare_String(L"IconMask"));
	if (iter == m_mapUI.end())
		return E_FAIL;

	CUI_IconMask* pIconMask = dynamic_cast<CUI_IconMask*>(iter->second);
	if (pIconMask == nullptr)
		return E_FAIL;

	pIconMask->Set_CoolDown(fCoolDown);
	return S_OK;

}

HRESULT CUI_Frame::Set_Radio(const _float & fRadio, const _tchar* pUI_Tag)
{
	auto& iter = find_if(m_mapUI.begin(), m_mapUI.end(), CCompare_String(pUI_Tag/*L"BossMask"*/));
	if (iter == m_mapUI.end())
		return E_FAIL;


	if (!lstrcmp(pUI_Tag, L"Mask"))
	{
		CUI_Mask* pMask = dynamic_cast<CUI_Mask*>(iter->second);
		if (pMask == nullptr)
			return E_FAIL;

		pMask->Set_Radio(fRadio);
		return S_OK;
	}

	if (!lstrcmp(pUI_Tag, L"BossMask"))
	{
		CUI_BossMask* pBossMask = dynamic_cast<CUI_BossMask*>(iter->second);
		if (pBossMask == nullptr)
			return E_FAIL;

		pBossMask->Set_Radio(fRadio);
		return S_OK;
	}


	return S_OK;
}

HRESULT CUI_Frame::Set_IsCoolDown(_bool bIsCoolDown)
{
	m_bIsCoolDown = bIsCoolDown;
	auto& iter = find_if(m_mapUI.begin(), m_mapUI.end(), CCompare_String(L"IconMask"));
	if (iter == m_mapUI.end())
		return E_FAIL;

	CUI_IconMask* pIconMask = dynamic_cast<CUI_IconMask*>(iter->second);
	if (pIconMask == nullptr)
		return E_FAIL;

	pIconMask->Set_IsCoolDown(bIsCoolDown);
	return S_OK;
}

_bool CUI_Frame::Get_IsCoolDown()
{
	return m_bIsCoolDown;
}

CUI * CUI_Frame::Get_UI(const _tchar * pTag)
{
	return Find_UI(pTag);
}

_bool CUI_Frame::Compare_Tag(const wstring & wstrTag)
{
	if (m_wstrTag == wstrTag)
		return true;
	return false;
}

HRESULT Client::CUI_Frame::Ready_Prototype()
{
	return S_OK;
}

HRESULT Client::CUI_Frame::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_vPosition = _vec3(400.f, 300.f, 0.f);

	return S_OK;
}

Engine::_int Client::CUI_Frame::Update_GameObject(const _float& fTimeDelta)
{

	//Traversal Update;
	D3DXMatrixOrthoLH(&m_matProj, _float(g_nBackCX), _float(g_nBackCY), 0.f, 1.f);

	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, m_vPosition.x - g_nBackCX * 0.5f, -m_vPosition.y + g_nBackCY * 0.5f, 0.f);

	for (auto& Pair : m_mapUI)
	{
		_int iEvent;
		iEvent = Pair.second->Update_UI(fTimeDelta,m_pTransformCom->Get_WorldMatrix());
		if (iEvent == COOLDOWN_END)
			m_bIsCoolDown = false;
	}

	return 0;
}

Engine::_int Client::CUI_Frame::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if (!m_bActivate)
		return 0;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	return 0;
}

void Client::CUI_Frame::Render_GameObject()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;

	pEffect->AddRef();
	{
		pEffect->Begin(nullptr, 0);

		for (auto& Pair : m_mapUI)
		{
			Pair.second->Render_UI(pEffect,&m_matProj);
		}
		pEffect->End();
	}
	Safe_Release(pEffect);

}


void CUI_Frame::Load_UIFrame_Info(HANDLE & hFile, _ulong & dwByte)
{
	_int	iFrameLength;
	_tchar  szUIFrameTag[MIN_PATH];

	//load tag
	ReadFile(hFile, &iFrameLength, sizeof(_int), &dwByte, nullptr);
	ReadFile(hFile, szUIFrameTag, sizeof(_tchar)*iFrameLength, &dwByte, nullptr);
	this->m_wstrTag = szUIFrameTag;

	//Load Frame Position;
	ReadFile(hFile, &m_vPosition, sizeof(_vec3), &dwByte, nullptr);

	//UI Size Load
	_int iMapSize = m_mapUI.size();
	ReadFile(hFile, &iMapSize, sizeof(_int), &dwByte, nullptr);

	for (_int i = 0; i < iMapSize; ++i)
	{
		_matrix matWorld;
		_int iLength;
		_int iTextureIndex;
		_tchar szUITag[MIN_PATH];
		_tchar szComponentTextureTag[MIN_PATH];

		//UITag  Load
		ReadFile(hFile, &iLength, sizeof(_int), &dwByte, nullptr);
		ReadFile(hFile, szUITag, sizeof(_tchar)*iLength, &dwByte, nullptr);

		//UI'matWorld Load
		ReadFile(hFile, &matWorld, sizeof(_matrix), &dwByte, nullptr);

		//UI Texture Info Load;
		ReadFile(hFile, &iLength, sizeof(_int), &dwByte, nullptr);
		ReadFile(hFile, szComponentTextureTag, sizeof(_tchar)*iLength, &dwByte, nullptr);

		ReadFile(hFile, &iTextureIndex, sizeof(_int), &dwByte, nullptr);

		if(!lstrcmp(szUITag, L"Back"))
		{
			CUI_Back*	pUI = CUI_Back::Create(m_pDevice);
			pUI->SetUp_LoadInfo(&matWorld, szComponentTextureTag);
			pUI->Set_TextureIndex(iTextureIndex);

			//wstring	wstrComponentTag = L"Component_Shader_UI_";
			//wstrComponentTag += szUITag;
			//pUI->Set_Shader(wstrComponentTag.c_str());

			m_mapUI.emplace(szUITag, pUI);
		}
		
		if(!lstrcmp(szUITag, L"Bar"))
		{
			CUI_Bar*	pUI = CUI_Bar::Create(m_pDevice);
			pUI->SetUp_LoadInfo(&matWorld, szComponentTextureTag);
			pUI->Set_TextureIndex(iTextureIndex);

			m_mapUI.emplace(szUITag, pUI);
		}

		if (!lstrcmp(szUITag, L"Mask"))
		{
			CUI_Mask*	pUI = CUI_Mask::Create(m_pDevice);
			pUI->SetUp_LoadInfo(&matWorld, szComponentTextureTag);
			pUI->Set_TextureIndex(iTextureIndex);

			m_mapUI.emplace(szUITag, pUI);
		}

		if (!lstrcmp(szUITag, L"Icon"))
		{
			CUI_Icon*	pUI = CUI_Icon::Create(m_pDevice);
			pUI->SetUp_LoadInfo(&matWorld, szComponentTextureTag);
			pUI->Set_TextureIndex(iTextureIndex);


			m_mapUI.emplace(szUITag, pUI);
		}

		if (!lstrcmp(szUITag, L"IconMask"))
		{
			CUI_IconMask*	pUI = CUI_IconMask::Create(m_pDevice);
			pUI->SetUp_LoadInfo(&matWorld, szComponentTextureTag);
			pUI->Set_TextureIndex(iTextureIndex);

			m_mapUI.emplace(szUITag, pUI);
		}

		if (!lstrcmp(szUITag, L"BossMask"))
		{
			CUI_BossMask*	pUI = CUI_BossMask::Create(m_pDevice);
			pUI->SetUp_LoadInfo(&matWorld, szComponentTextureTag);
			pUI->Set_TextureIndex(iTextureIndex);

			m_mapUI.emplace(szUITag, pUI);
		}

		if (!lstrcmp(szUITag, L"BossBar"))
		{
			CUI_BossBar*	pUI = CUI_BossBar::Create(m_pDevice);
			pUI->SetUp_LoadInfo(&matWorld, szComponentTextureTag);
			pUI->Set_TextureIndex(iTextureIndex);

			m_mapUI.emplace(szUITag, pUI);
		}


	}
}



HRESULT CUI_Frame::Ready_Component()
{
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();

	//For. Renderer
	m_pRendererCom = dynamic_cast<CRenderer*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Renderer"));
	if (FAILED(Add_Component(L"Com_Renderer", m_pRendererCom)))
		return E_FAIL;

	// For. Transform
	m_pTransformCom = dynamic_cast<CTransform*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Transform"));
	if (FAILED(Add_Component(L"Com_Transform", m_pTransformCom)))
		return E_FAIL;

	// For. Shader
	m_pShaderCom = dynamic_cast<CShader*>(pManagement->Clone_Component(SCENE_STAGE, L"Component_Shader_UI"));
	if (FAILED(Add_Component(L"Com_Shader", m_pShaderCom)))
		return E_FAIL;



	Safe_Release(pManagement);
	return S_OK;
}

CUI * CUI_Frame::Find_UI(const _tchar * pTag)
{
	auto& iter = find_if(m_mapUI.begin(), m_mapUI.end(), CCompare_String(pTag));
	
	if (iter == m_mapUI.end())
		return nullptr;

	return  iter->second;
}

Engine::CGameObject* Client::CUI_Frame::Clone_GameObject()
{
	CUI_Frame*	pInstance = new CUI_Frame(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX(L"CUI_Frame Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUI_Frame::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CUI_Frame*	pInstance = new CUI_Frame(pDevice);

	if (FAILED(pInstance->Ready_Prototype()))
	{
		_MSG_BOX(L"CUI_Frame Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Client::CUI_Frame::Free()
{
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pShaderCom);

	for (auto& Pair : m_mapUI)
		Safe_Release(Pair.second);
	m_mapUI.clear();

	CGameObject::Free();
}

