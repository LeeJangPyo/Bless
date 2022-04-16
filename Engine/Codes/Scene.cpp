#include "..\Headers\Scene.h"
#include "Object_Manager.h"
#include "Management.h"


CScene::CScene(LPDIRECT3DDEVICE9 pDevice)
	: m_pDevice(pDevice)
	, m_pObject_Manager(CObject_Manager::GetInstance())
	, m_pInput_Device(CInput_Device::GetInstance())
{
	m_pDevice->AddRef();
	m_pInput_Device->AddRef();
	m_pObject_Manager->AddRef();
}

HRESULT CScene::Ready_Scene()
{
	return NOERROR;
}

_int CScene::Update_Scene(const _float & fTimeDelta)
{
	if (nullptr == m_pObject_Manager)
		return -1;


	return m_pObject_Manager->Update_Object_Manager(fTimeDelta);
}

_int CScene::LastUpdate_Scene(const _float & fTimeDelta)
{
	if (nullptr == m_pObject_Manager)
		return -1;

	return m_pObject_Manager->LastUpdate_Object_Manager(fTimeDelta);
}

_int CScene::Update_Scene(const _uint & iSceneID, const _float & fTimeDelta)
{
	if (nullptr == m_pObject_Manager)
		return -1;

	return m_pObject_Manager->Update_Object_Manager(iSceneID,fTimeDelta);
}

_int CScene::LastUpdate_Scene(const _uint & iSceneID, const _float & fTimeDelta)
{
	if (nullptr == m_pObject_Manager)
		return -1;

	return m_pObject_Manager->LastUpdate_Object_Manager(iSceneID, fTimeDelta);
}

void CScene::Render_Scene()
{

}

void CScene::Free()
{

	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pDevice);
}
