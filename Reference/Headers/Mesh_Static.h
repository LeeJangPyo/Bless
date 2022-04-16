#ifndef Mesh_Static_h__
#define Mesh_Static_h__

#include "Mesh.h"

_BEGIN(Engine)

class _ENGINE_DLL CMesh_Static : public CMesh
{
private:
	explicit	CMesh_Static(LPDIRECT3DDEVICE9 pDevice);
	explicit	CMesh_Static(const CMesh_Static& rhs);
	virtual		~CMesh_Static(void);

	//°Ù
public:
	_ulong			Get_NumVertex(void)	const { return m_dwNumVertex; }
	_ulong			Get_Stride(void)	const { return m_dwStride; }
	const _vec3*	Get_VtxPos(void)	const { return m_pVtxPos; }
	const LPD3DXMESH	Get_Mesh(void) const { return m_pMesh; }
public:
	HRESULT			Ready_Mesh(const _tchar* pFilePath, const _tchar* pFileName);
	void			Render_Mesh(void);
	void			Render_Mesh(LPD3DXEFFECT pEffect);

private:
	_bool		Find_Alpha(const char* pFileName);
	HRESULT		Load_Texture(const _tchar* pFileName, const _tchar * pFilePath, const _tchar* pTextureKey, LPDIRECT3DTEXTURE9* ppTexture);

private:
	LPD3DXMESH				m_pMesh;
	LPD3DXBUFFER			m_pAdjacency;
	LPD3DXBUFFER			m_pSubsetBuffer;
	D3DXMATERIAL*			m_pSubsets;
	_ulong					m_dwSubsetCnt;

	//LPDIRECT3DTEXTURE9*		m_ppTexture;
	MESHTEXTURE*			m_pTextures;

	LPD3DXMESH				m_pOriMesh;
	_ulong					m_dwNumVertex;
	_ulong					m_dwStride;
	_vec3*					m_pVtxPos;

public:
	static	CMesh_Static*		Create(LPDIRECT3DDEVICE9 pDevice,
		const _tchar* pFilePath,
		const _tchar* pFileName);

	virtual	CComponent*			Clone_Component(void) override;

protected:
	virtual void				Free(void);

};

_END

#endif // Mesh_Static_h__
