#include "stdafx.h"
#include "..\Headers\Axis.h"

#include "Management.h"

CAxis::CAxis(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, m_pTransformCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pLine_Manager(CLine_Manager::GetInstance())
{
	m_pLine_Manager->AddRef();

	for (_int i = 0; i < AXIS_END; ++i)
		m_pPoint[i] = nullptr;
}

CAxis::CAxis(const CAxis & rhs)
	: CGameObject(rhs)
	,m_pLine_Manager(rhs.m_pLine_Manager)
{
	m_pLine_Manager->AddRef();
}


HRESULT CAxis::Ready_Prototype()
{
	m_pLine_Manager->Ready_Line(m_pDevice, L"Axis",3.f);

	return S_OK;
}

HRESULT CAxis::Ready_GameObject()
{
	
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_dwArrayCount[AXIS_X] = 2;
	m_dwArrayCount[AXIS_Y] = 2;
	m_dwArrayCount[AXIS_Z] = 2;

	m_pPoint[AXIS_X] = new _vec3[m_dwArrayCount[AXIS_X]];
	m_pPoint[AXIS_X][0] = _vec3(0.f, 0.f, 0.f);
	m_pPoint[AXIS_X][1] = _vec3(10.f, 0.f, 0.f);

	m_pPoint[AXIS_Y] = new _vec3[m_dwArrayCount[AXIS_Y]];
	m_pPoint[AXIS_Y][0] = _vec3(0.f, 0.f, 0.f);
	m_pPoint[AXIS_Y][1] = _vec3(0.f, 10.f, 0.f);

	m_pPoint[AXIS_Z] = new _vec3[m_dwArrayCount[AXIS_Z]];
	m_pPoint[AXIS_Z][0] = _vec3(0.f, 0.f, 0.f);
	m_pPoint[AXIS_Z][1] = _vec3(0.f, 0.f, 10.f);


	return S_OK;
}

Engine::_int CAxis::Update_GameObject(const _float& fTimeDelta)
{

	return 0;
}

Engine::_int CAxis::LastUpdate_GameObject(const _float& fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	return 0;
}

void CAxis::Render_GameObject()
{
	_matrix matView, matProj, matWorld;
	D3DXMatrixIdentity(&matWorld);

	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	matWorld = matView * matProj;

	/*m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pDevice->SetTexture(0, 0);*/

	m_pLine_Manager->Render_Line(L"Axis", m_pPoint[AXIS_X], m_dwArrayCount[AXIS_X], D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	m_pLine_Manager->Render_Line(L"Axis", m_pPoint[AXIS_Y], m_dwArrayCount[AXIS_Y], D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
	m_pLine_Manager->Render_Line(L"Axis", m_pPoint[AXIS_Z], m_dwArrayCount[AXIS_Z], D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
}


HRESULT CAxis::Ready_Component(void)
{
	CManagement*	pManagement = CManagement::GetInstance();
	if (pManagement == nullptr)
		return E_FAIL;

	pManagement->AddRef();

	m_pRendererCom = dynamic_cast<CRenderer*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Renderer"));
	if (FAILED(Add_Component(L"Com_Renderer", m_pRendererCom)))
		return E_FAIL;

	m_pTransformCom = dynamic_cast<CTransform*>(pManagement->Clone_Component(SCENE_STATIC, L"Component_Transform"));
	if (FAILED(Add_Component(L"Com_Transform", m_pTransformCom)))
		return E_FAIL;

	Safe_Release(pManagement);


	return S_OK;
}



CAxis* CAxis::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CAxis*	pInstance = new CAxis(pDevice);

	if (FAILED(pInstance->Ready_Prototype()))
	{
		_MSG_BOX(L"CAxis Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CAxis::Clone_GameObject()
{
	CAxis*	pInstance = new CAxis(*this);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		_MSG_BOX(L"CAxis Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAxis::Free(void)
{

	Safe_Release(m_pLine_Manager);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);

	for (_int i = 0; i < AXIS_END; ++i)
		Safe_Delete_Array(m_pPoint[i]);

	CGameObject::Free();
}

