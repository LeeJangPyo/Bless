#ifndef Box_Collider_h__
#define Box_Collider_h__

#include "Component.h"

_BEGIN(Engine)

class _ENGINE_DLL CBox_Collider : public CComponent
{
public:
	enum COLLTYPE
	{
		COL_FALSE, COL_TRUE, COL_END
	};

private:
	explicit	CBox_Collider(LPDIRECT3DDEVICE9	pDevice);
	virtual		~CBox_Collider(void);
public:
	const _vec3*	Get_Min(void) const { return &m_vMin; }
	const _vec3*	Get_Max(void) const { return &m_vMax; }
	const _matrix*	Get_WorldMatrix(void) const { return &m_matWorld; }
public:
	HRESULT		Ready_Box_Collider(/*const _vec3* pPosition,
								const _ulong& dwNumVertices,
								const _ulong& dwStride);*/);

	void		Render_Box_Collider(COLLTYPE eType, const _matrix* pBox_ColliderMatrix);

public:
	_vec3		m_vMin;
	_vec3		m_vMax;
	_vec3		m_vWorldMin;
	_vec3		m_vWorldMax;


	LPDIRECT3DVERTEXBUFFER9			m_pVB;
	LPDIRECT3DINDEXBUFFER9			m_pIB;
	LPDIRECT3DTEXTURE9				m_pTexture[COL_END];

	D3DXMATRIX						m_matWorld;

public:
	static CBox_Collider*		Create(LPDIRECT3DDEVICE9 pDevice);
		//const _vec3* pPosition,
		//const _ulong& dwNumVertices,
		//const _ulong& dwStride);

	CComponent* Clone_Component() override;
protected:
	virtual void	Free(void) override;
};

_END
#endif // Box_Collider_h__
