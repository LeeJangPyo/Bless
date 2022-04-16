#ifndef Object_Dynamic_h__
#define Object_Dynamic_h__

#include "Object_Mesh.h"


_BEGIN(Engine)
class CMesh_Dynamic;
class CCollider_Sphere;
class CFont_Manager;
_END


_BEGIN(Client)

class CObject_Dynamic : public CObject_Mesh	
{
public:
	typedef struct tagColliderInfo
	{
		string	strBoneTag;
		CCollider_Sphere*	pCollider;
	}COLLIDERINFO;

private:
	explicit	CObject_Dynamic(LPDIRECT3DDEVICE9 pDevice);
	explicit	CObject_Dynamic(const CObject_Dynamic& rhs);
	virtual		~CObject_Dynamic(void) = default;


	//Get
public:
	_uint		Get_MaxAnimation(void);
	D3DXFRAME*	Get_RootFrame(void);
	//Set
public:
	void		Set_Animation(const _int&  iIndex);

	HRESULT		Set_ColliderInfo(const _tchar* pColliderTag,const _float& fX, const _float& fY, const _float& fZ, const _float& fRadius);
	HRESULT		Create_Collider(const _tchar* pColliderTag, const _tchar*	pParentBoneTag, const _float& fRadius, CCollider_Sphere** ppCollider);
	HRESULT		Change_Collider_Parent(const _tchar* pColliderTag, const _tchar*	pFrameTag);
	HRESULT		Save_ColliderInfo(const HANDLE& hFile, _ulong &dwByte);
	HRESULT		Load_ColliderInfo(const HANDLE& hFile, _ulong &dwByte);
public:

	virtual HRESULT Ready_Prototype() override;
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

public:
	HRESULT			SetUp_ObjectInfo(const _matrix* pMatrix, const _tchar* pComponentMeshTag, CMeshTab* pMeshTab);
	HRESULT			SetUp_ObjectInfo(const _vec3* pPosition, const _tchar* pComponentMeshTag, CMeshTab* pMeshTab);
	HRESULT			SetUp_ObjectInfo(const _tchar* pComponentMeshTag);
private:
	virtual HRESULT Ready_Component() override;

	void			Check_KeyInput(const _float& fTimeDelta);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	void			Render_Frame(D3DXFRAME* pFrame);

	_bool			Intersect_Dynamic_Mesh(const RAY& rRay, _vec3* pOutPosition);
private:
	CMesh_Dynamic*		m_pMeshCom = nullptr;
	CFont_Manager*		m_pFont_Manager = nullptr;

	//Key : 콜라이더 테그
	// COLLIDER : 본정보와 충돌체객체를 가지고잇음
	map<wstring, COLLIDERINFO*>	m_mapCollider;

public:
	static CObject_Dynamic*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject* Clone_GameObject() override;
protected:
	virtual void Free(void) override;


};


_END
#endif // Object_Dynamic_h__
