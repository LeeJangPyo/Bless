#ifndef Collider_Sphere_h__
#define Collider_Sphere_h__

#include "Component.h"

_BEGIN(Engine)
class CTexture;
class _ENGINE_DLL CCollider_Sphere : public CComponent
{
public:
	enum COLORTYPE
	{
		COLOR_RED, COLOR_GREEN, COLOR_YELLOW
	};
private:
	explicit	CCollider_Sphere(LPDIRECT3DDEVICE9	 pDevice);
	explicit	CCollider_Sphere(const CCollider_Sphere& rhs);
	virtual		~CCollider_Sphere(void) = default; 



public://Get
	const _vec3* Get_Center() const { 
		return &m_vCenter; }
	const _float* Get_Radius() const { 
		return &m_fRadius; }
	
	//Debug¿ë
	void Set_Center_Position(const _vec3* pCenterPos);	
	void Set_Radius(const _float& fRaidus);
	void Set_Parnet_Bone_Matrix(const _matrix* pmatParentBone){
		m_pmatParentBone = pmatParentBone;
	}
	void Set_Parnet_World_Matrix(const _matrix* pmatParentWorld) {
		m_pmatParentWorld = pmatParentWorld;
	}

public:
	HRESULT		Ready_Collider_Spehre();
	_int		Update_Collider_Sphere();
	void		Render_Collider_Sphere();
	
	HRESULT		SetUp_Collider_Spehre(const _vec3* pCenter,const _float& fRadius);
	HRESULT		SetUp_Collider_Spehre(const _matrix* pmatParentBone, const _matrix* pmatParentWorld,const _vec3* pCenterPos, const _float& fRadius);
	HRESULT		SetUp_Collider_Spehre(const _matrix* pmatParentBone, const _matrix* pmatParentWorld);
	
	
	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT pEffect, COLORTYPE eType);
	HRESULT		Save_ColliderInfo(const HANDLE& hFile, _ulong& dwByte);

	CCollider_Sphere&	operator=(const CCollider_Sphere& rhs);
private:
	_vec3		m_vCenter;
	_float		m_fRadius;
	
	_vec3		m_vOriginCenter;
	_float		m_fOriginRadius;

	_matrix		m_matWorld;
	_matrix		m_matCombine;

	const _matrix*				m_pmatParentBone = nullptr;
	const _matrix*				m_pmatParentWorld = nullptr;

	LPD3DXMESH				m_pMesh_Sphere;
	CTexture*				m_pTextureCom;
public:
	static	CCollider_Sphere*		Create(LPDIRECT3DDEVICE9 pDevice);
							
	CComponent*	Clone_Component() override;
protected:
	virtual		void	Free(void) override;
};
_END
#endif // Collider_Sphere_h__
