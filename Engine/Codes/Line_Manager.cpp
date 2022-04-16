#include "Line_Manager.h"
#include "Line.h"

_IMPLEMENT_SINGLETON(CLine_Manager)

Engine::CLine_Manager::CLine_Manager(void)
{

}


Engine::CLine_Manager::~CLine_Manager(void)
{

}

HRESULT Engine::CLine_Manager::Ready_Line(LPDIRECT3DDEVICE9 pDevice,
	const _tchar* pLineTag,
	const _float& fWidth, 
	const _bool& bAntiAlias,
	const _bool& bGLLines,
	const _long& dwPattern, 
	const _float& fPatScale)
{
	CLine*		pLine = nullptr;

	pLine = Find_Line(pLineTag);
	if (nullptr != pLine)
		return E_FAIL;

	pLine = CLine::Create(pDevice, fWidth, bAntiAlias, bGLLines, dwPattern, fPatScale);
	if (nullptr == pLine)
		return E_FAIL;

	m_mapLine.emplace(pLineTag, pLine);

	return S_OK;
}

void Engine::CLine_Manager::Render_Line(const _tchar* pLineTag, 
									_vec3* pVertexArray,
									const _long& dwArrayCount,
									const D3DXCOLOR& Color)
{
	CLine*		pLine = Find_Line(pLineTag);
	if(pLineTag == nullptr)
	{
		_TAGMSG_BOX(L"Dont Find Line Object, in RenderLine", L"CLine_Manager");
	}

	pLine->Render_Line(pVertexArray, dwArrayCount, Color);
}

Engine::CLine* Engine::CLine_Manager::Find_Line(const _tchar* pLineTag)
{
	auto	iter = find_if(m_mapLine.begin(), m_mapLine.end(), CFinder_Tag(pLineTag));
	if (iter == m_mapLine.end())
		return nullptr;

	return iter->second;
}

void Engine::CLine_Manager::Free(void)
{
	for (auto& Pair : m_mapLine)
		Safe_Release(Pair.second);
	m_mapLine.clear();
}

