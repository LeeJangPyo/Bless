#ifndef Sword_h__
#define Sword_h__

#include "GameObject_Weapon.h"

_BEGIN(Engine)
class CCollider_Sphere;
_END

_BEGIN(Client)
class CLups;
class CCollision_Manager;

class CTrail;

class CSword final : public CGameObject_Weapon
{
public:
	explicit CSword(LPDIRECT3DDEVICE9 pDevice);
	explicit CSword(const CSword& rhs);
	virtual ~CSword() = default;

public:
	
public:
	virtual HRESULT Ready_Prototype() override;
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

public:
	HRESULT	SetUp_SworldInfo();

	void	Begin_Trail();
	void	End_Trail();

	CCollider_Sphere*	Get_Collider() { 
		return m_pColliderCom; }

private:
	HRESULT	Ready_Component();

private:
	HRESULT	SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	HRESULT	Set_ParentMatrix(const _tchar* pLayerTag, const _tchar* FrameTag);

private:
	CCollider_Sphere*			m_pColliderCom = nullptr;

	CLups*						m_pLups;

	const DAMAGE*				m_pLupsDamage = nullptr;

	CTrail*						m_pTrail = nullptr;

	_float						m_fTimeAcc;
public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject* Clone_GameObject() override;
protected:
	virtual void Free() override;

};

_END
#endif // Sword_h__