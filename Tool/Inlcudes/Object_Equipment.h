#ifndef Object_Equipment_h__
#define Object_Equipment_h__

#include "Object_Mesh.h"

_BEGIN(Engine)
	class CMesh_Static;
_END


_BEGIN(Client)

class CObject_Equipment : public CObject_Mesh
{
private:
	explicit	CObject_Equipment(LPDIRECT3DDEVICE9 pDevice);
	explicit	CObject_Equipment(const CObject_Equipment& rhs);
	virtual		~CObject_Equipment(void) = default;

public:
	//set
	HRESULT	Set_ParentMatrix(const _tchar* pLayerTag,const _tchar* FrameTag);
	HRESULT	SetUp_ObjectInfo(const _tchar* pComponentMeshTag);

public:
	virtual HRESULT Ready_Prototype() override;
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

protected:
	virtual HRESULT Ready_Component() override;
	
private:
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT pEffect);

private:
	CMesh_Static*		m_pMeshCom;


	const _matrix*				m_pParentBoneMatrix = nullptr;
	const _matrix*				m_pParentWorldMatrix = nullptr;

	_matrix						m_matCombineWorld;
public:
	static CObject_Equipment*		Create(LPDIRECT3DDEVICE9 pDevice);
	virtual		CGameObject*		Clone_GameObject() override;

private:
	virtual void Free(void) override;


};

_END
#endif // Object_Equipment_h__
