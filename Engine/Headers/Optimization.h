#ifndef Optimization_h__
#define Optimization_h__

#include "Component.h"
#include "QuadTree.h"
#include "Frustum.h"

_BEGIN(Engine)


class _ENGINE_DLL COptimization final : public CComponent
{

private:
	explicit COptimization(LPDIRECT3DDEVICE9 pDevice);
	explicit COptimization(const COptimization& rhs);
	virtual ~COptimization(void) = default;

public:
	HRESULT	Ready_Optimization(_bool bUsed, const _ulong& dwCntX, const _ulong& dwCntZ);
	void	Update_Component(const _float& fTimeDelta) {}
	_bool	Is_InFrustum_ForObject(const _vec3* pPosInWorld, const _float fRadius);
	void	Is_InFrustum_ForTerrain(const _vec3* pVertexPos,
									const _ulong& dwCntX,
									const _ulong& dwCntZ,
									Engine::INDEX32* pIndex,
									_ulong* pTriCnt);

private:
	LPDIRECT3DDEVICE9			m_pGraphicDev	= nullptr;
	CFrustum*					m_pFrustum		= nullptr;
	CQuadTree*					m_pQuadTree		= nullptr;
	_bool						m_bClone;

public:
	static COptimization*		Create(LPDIRECT3DDEVICE9 pDevice,
										_bool bUsed, 
										const _ulong& dwCntX, 
										const _ulong& dwCntZ);
	CComponent*					Clone_Component(void) override;
protected:
	virtual void				Free(void);

};

_END
#endif // Optimization_h__
