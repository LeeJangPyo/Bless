#include "..\Headers\Camera_Manager.h"


_IMPLEMENT_SINGLETON(CCamera_Manager)

Engine::CCamera_Manager::CCamera_Manager()
	:m_iNumCameraType(0)
	, m_pmapCamera(nullptr)
{

}


CCamera * CCamera_Manager::Get_Camera(const _uint & iCameraType, const _tchar * pCameraTag)
{
	CCamera*	pCamera = Find_Camera(iCameraType, pCameraTag);

	if (pCamera == nullptr)
		return nullptr;

	return pCamera;	
}

HRESULT CCamera_Manager::Reserve_Camera_Manager(const _uint & iNumCameraType)
{
	m_iNumCameraType = iNumCameraType;

	m_pmapCamera = new MAPCAMERAS[iNumCameraType];

	return S_OK;

}

HRESULT CCamera_Manager::Add_Prototype_Camera(const _tchar * pProtoTag, CCamera * pCamera)
{
	if (pCamera == nullptr)
		return E_FAIL;

	CGameObject* pPrototype = Find_Prototype(pProtoTag);
	if (nullptr != pPrototype)
		return E_FAIL;

	m_mapPrototype.emplace(pProtoTag, pCamera);
	return S_OK;
}

HRESULT CCamera_Manager::Add_Camera(const _uint & iCameraType, const _tchar * pProtoTag, const _tchar* pCameraTag, CCamera ** ppCamera)
{
	if (nullptr == m_pmapCamera)
		return E_FAIL;

	CCamera* pPrototype = Find_Prototype(pProtoTag);
	if (nullptr == pPrototype)
		return E_FAIL;

	CCamera*	pCamera = dynamic_cast<CCamera*>(pPrototype->Clone_GameObject());
	if (nullptr == pCamera)
		return E_FAIL;

	m_pmapCamera[iCameraType].emplace(pCameraTag, pCamera);

	if (ppCamera != nullptr)
		*ppCamera = pCamera;

	return S_OK;
}

void CCamera_Manager::Render_Debug()
{
	for(_uint i = 0; i < m_iNumCameraType; ++i)
	{
		for (auto& Pair : m_pmapCamera[i])
			Pair.second->Render_GameObject();
	}
}

CCamera * CCamera_Manager::Find_Prototype(const _tchar * pProtoTag)
{
	auto iter = find_if(m_mapPrototype.begin(), m_mapPrototype.end(), CCompare_String(pProtoTag));

	if (iter == m_mapPrototype.end())
		return nullptr;

	return iter->second;
}

CCamera * CCamera_Manager::Find_Camera(const _uint & iCameraType, const _tchar * pCameraTag)
{
	auto iter = find_if(m_pmapCamera[iCameraType].begin(), m_pmapCamera[iCameraType].end(), CCompare_String(pCameraTag));

	if (iter == m_pmapCamera[iCameraType].end())
		return nullptr;

	return iter->second;
}

void Engine::CCamera_Manager::Free()
{
	for (size_t i = 0; i < m_iNumCameraType; i++)
	{
		for (auto& Pair : m_pmapCamera[i])
		{
			Safe_Release(Pair.second);
		}
		m_pmapCamera[i].clear();
	}
	Safe_Delete_Array(m_pmapCamera);

	for (auto& Pair : m_mapPrototype)
	{
		Safe_Release(Pair.second);
	}
	m_mapPrototype.clear();
}
