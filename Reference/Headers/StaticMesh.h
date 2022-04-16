#ifndef StaticMesh_h__
#define StaticMesh_h__

#include "Mesh.h"

_BEGIN(Engine)

class _ENGINE_DLL CStaticMesh : public CMesh
{
private:
	explicit	CStaticMesh(LPDIRECT3DDEVICE9 pDevice);
	explicit	CStaticMesh(const CStaticMesh& rhs);
	virtual		~CStaticMesh(void);

	//°Ù
public:
	_ulong			Get_NumVertex(void)	const { return m_dwNumVertex; }
	_ulong			Get_Stride(void)	const { return m_dwStride; }
	const _vec3*	Get_VtxPos(void)	const { return m_pVtxPos; }
public:
	HRESULT			Ready_StaticMesh(const _tchar* pFilePath, const _tchar* pFileName);
	void			Render_StaticMesh(void);
	void			Render_StaticMesh(LPD3DXEFFECT pEffect);

private:
	_bool		Find_Alpha(const char* pFileName);

private:
	LPD3DXMESH				m_pMesh;
	LPD3DXBUFFER			m_pAdjacency;
	LPD3DXBUFFER			m_pSubsetBuffer;
	D3DXMATERIAL*			m_pSubsets;
	_ulong					m_dwSubsetCnt;

	LPDIRECT3DTEXTURE9*		m_ppTexture;

	LPD3DXMESH				m_pOriMesh;
	_ulong					m_dwNumVertex;
	_ulong					m_dwStride;
	_vec3*					m_pVtxPos;

public:
	static	CStaticMesh*		Create(LPDIRECT3DDEVICE9 pDevice,
		const _tchar* pFilePath,
		const _tchar* pFileName);

	virtual	CComponent*			Clone_Component(void) override;

protected:
	virtual void				Free(void);

};

_END

#endif // StaticMesh_h__
