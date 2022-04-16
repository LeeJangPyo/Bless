#ifndef Shield_h__
#define Shield_h__

#include "GameObject_Weapon.h"

_BEGIN(Client)

class CShield final : public CGameObject_Weapon
{
public:
	explicit CShield(LPDIRECT3DDEVICE9 pDevice);
	explicit CShield(const CShield& rhs);
	virtual ~CShield() = default;


public:
	virtual HRESULT Ready_Prototype() override;
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

public:
	HRESULT	SetUp_ShieldInfo();

private:
	HRESULT	Ready_Component();
private:
	HRESULT	SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	HRESULT	Set_ParentMatrix(const _tchar* pLayerTag, const _tchar* FrameTag);

public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject* Clone_GameObject() override;
protected:
	virtual void Free() override;

};
_END

#endif // Shield_h__
