#ifndef Collider_Box_h__
#define Collider_Box_h__

#include "Component.h"

_BEGIN(Engine)
class CTexture;
class CTransform;

class _ENGINE_DLL CCollider_Box : public CComponent
{
public:
	enum COLORTYPE
	{
		COLOR_RED, COLOR_GREEN, COLOR_YELLOW
	};

private:
	explicit	CCollider_Box(LPDIRECT3DDEVICE9	pDevice);
	explicit	CCollider_Box(const CCollider_Box& rhs);
	virtual		~CCollider_Box(void) = default;
public:
	const _vec3*	Get_Min(void) const { return &m_vMin; }
	const _vec3*	Get_Max(void) const { return &m_vMax; }
	const _matrix*	Get_WorldMatrix(void) const;
public:
	HRESULT		Ready_Collider_Box();

	HRESULT		SetUp_Collider_Box(const _vec3* pPosition,
										const _ulong& dwNumVertices,
										const _ulong& dwStride);
	HRESULT		SetUp_Collider_Box();
	
	void		Render_Collider_Box();
	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT pEffect, COLORTYPE eType, const _matrix* pmatWorld);

public:
	_vec3		m_vMin;
	_vec3		m_vMax;
	_vec3		m_vWorldMin;
	_vec3		m_vWorldMax;


	LPDIRECT3DVERTEXBUFFER9			m_pVB;
	LPDIRECT3DINDEXBUFFER9			m_pIB;
	CTexture*						m_pTextureCom;
	CTransform*						m_pTransformCom;

public:
	static CCollider_Box*		Create(LPDIRECT3DDEVICE9 pDevice);
		//const _vec3* pPosition,
		//const _ulong& dwNumVertices,
		//const _ulong& dwStride);

	CComponent* Clone_Component() override;
protected:
	virtual void	Free(void) override;
};

_END
#endif // Collider_Box_h__
