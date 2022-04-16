#include "..\Headers\GameObject.h"


CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pDevice(pGraphic_Device)
	, m_bIsDead(false)
	, m_bDraw(true)
{
	m_pDevice->AddRef();
	
}

CGameObject::CGameObject(const CGameObject & rhs)
	: m_pDevice(rhs.m_pDevice)
	, m_bIsDead(false)
	, m_bDraw(true)
{
	m_pDevice->AddRef();
}

CComponent * CGameObject::Get_Component(const _tchar * pComponentTag)
{
	CComponent* pComponent = Find_Component(pComponentTag);
	if (pComponent == nullptr)
	{
		_tchar szComponentTag[MIN_PATH];
		wsprintf(szComponentTag, L"%s Don't Have Component in MapContainer, in Get_Component", pComponentTag);
		_TAGMSG_BOX(szComponentTag, L"CGameObject");
		return nullptr;
	}

	return pComponent;
}

HRESULT CGameObject::Replace_Component(const _tchar * pComponentTag, const _tchar * pComponentProtoTag)
{

	return S_OK;
}

void CGameObject::Compute_ViewZ(const _vec3 * pWorldPos)
{
	_vec3		vCamPosition;

	_matrix		matView;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);

	memcpy(&vCamPosition, &matView.m[3][0], sizeof(_vec3));

	m_fViewZ = D3DXVec3Length(&(vCamPosition - *pWorldPos));
}

HRESULT CGameObject::Ready_Prototype()
{
	return S_OK;
}

HRESULT CGameObject::Ready_GameObject()
{
	return S_OK;
}

_int CGameObject::Update_GameObject(const _float & fTimeDelta)
{
	return _int();
}

_int CGameObject::LastUpdate_GameObject(const _float & fTimeDelta)
{
	return _int();
}

void CGameObject::Render_GameObject()
{
}

HRESULT CGameObject::Add_Component(const _tchar * pComponentTag, CComponent * pComponent)
{
	if (pComponent == nullptr)
	{
		_tchar szComponentTag[MIN_PATH];
		wsprintf(szComponentTag, L"%s is nullptr in Add_Component", pComponentTag);
		_TAGMSG_BOX(szComponentTag, L"CGameObject");
		return E_FAIL;
	}

	//있으면 넣지 말아야한다.
	if (Find_Component(pComponentTag) != nullptr)
		return E_FAIL;

	m_mapComponent.emplace(pComponentTag, pComponent);

	pComponent->AddRef();

	return S_OK;
}

HRESULT CGameObject::Remove_Component(const _tchar * pComponentTag)
{
	CComponent* pComponent = Find_Component(pComponentTag);

	if (pComponent == nullptr)
		return E_FAIL;

	Safe_Release(pComponent);
	
	auto	iter = find_if(m_mapComponent.begin(), m_mapComponent.end(), CFinder_Tag(pComponentTag));
	if (iter == m_mapComponent.end())
		return E_FAIL;

	m_mapComponent.erase(iter);

	return S_OK;
}


CComponent * CGameObject::Find_Component(const _tchar * pComponentTag)
{
	auto	iter = find_if(m_mapComponent.begin(), m_mapComponent.end(), CFinder_Tag(pComponentTag));

	//못찾음
	if (iter == m_mapComponent.end())
		return nullptr;

	return iter->second;
}

void CGameObject::Free()
{
	for (auto& Pair : m_mapComponent)
		Safe_Release(Pair.second);

	m_mapComponent.clear();


	Safe_Release(m_pDevice);
}
