#ifndef UI_Resources_Loader_h__
#define UI_Resources_Loader_h__

#include "Defines.h"
#include "Base.h"

class CUI_Resources_Loader
{
public:
	CUI_Resources_Loader(LPDIRECT3DDEVICE9 pDevice);
	~CUI_Resources_Loader();

public:
	void UI_Resources_Load(const wstring& wstrFilePath);
	_int Get_FileCount(const wstring& wstrFilePath);

	HRESULT Effect_Mesh_Load(const wstring& wstrFilePath);
	HRESULT	Effect_Texture_Load(const wstring& wstrFilePath);

private:
	LPDIRECT3DDEVICE9 m_pDevice;
};

#endif // UI_Resources_Loader_h__