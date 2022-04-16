#include "..\Headers\Object_Manager.h"


_IMPLEMENT_SINGLETON(CObject_Manager)

CObject_Manager::CObject_Manager()
{
}


CGameObject * CObject_Manager::Get_GameObject(const _uint & iSceneID, const _tchar * pLayerTag, const _uint& iIndex)
{
	if (m_pMapLayer == nullptr)
		return nullptr;


	CLayer* pLayer = Find_Layer(iSceneID, pLayerTag);
	if (pLayer == nullptr)
		return nullptr;

	return pLayer->Get_GameObject(iIndex);
}

CComponent * CObject_Manager::Get_Component(const _uint & iSceneID, const _tchar * pLayerTag, const _tchar * pComponentTag, const _uint & iIndex)
{
	if (m_pMapLayer == nullptr)
		return nullptr;

	if (m_iNumScene <= iSceneID)
		return nullptr;

	CLayer*	pLayer = Find_Layer(iSceneID, pLayerTag);
	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Get_Component(pComponentTag, iIndex);
}

const list<CGameObject*>* CObject_Manager::Get_Layer_ObjectList(const _uint & iSceneID, const _tchar * pLayerTag)
{
	CLayer*	pLayer = Find_Layer(iSceneID, pLayerTag);
	if (pLayer == nullptr)
		return nullptr;

	return  pLayer->Get_Layer_ObjectList();
}

HRESULT CObject_Manager::Reserve_Object_Manager(const _uint & iNumScene)
{
	m_iNumScene = iNumScene;

	m_pMapLayer = new MAPLAYERS[iNumScene];

	return S_OK;
}

HRESULT CObject_Manager::Add_Prototype_GameObject(const _tchar * pProtoTag, CGameObject * pGameObject, CGameObject** ppGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	CGameObject* pPrototype = Find_Prototype(pProtoTag);
	if (nullptr != pPrototype)
		return E_FAIL;

	m_mapPrototype.emplace(pProtoTag, pGameObject);

	if(ppGameObject != nullptr)
		*ppGameObject = pGameObject;

	return S_OK;
}

HRESULT CObject_Manager::Add_Layer(const _uint & iSceneID, const _tchar * pLayerTag, const _tchar * pProtoTag, CGameObject** ppGameObject)
{
	if (nullptr == m_pMapLayer)
		return E_FAIL;

	CGameObject* pPrototype = Find_Prototype(pProtoTag);
	if (nullptr == pPrototype)
		return E_FAIL;

	CGameObject*	pGameObject = pPrototype->Clone_GameObject();
	if (nullptr == pGameObject)
		return E_FAIL;

	CLayer*		pLayer = Find_Layer(iSceneID, pLayerTag);
	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();
		if (nullptr == pLayer)
			return E_FAIL;

		if (FAILED(pLayer->Add_Object(pGameObject)))
			return E_FAIL;

		m_pMapLayer[iSceneID].insert(MAPLAYERS::value_type(pLayerTag, pLayer));
	}
	else
	{
		if (FAILED(pLayer->Add_Object(pGameObject)))
			return E_FAIL;
	}

	if (ppGameObject != nullptr)
		*ppGameObject = pGameObject;
	
	return S_OK;
}

HRESULT CObject_Manager::Remove_Layer(const _uint & iSceneID, const _tchar * pLayerTag)
{
	if (nullptr == m_pMapLayer)
		return E_FAIL;


	CLayer*		pLayer = Find_Layer(iSceneID, pLayerTag);
	if (nullptr == pLayer)
	{
		_TAGMSG_BOX(L"Remove Layer Can't Find", L"CObject_manager");
		return E_FAIL;
	}

	Safe_Release(pLayer);

	return S_OK;
}

HRESULT CObject_Manager::Clear_Scene_Container(const _uint & iSceneID)
{
	if (m_pMapLayer == nullptr)
		return E_FAIL;

	for (auto& Pair : m_pMapLayer[iSceneID])
		Safe_Release(Pair.second);

	m_pMapLayer[iSceneID].clear();

	return S_OK;
}

HRESULT CObject_Manager::Find_Prototype_GameObject(const _tchar * pProtoTag)
{
	CGameObject* pPrototype = Find_Prototype(pProtoTag);
	if (pPrototype  !=  nullptr)	//이미 존재한다
		return E_FAIL;

	return S_OK;
}

