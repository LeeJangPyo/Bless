#ifndef QuadTree_h__
#define QuadTree_h__

#include "Base.h"

_BEGIN(Engine)

class CFrustum;
class _ENGINE_DLL CQuadTree : public CBase 
{
public:
	enum CHILD	{ CHILD_LT, CHILD_RT, CHILD_LB, CHILD_RB, CHILD_END };
	enum CORNER { CORNER_LT, CORNER_RT, CORNER_LB, CORNER_RB, CORNER_END };
	enum NEIGHBOR { NEIGHBOR_LEFT, NEIGHBOR_TOP, NEIGHBOR_RIGHT, NEIGHBOR_BOTTOM, NEIGHBOR_END };

private:
	explicit CQuadTree(void);
	virtual ~CQuadTree(void) = default;

public:
	HRESULT		Ready_QuadTree(const _ulong& dwCntX, const _ulong& dwCntZ);
	HRESULT		Ready_Neighbor(void);
	void		CullingForTerrain(Engine::CFrustum* pFrustum, 
									const _vec3* pVertex, 
									INDEX32* pIndex, 
									_ulong* pTrinCnt);

private:
	void		Make_ChildCorner(CHILD eType, 
									const _ulong& dwPLT,
									const _ulong& dwPRT,
									const _ulong& dwPLB,
									const _ulong& dwPRB,
									const _ulong& dwPCenter,
									_ulong* pLT,
									_ulong* pRT,
									_ulong* pLB,
									_ulong* pRB);
	void		Make_ChildQuadTree(void);
	void		Make_Neighbor(void);
	void		Ready_Child(const _ulong& dwLT, const _ulong& dwRT,
							const _ulong& dwLB, const _ulong& dwRB);
	_bool		LevelOfDetail(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pVertex);
	
private:
	CQuadTree*				m_pChild[CHILD_END]; 
	CQuadTree*				m_pNeighbor[NEIGHBOR_END];
	_ulong					m_dwCorner[CORNER_END];
	_ulong					m_dwCenter = 0;
	_float					m_fRadius = 0.f;

public:
	static		CQuadTree*	Create(const _ulong& dwCntX, const _ulong& dwCntZ);
	CQuadTree*	Create(const _ulong& dwLT, 
						const _ulong& dwRT, 
						const _ulong& dwLB, 
						const _ulong& dwRB);
	virtual void	Free(void);

};

_END
#endif // QuadTree_h__
