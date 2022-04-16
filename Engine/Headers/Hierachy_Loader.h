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

	// 뼈대를 만들어주는 함수
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name,
		LPD3DXFRAME *ppNewFrame);

	// 메쉬 정보를 저장하는 컨테이너(메쉬의 살을 만들어준다고 보면 됨) - 하나의 STATICMESH가 컨테이너 원소
	STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR Name,
									CONST D3DXMESHDATA *pMeshData,
									CONST D3DXMATERIAL *pMaterials,
									CONST D3DXEFFECTINSTANCE *pEffectInstances,
									DWORD NumMaterials,
									CONST DWORD *pAdjacency,
									LPD3DXSKININFO pSkinInfo,
									LPD3DXMESHCONTAINER *ppNewMeshContainer);

	// 뼈대를 삭제하는 함수
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
	
	// 살을 제거하는 함수
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree);
private:
	//뼈(프레임)에 이름을 넣어주는 함수
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
