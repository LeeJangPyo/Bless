#ifndef Sphere_Collider_h__
#define Sphere_Collider_h__

#include "Component.h"

_BEGIN(Engine)

class _ENGINE_DLL CSphere_Collider : public CComponent
{
public:
	enum COLLTYPE
	{
		COL_FALSE, COL_TRUE, COL_END
	};
	
private:
	explicit	CSphere_Collider(LPDIRECT3DDEVICE9	 pDevice);
	virtual		~CSphere_Collider(void);

public:
	void		Plus_Center(_vec3& pRhs);

public:
	_float		Get_Radius(void);

public:
	HRESULT		Ready_Collider(/*const _vec3* pCenter,
								const _float& fRadius*/);

	HRESULT		Render_Collider(COLLTYPE eType);

public:
	_vec3		m_vCenter;
	_float		m_fRadius;

	LPD3DXMESH				m_pSphereMesh;
	LPDIRECT3DTEXTURE9		m_pTexture[COL_END];
	D3DXMATRIX				m_matWorld;
public:
	static	CSphere_Collider*		Create(LPDIRECT3DDEVICE9 pDevice);
									//const _vec3* pCenter,
									//const _float& fRadius);
	CComponent*	Clone_Component() override;
protected:
	virtual		void	Free(void) override;


};


_END


#endif // Sphere_Collider_h__
