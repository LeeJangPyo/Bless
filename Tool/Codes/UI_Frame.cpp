#include "stdafx.h"
#include "..\Headers\UI_Frame.h"

#include "Management.h"

_USING(Client)

CUI_Frame::CUI_Frame(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
	,m_pRendererCom(nullptr)
	, m_pShaderCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_bIsSelect(false)
	, m_pInput_Device(CInput_Device::GetInstance())
	, m_bIsLButtonDown(false)
{
	m_pInput_Device->AddRef();
}

CUI_Frame::CUI_Frame(const CUI_Frame & rhs)
	:CGameObject(rhs)
	, m_pInput_Device(rhs.m_pInput_Device)
	, m_bIsSelect(false)
	, m_bIsLButtonDown(false)
{
	m_pInput_Device->AddRef();
}

void CUI_Frame::Set_Position(const _float & fX, const _float & fY)
{
	//m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, fX, fY,0.f);
	m_vPosition.x = fX;
	m_vPosition.y = fY;
}

void CUI_Frame::Set_Scale(const _float & fX, const _float & fY)
{
	m_pTransformCom->Set_ScaleInfo(fX, fY, 1.f);
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
	if(m_bIsSelect)
		Check_KeyInput(fTimeDelta);

	//Traversal Update;
	D3DXMatrixOrthoLH(&m_matProj, _float(g_nBackCX), _float(g_nBackCY), 0.f, 1.f);

	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, m_vPosition.x - g_nBackCX * 0.5f, -m_vPosition.y + g_nBackCY * 0.5f, 0.f);

	for (auto& Pair : m_mapUI)
	{
		Pair.second->Update_UI(m_pTransformCom->Get_WorldMatrix());
	}

	return 0;
}

Engine::_int Client::CUI_Frame::LastUpdate_GameObject(const _float& fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	return 0;
}

void Client::CUI_Frame::Render_GameObject()
{
	//Travalsal Render;
	if (m_pShaderCom == nullptr)
		return;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == nullptr)
		return;
	pEffect->AddRef();

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);
	
	for(auto& Pair : m_mapUI)
	{
		Pair.second->Render_UI(pEffect, &m_matProj);
	}

	pEffect->EndPass();
	pEffect->End();


	Safe_Release(pEffect);

}

HRESULT CUI_Frame::Add_UI(const _tchar * pTag)
{
	CUI*	pUI = CUI::Create(m_pDevice);
	if (pUI == nullptr)
		return E_FAIL;
	pUI->Set_UITab(m_pUITab);

	m_mapUI.emplace(pTag, pUI);

	return S_OK;
}

HRESULT CUI_Frame::Change_UITag(const _tchar* pCurrentTag, const _tchar* pDestTag)
{
	//CUI* pUI = Find_UI(pCurrentTag);
	//if (pUI == nullptr)
	//	return E_FAIL;
	//pUI->

	return S_OK;
}

HRESULT CUI_Frame::Save_UIFrame_Info(HANDLE & hFile,_ulong& dwByte)
{
	//Svae Frametag;
	_int	iLength = m_wstrTag.length() + 1;
	WriteFile(hFile, &iLength, sizeof(_int), &dwByte, nullptr);
	WriteFile(hFile, m_wstrTag.c_str(), sizeof(_tchar)*iLength, &dwByte, nullptr);

	//Save Frame Position;
	WriteFile(hFile, &m_vPosition, sizeof(_vec3), &dwByte, nullptr);

	//UI ÀúÀå
	_int iMapSize = m_mapUI.size();
	WriteFile(hFile, &iMapSize, sizeof(_int), &dwByte, nullptr);
	for(auto& Pair : m_mapUI)
	{
		_int	iLength = Pair.first.length()+1;
		WriteFile(hFile, &iLength, sizeof(_int), &dwByte, nullptr);
		WriteFile(hFile, Pair.first.c_str(), sizeof(_tchar)*iLength, &dwByte, nullptr);


		Pair.second->Save_UI_Info(hFile, dwByte);
	}

	return S_OK;
}

void CUI_Frame::Load_UIFrame_Info(HANDLE & hFile, _ulong & dwByte, CTreeCtrl& TreeUIList)
{
	_int	iFrameLength;
	_tchar  szUIFrameTag[MIN_PATH];
	//load tag
	ReadFile(hFile, &iFrameLength, sizeof(_int), &dwByte, nullptr);
	ReadFile(hFile, szUIFrameTag, sizeof(_tchar)*iFrameLength, &dwByte, nullptr);
	m_wstrTag = szUIFrameTag;
	HTREEITEM hFrame = TreeUIList.InsertItem(szUIFrameTag);

	//Load Frame Position;
	ReadFile(hFile, &m_vPosition, sizeof(_vec3), &dwByte, nullptr);

	//UI Size Load
	_int iMapSize = m_mapUI.size();
	ReadFile(hFile, &iMapSize, sizeof(_int), &dwByte, nullptr);

	for(_int i =0 ; i < iMapSize; ++i)
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

		CUI*	pUI = CUI::Create(m_pDevice);
		pUI->SetUp_LoadInfo(&matWorld, szComponentTextureTag);
		pUI->Set_TextureIndex(iTextureIndex);
		pUI->Set_UITab(m_pUITab);
		m_mapUI.emplace(szUITag, pUI);
		TreeUIList.InsertItem(szUITag, hFrame);
		
	}
}


void CUI_Frame::Check_KeyInput(const _float & fTimeDelta)
{
	POINT	ptMouse = {};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);
	if (ptMouse.x < 0 || ptMouse.x > 800 ||
		ptMouse.y < 0 || ptMouse.y > 600)
		return;

	if(m_pInput_Device->Get_MouseDown(CInput_Device::DIM_LB))
	{
		m_bIsLButtonDown = true;
	}
	if(m_pInput_Device->Get_MousePressing(CInput_Device::DIM_LB))
	{
		if (m_bIsLButtonDown)
		{
			_float fMouseMove = 0.f;
			if(fMouseMove = m_pInput_Device->Get_MouseMove(CInput_Device::DIMS_X))
			{
				m_vPosition.x += fMouseMove;
			}

			fMouseMove = 0.f;
			if (fMouseMove = m_pInput_Device->Get_MouseMove(CInput_Device::DIMS_Y))
			{
				m_vPosition.y += fMouseMove;
			}
		}
		
	}
	if(m_pInput_Device->Get_MouseUp(CInput_Device::DIM_LB))
	{
		m_bIsLButtonDown = false;
	}

	const _vec3* pPosition = m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
	m_pUITab->Update_EditBox(0.f,0.f, pPosition->x, pPosition->y,0.f);
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

	//For. Shader
	m_pShaderCom = dynamic_cast<CShader*>(pManagement->Clone_Component(SCENE_EDIT, L"Component_Shader_UI"));
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
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pInput_Device);

	for (auto& Pair : m_mapUI)
		Safe_Release(Pair.second);
	m_mapUI.clear();

	CGameObject::Free();
}

