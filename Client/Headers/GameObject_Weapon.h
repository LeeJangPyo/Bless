#ifndef GameObject_Weapon_h__
#define GameObject_Weapon_h__


#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
class CTransform;
class CRenderer;
class CMesh_Static;
class CShader;
_END

_BEGIN(Client)


class CGameObject_Weapon abstract : public CGameObject
{
public:
	explicit CGameObject_Weapon(LPDIRECT3DDEVICE9 pDevice);
	explicit CGameObject_Weapon(const CGameObject_Weapon& rhs);
	virtual ~CGameObject_Weapon() = default;

public:
	//Set
	void	Set_Equip(_bool	bIsEquip) {
		m_bIsEquiped = bIsEquip;
	}

	const _vec3*	Get_NearPosition() { return &m_vTrail_NearPosition; }
	const _vec3*	Get_FarPosition() { return &m_vTrail_FarPosition; }
public:
	virtual HRESULT Ready_Prototype() override;
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;


protected:
	HRESULT	SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	HRESULT	Ready_Component();

protected:
	const _matrix*				m_pParentBoneMatrix = nullptr;
	const _matrix*				m_pParentWorldMatrix = nullptr;

	_matrix						m_matCombineWorld;

	_bool						m_bIsEquiped;

	_vec3						m_vTrail_NearPosition;
	_vec3						m_vTrail_FarPosition;


	//Component
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CMesh_Static*		m_pMeshCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
public:
	virtual CGameObject* Clone_GameObject() = 0;

protected:
	virtual void Free() override;

};

_END

#endif // GameObject_Weapon_h__
