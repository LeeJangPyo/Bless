#ifndef Calculator_h__
#define Calculator_h__

#include "Component.h"


_BEGIN(Engine)

class CCollider_Sphere;
class CCollider_Box;
class CBuffer_TerrainTex;
class CTransform;

class _ENGINE_DLL CCalculator : public CComponent
{
public:
	explicit CCalculator(LPDIRECT3DDEVICE9	pDevice);
	explicit CCalculator(const CCalculator& rhs);
	virtual ~CCalculator() = default;

public:
	HRESULT		Ready_Calculator(void);

	_float		Compute_HeightOnTerrain(const _vec3* pPosition,
		const _vec3* pTerrainVtxPos,
		const _ulong& dwCntX,
		const _ulong& dwCntZ,
		const _float& fItv);

	_vec3		Picking_OnTerrain(HWND hWnd,
		const CBuffer_TerrainTex* pTerrainBufferCom,
		const CTransform* pTransform);

	_vec3		Picking_OnTerrain(const RAY& Ray,
		const CBuffer_TerrainTex* pTerrainBufferCom,
		const CTransform* pTransform);

	_bool		Check_CollisionAABB(const _vec3* pDestMin, const _vec3* pDestMax, const _matrix* pDestWorld,
		const _vec3* pSrcMin, const _vec3* pSrcMax, const _matrix* pSrcWorld);

	_bool		Picking_Sphere(const RAY& rRay, const CCollider_Sphere* pCollider);

	_bool		Picking_Triangle(RAY& tRay, const _vec3* pPoint);

	_bool		Picking_AABB(const _vec3* pMin, const _vec3* pMax, RAY& tRay, _vec3* pIntersectPoint);



	RAY		Transform_WorldToLocal(const RAY& rRay, const _matrix * pmatWorld);
	RAY		Transform_ScreenPosToRay(HWND hWnd);
	RAY		Transform_ScreenPosToRayLocal(HWND hWnd, const _matrix* pWorld);
	RAY		Transform_ScreenPosToRay(const _vec2 *pScreenPos);
	RAY		Transform_ScreenPosToRayLocal(const _vec2 *pScreenPos, const _matrix* pWorld);

	_vec3	 SpringDamp(const _vec3* pCurrentPos, 
						const _vec3* pTargetPos,
						const _vec3*	pPrevTargetPos, 
						const _float& fTimeDelta, 
						const _float& fSpringConst, 
						const _float& fDmapConst, 
						const _float& fSpringLen);
public:
	virtual CComponent* Clone_Component() override;
	static CCalculator*	Create(LPDIRECT3DDEVICE9 pDevice);
protected:
	virtual void Free() override;

};
_END

#endif // Calculator_h__