#ifndef Frustum_h__
#define Frustum_h__

#include "QuadTree.h"

_BEGIN(Engine)

class _ENGINE_DLL CFrustum : public CBase
{
private:
	explicit CFrustum(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CFrustum(void) = default;

public:
	LPDIRECT3DDEVICE9	GetDevice(void) { return m_pDevice; }
	HRESULT	Ready_Frustum(void);
	_bool	IsIn_Frustum(const _vec3* pPosition);
	_bool	IsIn_Frustum(const _vec3* pPosition, const _float& fRadius);
	_bool	Is_InFrustum_ForObject(const _vec3* pPosInWorld, const _float fRadius);
	void	Is_InFrustum_ForTerrain(const _vec3* pVertexPos,
									const _ulong& dwCntX,
									const _ulong& dwCntZ,
									Engine::INDEX32* pIndex,
									_ulong* pTriCnt, 
									CQuadTree* pQuadTree);

private:
	LPDIRECT3DDEVICE9			m_pDevice = nullptr;
	_vec3						m_vPoint[8];
	D3DXPLANE					m_Plane[6];

public:
	static	CFrustum*			Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual	void				Free(void);
};

_END
#endif // Frustum_h__
