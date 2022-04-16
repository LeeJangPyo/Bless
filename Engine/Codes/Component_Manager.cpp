#include "..\Headers\Component_Manager.h"


_IMPLEMENT_SINGLETON(CComponent_Manager)

CComponent_Manager::CComponent_Manager()
{
}

HRESULT CComponent_Manager::Reserve_Component_Manager(const _uint & iNumScene)
{
	m_iNumScene = iNumScene;

	m_pMapComponent = new MAPCOMPONENT[iNumScene];

	return NOERROR;
}

HRESULT CComponent_Manager::Add_Prototype_Component(const _uint & iSceneID, const _tchar * pComponentTag, CComponent * pPrototype)
{
	if (iSceneID >= m_iNumScene)
	{
		_TAGMSG_BOX(L"SceneID is Bigger then CurrentMaxScene, in Add_Prototype_Component", L"CComponent_Manager");
		return E_FAIL;
	}

	if (m_pMapComponent == nullptr)
	{
		_TAGMSG_BOX(L"m_pMapComponent is Nullptr in Add_Prototype_GameObject", L"CComponent_Manager");
		return E_FAIL;
	}

	if (pPrototype == nullptr)
	{
		_TAGMSG_BOX(L"pPrototype is Nullptr in Add_Prototype_GameObject", L"CComponent_Manager");
		return E_FAIL;
	}

	CComponent* pComponent = Find_Component(iSceneID, pComponentTag);
	if (pComponent != nullptr)
	{
		return S_OK;
	}

	m_pMapComponent[iSceneID].insert(MAPCOMPONENT::value_type(pComponentTag, pPrototype));

	return S_OK;	
}

CComponent * CComponent_Manager::Clone_Component(const _uint & iSceneID, const _tchar * pComponentTag)
{
	if (iSceneID >= m_iNumScene)
	{
		_TAGMSG_BOX(L"SceneID is Bigger then CurrentMaxScene, in Clone_Component", L"CComponent_Manager");
		return nullptr;
	}

	if (m_pMapComponent == nullptr)
	{
		_TAGMSG_BOX(L"m_pMapComponent is Nullptr in Clone_Component", L"CComponent_Manager");
		return nullptr;
	}

	CComponent* pComponent = Find_Component(iSceneID, pComponentTag);
	if (pComponent == nullptr)
	{
		_tchar szMsg[MIN_PATH];
		//+		wstrComponentTag	L"Component_Texture_FX_Crack_006_TEX_HKB"	std::basic_string<wchar_t,std::char_traits<wchar_t>,std::allocator<wchar_t> >
		//pComponentTag = 0x6521fb9c L"Component_Texture_FX_Crack_006_TEX_HKB"
		wsprintf(szMsg, L"%s Not Valied", pComponentTag);
		_TAGMSG_BOX(szMsg, L"CComponent_Manager");
		return nullptr;
	}

	return pComponent->Clone_Component();

}

HRESULT CComponent_Manager::Clear_Scene_Container(const _uint & iSceneID)
{
	if (nullptr == m_pMapComponent)
		return E_FAIL;

	for (auto& Pair : m_pMapComponent[iSceneID])
		Safe_Release(Pair.second);

	m_pMapComponent[iSceneID].clear();

	return S_OK;
}

HRESULT CComponent_Manager::Find_Prototype_Component(const _uint & iSceneID, const _tchar * pProtoTag)
{
	CComponent* pComponent = Find_Component(iSceneID, pProtoTag);
	if (pComponent != nullptr)	//이미 있다
		return E_FAIL;

	return S_OK;
}

CComponent * CComponent_Manager::Find_Component(const _uint & iSceneID, const _tchar * pComponentTag)
{
	auto iter = find_if(m_pMapComponent[iSceneID].begin(), m_pMapComponent[iSceneID].end(), CCompare_String(pComponentTag));
	//auto& iter = m_pMapComponent[iSceneID].find(pComponentTag);

	if (iter == m_pMapComponent[iSceneID].end())
		return nullptr;

	return iter->second;
}

void CComponent_Manager::Free()
{
	for (size_t i = 0; i < m_iNumScene; i++)
	{
		for (auto& Pair : m_pMapComponent[i])
		{
			_ulong dwRefCont = 0;
			dwRefCont = Safe_Release(Pair.second);
		}
		m_pMapComponent[i].clear();
	}

	Safe_Delete_Array(m_pMapComponent);
}