HRESULT CObject_Manager::Remove_AllObject(const _uint& iSceneID, const _tchar * pLayerTag)
{
	
	CLayer* pLayer = Find_Layer(iSceneID, pLayerTag);
	// Dont have layer
	if(pLayer == nullptr)
	{
		return E_FAIL;
	}

	if (FAILED(pLayer->Remove_AllObject()))
		return E_FAIL;

	return S_OK;
}

HRESULT CObject_Manager::Remove_Layer_GameObject(const _uint & iSceneID, const _tchar * pLayerTag, CGameObject * pObject)
{
	CLayer* pLayer = Find_Layer(iSceneID, pLayerTag);
	// Dont have layer
	if (pLayer == nullptr)
	{
		return E_FAIL;
	}

	if (FAILED(pLayer->Remove_GameObject(pObject)))
		return E_FAIL;

	return S_OK;
}

HRESULT CObject_Manager::Reserve_ObjectPool(const _tchar * pObjectTag, const _int & iNumObject)
{
	return E_NOTIMPL;
}




_int CObject_Manager::Update_Object_Manager(const _float & fTimeDelta)
{
	if (nullptr == m_pMapLayer)
		return -1;

	for (size_t i = 0; i < m_iNumScene; ++i)
	{
		for (auto& Pair : m_pMapLayer[i])
		{
			if (Pair.second == nullptr)
				continue;

			if (Pair.second->Update_Layer(fTimeDelta) & 0x80000000)
				return -1;
		}
	}
	

	return 0;
}

_int CObject_Manager::LastUpdate_Object_Manager(const _float & fTimeDelta)
{
	if (nullptr == m_pMapLayer)
		return -1;

	for (size_t i = 0; i < m_iNumScene; ++i)
	{
		for (auto& Pair : m_pMapLayer[i])
		{
			if (Pair.second == NULL)
				continue;

			if (Pair.second->LastUpdate_Layer(fTimeDelta) & 0x80000000)
				return -1;
		}
	}

	return 0;
}

_int CObject_Manager::Update_Object_Manager(const _uint & iSceneID, const _float & fTimeDelta)
{
	if (nullptr == m_pMapLayer)
		return -1;

	for (auto& Pair : m_pMapLayer[iSceneID])
	{
		if (Pair.second == nullptr)
			continue;

		if (Pair.second->Update_Layer(fTimeDelta) & 0x80000000)
			return -1;
	}

	return 0;
}

_int CObject_Manager::LastUpdate_Object_Manager(const _uint & iSceneID, const _float & fTimeDelta)
{
	if (nullptr == m_pMapLayer)
		return -1;


	for (auto& Pair : m_pMapLayer[iSceneID])
	{
		if (Pair.second == NULL)
			continue;

		if (Pair.second->LastUpdate_Layer(fTimeDelta) & 0x80000000)
			return -1;
	}

	return 0;
}

CGameObject * CObject_Manager::Find_Prototype(const _tchar * pProtoTag)
{
	auto iter = find_if(m_mapPrototype.begin(), m_mapPrototype.end(), CCompare_String(pProtoTag));
//	wstring wstrProtoTag = pProtoTag;
//	auto iter = m_mapPrototype.find(pProtoTag);

	//없다
	if (iter == m_mapPrototype.end())
		return nullptr;

	return iter->second;	
}

CLayer * CObject_Manager::Find_Layer(const _uint& iSceneID, const _tchar * pLayerTag)
{
	auto iter = find_if(m_pMapLayer[iSceneID].begin(), m_pMapLayer[iSceneID].end(), CCompare_String(pLayerTag));
//	wstring wstrLayerTag = pLayerTag;
//	auto iter = m_pMapLayer[iSceneID].find(pLayerTag);


	if (iter == m_pMapLayer[iSceneID].end())
		return nullptr;

	return iter->second;
}

void CObject_Manager::Free()
{
	for (size_t i = 0; i < m_iNumScene; i++)
	{
		for (auto& Pair : m_pMapLayer[i])
		{
			Safe_Release(Pair.second);
		}
		m_pMapLayer[i].clear();
	}
	Safe_Delete_Array(m_pMapLayer);

	for (auto& Pair : m_mapPrototype)
	{
		Safe_Release(Pair.second);
	}
	m_mapPrototype.clear();
}
