#include "Font_Manager.h"


_IMPLEMENT_SINGLETON(CFont_Manager)


Engine::CFont_Manager::CFont_Manager(void)
{
	
}




HRESULT Engine::CFont_Manager::Ready_Font(LPDIRECT3DDEVICE9 pDevice, const _tchar* pFontTag, const _tchar* pFontType, const _uint& iWidth, const _uint& iWeight, const _uint& iHeight)
{
	CFont*		pFont = nullptr;

	pFont = Find_Font(pFontTag);
	if (nullptr != pFont)
		return E_FAIL;

	pFont = CFont::Create(pDevice, pFontType, iWidth, iHeight, iWeight);
	if (pFont == nullptr)
		return E_FAIL;

	m_mapFont.emplace(pFontTag, pFont);

	return S_OK;
}

void Engine::CFont_Manager::Render_Font(const _tchar* pFontTag, const _tchar* pString, const _vec2* pPosition, D3DXCOLOR Color)
{
	CFont*		pFont = Find_Font(pFontTag);
	if (pFont == nullptr)
	{
		_TAGMSG_BOX(L"Font Object Don't Find, in Render_Font", L"CFont_Manager");
		return;
	}

	pFont->Render_Font(pString, pPosition, Color);
}
void Engine::CFont_Manager::Render_Font(const _tchar* pFontTag, const _tchar* pString, const _vec3* pPosition, D3DXCOLOR Color)
{
	CFont*		pFont = Find_Font(pFontTag);
	if (pFont == nullptr)
	{
		_TAGMSG_BOX(L"Font Object Don't Find, in Render_Font", L"CFont_Manager");
		return;
	}

	pFont->Render_Font(pString, pPosition, Color);


}

Engine::CFont* Engine::CFont_Manager::Find_Font(const _tchar* pFontTag)
{
	auto	iter = find_if(m_mapFont.begin(), m_mapFont.end(), CFinder_Tag(pFontTag));
	if (iter == m_mapFont.end())
		return nullptr;

	return iter->second;
}

void Engine::CFont_Manager::Free(void)
{
	for(auto& Pair : m_mapFont)
		Safe_Release(Pair.second);
	
	m_mapFont.clear();
}

