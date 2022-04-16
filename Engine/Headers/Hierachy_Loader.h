#ifndef Hierachy_Loader_h__
#define Hierachy_Loader_h__

#include "Engine_Defines.h"

_BEGIN(Engine)

class CHierachy_Loader : public ID3DXAllocateHierarchy
{
private:
	explicit	CHierachy_Loader(LPDIRECT3DDEVICE9	pDevice,
		const _tchar* pFilePath);
	virtual		~CHierachy_Loader(void);


public:
	STDMETHOD(Ready_Loader)(void);

	// ���븦 ������ִ� �Լ�
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name,
		LPD3DXFRAME *ppNewFrame);

	// �޽� ������ �����ϴ� �����̳�(�޽��� ���� ������شٰ� ���� ��) - �ϳ��� STATICMESH�� �����̳� ����
	STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR Name,
									CONST D3DXMESHDATA *pMeshData,
									CONST D3DXMATERIAL *pMaterials,
									CONST D3DXEFFECTINSTANCE *pEffectInstances,
									DWORD NumMaterials,
									CONST DWORD *pAdjacency,
									LPD3DXSKININFO pSkinInfo,
									LPD3DXMESHCONTAINER *ppNewMeshContainer);

	// ���븦 �����ϴ� �Լ�
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
	
	// ���� �����ϴ� �Լ�
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree);
private:
	//��(������)�� �̸��� �־��ִ� �Լ�
	void		Allocate_Name(char** ppName, const char* pFrameName);
	HRESULT		Load_Texture(const _tchar* pFullName, const _tchar* pTextureKey, LPDIRECT3DTEXTURE9* ppTexture);
private:
	LPDIRECT3DDEVICE9		m_pDevice;
	const _tchar*			m_pFilePath;

public:
	static CHierachy_Loader*		Create(LPDIRECT3DDEVICE9 pDevice, const _tchar* pFilePath);

	_ulong	Release(void);
};

_END

#endif // Hierachy_Loader_h__
