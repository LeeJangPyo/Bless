#ifndef Staff_h__
#define Staff_h__

#include "GameObject_Weapon.h"

_BEGIN(Engine)
class CCollider_Sphere;
_END

_BEGIN(Client)
class CSire;
class CCollision_Manager;
class CStaff final : public CGameObject_Weapon
{
public:
	explicit CStaff(LPDIRECT3DDEVICE9 pDevice);
	explicit CStaff(const CStaff& rhs);
	virtual ~CStaff() = default;

public:
	
public:
	virtual HRESULT Ready_Prototype() override;
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

public:
	HRESULT	SetUp_StaffInfo();

private:
	HRESULT	Ready_Component();
private:
	HRESULT	SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	HRESULT	Set_ParentMatrix(const _tchar* pLayerTag, const _tchar* FrameTag);

private:
	//CCollider_Sphere*			m_pColliderCom = nullptr;

	CSire*						m_pSire;
public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject* Clone_GameObject() override;
protected:
	virtual void Free() override;

};

_END
#endif // Staff_h__