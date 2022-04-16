#include "stdafx.h"
#include "..\Headers\UI.h"

#include "Management.h"

_USING(Client)

CUI::CUI(LPDIRECT3DDEVICE9 pDevice)
	:m_pDevice(pDevice)
	, m_bIsSelect(false)
	, m_bIsLButtonDown(false)
	, m_pInput_Device(CInput_Device::GetInstance())
	, m_iTextureIndex(0)
{
	m_pDevice->AddRef();
	m_pInput_Device->AddRef();
	m_wstrTextureComponentTag = L"";
}



void CUI::SetUp_LoadInfo(const _matrix * pmatWorld, const _tchar * pTextureComponentTag)
{
	m_pTransformCom->Set_WorldMatrix(pmatWorld);
	m_wstrTextureComponentTag = pTextureComponentTag;

	Replace_Texture_Component(pTextureComponentTag);
}

void CUI::Set_Position(const _float & fX, const _float & fY, const _float & fZ)
{
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, fX, fY, fZ);
}

void CUI::Set_Scale(const _float & fX, const _float & fY)
{
	m_pTransformCom->Set_ScaleInfo(fX, fY, 1.f);
}


const _vec3 * CUI::Get_Position() const
{
	return m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
}
 _float CUI::Get_Scale_X() const
{
	return m_pTransformCom->Get_ScaleInfo(CTransform::STATE_RIGHT);
}

 _float CUI::Get_Scale_Y() const
{
	return m_pTransformCom->Get_ScaleInfo(CTransform::STATE_UP);
}

HRESULT CUI::Ready_UI()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Set_ScaleInfo(400.f, 300.f, 1.f);

	return S_OK;
}

_int CUI::Update_UI(const _matrix * pmatParent)
{
	if(m_bIsSelect)
		Check_KeyInput();

	m_matCombinedWorld = (*m_pTransformCom->Get_WorldMatrix()) * (*pmatParent);
	

	return 0;
}

void CUI::Render_UI(LPD3DXEFFECT pEffect, const _matrix * pmatProj)
{
	if (pEffect == nullptr)
		return;

	pEffect->AddRef();

	SetUp_ConstantTable(pEffect, pmatProj);
	pEffect->CommitChanges();

	m_pBufferCom->Render_Buffer();

	Safe_Release(pEffect);
}

HRESULT CUI::Replace_Texture_Component(const _tchar* pComponentProtoTag)
{
	m_wstrTextureComponentTag = pComponentProtoTag;

	//Release Current Texture;
	Safe_Release(m_pTextureCom);
	
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();

	//For. Texutre
	m_pTextureCom = dynamic_cast<CTexture*>(pManagement->Clone_Component(SCENE_EDIT, pComponentProtoTag));
	if (m_pTextureCom == nullptr)
		return E_FAIL;

	Safe_Release(pManagement);

	return S_OK;
}

HRESULT CUI::Save_UI_Info(HANDLE & hFile, _ulong& dwByte)
{
	//UI'matWorld Save
	WriteFile(hFile, m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix), &dwByte, nullptr);

	//UI Texture Info Save;
	_int iLength = m_wstrTextureComponentTag.length()+1;
	WriteFile(hFile, &iLength, sizeof(_int), &dwByte, nullptr);
	WriteFile(hFile, m_wstrTextureComponentTag.c_str(), sizeof(_tchar)*iLength, &dwByte, nullptr);


	//Texture Index;
	WriteFile(hFile, &m_iTextureIndex, sizeof(_int), &dwByte, nullptr);



	return S_OK;
}



void CUI::Check_KeyInput()
{
	POINT	ptMouse = {};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);
	if (ptMouse.x < 0 || ptMouse.x > 800 ||
		ptMouse.y < 0 || ptMouse.y > 600)
		return;

	if (m_pInput_Device->Get_MouseDown(CInput_Device::DIM_LB))
	{
		m_bIsLButtonDown = true;
	}
	if (m_pInput_Device->Get_MousePressing(CInput_Device::DIM_LB))
	{
		if (m_bIsLButtonDown)
		{
			_float fMouseMove = 0.f;
			if (fMouseMove = m_pInput_Device->Get_MouseMove(CInput_Device::DIMS_X))
			{
				m_pTransformCom->Plus_Position(fMouseMove, 0.f, 0.f);
			}

			fMouseMove = 0.f;
			if (fMouseMove = m_pInput_Device->Get_MouseMove(CInput_Device::DIMS_Y))
			{
				m_pTransformCom->Plus_Position(0.f, -fMouseMove, 0.f);
			}
		}

	}
	if (m_pInput_Device->Get_MouseUp(CInput_Device::DIM_LB))
	{
		m_bIsLButtonDown = false;
	}


	if(m_pInput_Device->Get_KeyDown(DIK_1))
	{
		m_iTextureIndex = 0;
	}
	if (m_pInput_Device->Get_KeyDown(DIK_2))
	{
		m_iTextureIndex = 1;
	}

	const _vec3* pPosition = m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
	m_pUITab->Update_EditBox(Get_Scale_X(), Get_Scale_Y(), pPosition->x, pPosition->y, pPosition->z);
}

HRESULT CUI::Ready_Component()
{
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();

	//For. Transfrom
	m_pTransformCom = dynamic_cast<CTransform*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Transform"));
//	if (FAILED(Add_Component(L"Com_Transform", m_pTransformCom)))
	if (m_pTransformCom == nullptr)
		return E_FAIL;

	//For. Buffer
	m_pBufferCom = dynamic_cast<CBuffer_RcTex*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Buffer_RcTex"));
//	if (FAILED(Add_Component(L"Com_Buffer", m_pBufferCom)))
	if (m_pBufferCom == nullptr)
		return E_FAIL;

	//For. Texutre
	m_pTextureCom = dynamic_cast<CTexture*>(pManagement->Clone_Component(SCENE_EDIT, L"Component_Texture_Default"));
	//if (FAILED(Add_Component(L"Com_Renderer", m_pRendererCom)))
	if(m_pTextureCom == nullptr)
		return E_FAIL;

	Safe_Release(pManagement);
	return S_OK;
}

HRESULT CUI::SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _matrix * pmatProj)
{
	if (pEffect == nullptr)
		return E_FAIL;
	pEffect->AddRef();

	_matrix matView, matProj;
	D3DXMatrixIdentity(&matView);
//	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
//	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &m_matCombinedWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", pmatProj);

	m_pTextureCom->SetUp_OnShader(pEffect,"g_BaseTexture",m_iTextureIndex);


	Safe_Release(pEffect);
	return S_OK;
}

CUI * CUI::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CUI*	pInstance = new CUI(pDevice);

	if (FAILED(pInstance->Ready_UI()))
	{
		_MSG_BOX(L"CUI Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Client::CUI::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pInput_Device);

	Safe_Release(m_pDevice);
}
