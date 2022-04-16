#include "stdafx.h"
#include "..\Headers\UI_Resources_Loader.h"
#include <io.h>

#include "Management.h"



CUI_Resources_Loader::CUI_Resources_Loader(LPDIRECT3DDEVICE9 pDevice)
	:m_pDevice(pDevice)
{
	m_pDevice->AddRef();
}


CUI_Resources_Loader::~CUI_Resources_Loader()
{
	Safe_Release(m_pDevice);
}

void CUI_Resources_Loader::UI_Resources_Load(const wstring& wstrFilePath)
{
	_wfinddata_t		FindData;

	wstring		wstrCombinedPath;
	long		handle;
	int			iContinue = 1;

	handle = _wfindfirst(wstrFilePath.c_str(), &FindData);
	if (handle == -1)
		return;

	while (iContinue != -1)
	{
		iContinue = _wfindnext(handle, &FindData);
	
		if (!lstrcmp(FindData.name, L".") || !lstrcmp(FindData.name, L"..") || iContinue == -1)
			continue;

		wstrCombinedPath = wstrFilePath.substr(0, wstrFilePath.length() - 3) + FindData.name;

		//파일이 있으면
		if (FindData.size != 0)
		{
			_int iFileCount = Get_FileCount(wstrFilePath);

			wstring		wstrTag = wstrCombinedPath;
			PathRemoveFileSpec((LPWSTR)wstrTag.c_str());
			wstrTag = PathFindFileName(wstrTag.c_str());

			wstring		wstrComponentTag = L"Component_Texture_" + wstrTag;
			wstring		wstrExtension = PathFindExtension(FindData.name);
			wstring		wstrPath = wstrCombinedPath;
			PathRemoveFileSpec((LPWSTR)wstrPath.c_str());


			_tchar		szFilePath[MAX_PATH];


			wsprintf(szFilePath, L"%s/%s_%%d%s", wstrPath.c_str(), wstrTag.c_str(), wstrExtension.c_str());

			//For. Texture Porototype Component Create;
			CManagement::GetInstance()->Add_Prototype_Component(SCENE_EDIT, wstrComponentTag.c_str(),
				CTexture::Create(m_pDevice, szFilePath, CTexture::TEXTURETYPE::TEX_NORMAL, iFileCount));
			
			_findclose(handle);
			return;
		}
		else
			UI_Resources_Load(wstrCombinedPath + L"\\*.*");
	}

	_findclose(handle);
}

_int CUI_Resources_Loader::Get_FileCount(const wstring & wstrFilePath)
{
	_wfinddata_t		FindData;

	wstring		wstrCombinedPath;
	_long		handle;
	_int		iContinue = 1;
	_int		iFileCount = 0;

	handle = _wfindfirst(wstrFilePath.c_str(), &FindData);
	if (handle == -1)
		return -1;

	while (iContinue != -1)
	{
		iContinue = _wfindnext(handle, &FindData);
		if (!lstrcmp(FindData.name, L".") || !lstrcmp(FindData.name, L"..") || iContinue == -1)
			continue;

		iFileCount++;
	}

	_findclose(handle);

	return iFileCount;
}

HRESULT CUI_Resources_Loader::Effect_Mesh_Load(const wstring & wstrFilePath)
{
	_wfinddata_t		FindData;

	wstring		wstrCombinedPath;
	long		handle;
	int			iContinue = 1;

	handle = _wfindfirst(wstrFilePath.c_str(), &FindData);
	if (handle == -1)
		return E_FAIL;

	while (iContinue != -1)
	{
		iContinue = _wfindnext(handle, &FindData);

		if (!lstrcmp(FindData.name, L".") || !lstrcmp(FindData.name, L"..") || iContinue == -1)
			continue;

		wstrCombinedPath = wstrFilePath.substr(0, wstrFilePath.length() - 3);

		//파일이 있으면
		if (FindData.size != 0)
		{
			wstring	wstrExtension = PathFindExtension(FindData.name);
			if (wstrExtension == L".tga" || wstrExtension == L"")
				continue;

			wstring		wstrTag = FindData.name;
			PathRemoveExtension((LPWSTR)wstrTag.c_str());
			
			wstring		wstrComponentTag = L"Component_Mesh_Static_" + wstrTag;
			//PathRemoveFileSpec((LPWSTR)wstrCombinedPath.c_str());

			//For. Mesh Porototype Component Create;
			if (FAILED(CManagement::GetInstance()->Add_Prototype_Component(SCENE_EDIT, wstrComponentTag.c_str(),
				CMesh_Static::Create(m_pDevice, wstrCombinedPath.c_str(), FindData.name))))
			{
				_findclose(handle);
				return E_FAIL;
			}
		}
	}

	_findclose(handle);

	return S_OK;
}

HRESULT CUI_Resources_Loader::Effect_Texture_Load(const wstring & wstrFilePath)
{
	_wfinddata_t		FindData;

	wstring		wstrCombinedPath;
	long		handle;
	int			iContinue = 1;

	handle = _wfindfirst(wstrFilePath.c_str(), &FindData);
	if (handle == -1)
		return S_OK;

	while (iContinue != -1)
	{
		iContinue = _wfindnext(handle, &FindData);

		if (!lstrcmp(FindData.name, L".") || !lstrcmp(FindData.name, L"..") || iContinue == -1)
			continue;

		wstrCombinedPath = wstrFilePath.substr(0, wstrFilePath.length() - 3) + FindData.name;

		//파일이 있으면
		if (FindData.size != 0)
		{
			wstring		wstrTag = FindData.name;
			PathRemoveExtension((LPWSTR)wstrTag.c_str());

			wstring		wstrComponentTag = L"Component_Texture_" + wstrTag;

			//For. Texture Porototype Component Create;
			if (FAILED(CManagement::GetInstance()->Add_Prototype_Component(SCENE_EDIT, wstrComponentTag.c_str(),
				CTexture::Create(m_pDevice, wstrCombinedPath.c_str(), CTexture::TEXTURETYPE::TEX_NORMAL))))
			{
				_findclose(handle);
				return E_FAIL;
			}

		}
	}

	_findclose(handle);

	return S_OK;
}
