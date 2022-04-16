#include "..\Headers\Layer.h"


CLayer::CLayer()
{
}

CGameObject * CLayer::Get_GameObject(const _uint & iIndex)
{
	if (m_ObjectList.size() <= iIndex)
		return nullptr;

	auto	iter = m_ObjectList.begin();

	for (size_t i = 0; i < iIndex; ++i)
		++iter;

	return *iter;
}

CComponent * CLayer::Get_Component(const _tchar * pComponentTag, const _uint & iIndex)
{
	if (m_ObjectList.size() <= iIndex)
		return nullptr;

	auto	iter = m_ObjectList.begin();

	for (size_t i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->Get_Component(pComponentTag);
}



HRESULT CLayer::Add_Object(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_ObjectList.push_back(pGameObject);

	return S_OK;
}

HRESULT CLayer::Remove_AllObject()
{
	for (auto& pObject : m_ObjectList)
		Safe_Release(pObject);
	m_ObjectList.clear();

	return S_OK;
}

HRESULT CLayer::Remove_GameObject(CGameObject * pGameObject)
{
	auto& iter = find_if(m_ObjectList.begin(), m_ObjectList.end(), [&](CGameObject* pObject) {
		if (pObject == pGameObject)
			return true;
		return false;
	});

	if (iter == m_ObjectList.end())
		return E_FAIL;

	Safe_Release(*iter);
	m_ObjectList.erase(iter);
	
	return S_OK;
}

HRESULT CLayer::Ready_Layer()
{
	return S_OK;
}

_int CLayer::Update_Layer(const _float & fTimeDelta)
{
	//for (auto& pGameObject : m_ObjectList)
	//{
	//	/*if (nullptr == pGameObject)
	//		continue;

	//	if (pGameObject->Update_GameObject(fTimeDelta) & 0x80000000)
	//		return -1;*/

	//	
	//}
	auto iter_begin = m_ObjectList.begin();
	auto iter_end = m_ObjectList.end();

	_int iEvent = NO_EVENT;

	for (; iter_begin != iter_end; )
	{
		iEvent = (*iter_begin)->Update_GameObject(fTimeDelta);

		if (iEvent == DEAD_OBJ)
		{
			Safe_Release(*iter_begin);
			iter_begin = m_ObjectList.erase(iter_begin);
		}
		else
			++iter_begin;
	}
	return iEvent;
}

_int CLayer::LastUpdate_Layer(const _float & fTimeDelta)
{
	for (auto& pGameObject : m_ObjectList)
	{
		if (nullptr == pGameObject)
			continue;

		if (pGameObject->LastUpdate_GameObject(fTimeDelta) & 0x80000000)
			return -1;
	}

	return _int();
}

CLayer * CLayer::Create()
{
	CLayer*		pInstance = new CLayer();

	if (FAILED(pInstance->Ready_Layer()))
	{
		_MSG_BOX(L"CLayer Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLayer::Free()
{
	for (auto& pGameObject : m_ObjectList) 	
		Safe_Release(pGameObject);

	m_ObjectList.clear();	
}
