#ifndef Object_Static_h__
#define Object_Static_h__

#include "Object_Mesh.h"

_BEGIN(Engine)
class CMesh_Static;
class CCollider_Box;
_END


_BEGIN(Client)

class CObject_Static : public CObject_Mesh
{
private:
	explicit	CObject_Static(LPDIRECT3DDEVICE9 pDevice);
	explicit	CObject_Static(const CObject_Static& rhs);
	virtual		~CObject_Static(void) = default;

public:
	virtual HRESULT Ready_Prototype() override;
	virtual HRESULT Ready_GameObject() override;
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual _int	LastUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void	Render_GameObject() override;


	virtual HRESULT			Ready_Component() override;
public:
	HRESULT			SetUp_ObjectInfo(const _tchar* pComponentMeshTag);
	HRESULT			SetUp_ObjectInfo(const _matrix* pMatrix ,const _tchar* pComponentMeshTag, CMeshTab* pMeshTab);
	HRESULT			SetUp_ObjectInfo(const _vec3* pPosition, const _tchar* pComponentMeshTag, CMeshTab* pMeshTab);

	//_bool			Intersect_Static_Mesh(const RAY& rRay, 
	//										_ulong*	pFaceIndex, 
	//										_float *pU, _float *pV, _float *pDist,
	//										LPD3DXBUFFER* ppAllHits, _ulong*  pCountOfHits);
	_bool			Intersect_Static_Mesh(const RAY& rRay, _vec3* pOutPosition);
private:
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	void			Key_Input(const _float fTimeDelta);


private:
	CMesh_Static*		m_pMeshCom;
	CCollider_Box*		m_pColliderCom;

	

public:
	static CObject_Static*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual	CGameObject*		Clone_GameObject() override;

protected:
	virtual void Free(void) override;



};

_END

#endif // Object_Static_h__
