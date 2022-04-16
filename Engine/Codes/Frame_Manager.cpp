#include "..\Headers\Frame_Manager.h"
#include "Frame.h"

_IMPLEMENT_SINGLETON(CFrame_Manager)

CFrame_Manager::CFrame_Manager()
{
}

HRESULT CFrame_Manager::Add_Frame(const _tchar * pFrameTag, const _float& fCallCnt)
{
	CFrame*		pFrame = Find_Frame(pFrameTag);

	if (nullptr != pFrame)
		return E_FAIL;

	pFrame = CFrame::Create(fCallCnt);
	if (nullptr == pFrame)
		return E_FAIL;

	m_mapFrame.insert(MAPFRAME::value_type(pFrameTag, pFrame));

	return NOERROR;
}

_bool CFrame_Manager::Permit_Call(const _tchar * pFrameTag, const _float & fTimeDelta)
{
	CFrame*		pFrame = Find_Frame(pFrameTag);

	if (nullptr == pFrame)
		return false;

	return pFrame->Permit_Call(fTimeDelta);
}

CFrame * CFrame_Manager::Find_Frame(const _tchar * pFrameTag)
{
	auto iter = find_if(m_mapFrame.begin(), m_mapFrame.end(), CFinder_Tag(pFrameTag));

	if (iter == m_mapFrame.end())
		return nullptr;

	return iter->second;	
	//find_if(m_mapFrame.begin(), m_mapFrame.end(), [&](MAPFrame::value_type Pair) {return !lstrcmp(pFrameTag, Pair.first); });
	

}

void CFrame_Manager::Free(void)
{
	for (auto& Pair : m_mapFrame)
	{
		Safe_Release(Pair.second);
	}
	m_mapFrame.clear();

	
}
