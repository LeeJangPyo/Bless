#ifndef FireBall_h__
#define FireBall_h__

#include "GameObject.h"

_BEGIN(Client)
class CEffectGroup;

class CFireBall final : public CGameObject
{
public:
	explicit CFireBall(LPDIRECT3DDEVICE9 pDevice);
	explicit CFireBall(const CFireBall& rhs);
	virtual ~CFireBall() = default;

public:
	virtual HRESULT Ready_Prototype() override;
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

private:
	CEffectGroup*	m_pEffectGroup;

public:
	virtual CGameObject* Clone_GameObject() override;
	static CFireBall*	Create(LPDIRECT3DDEVICE9 pDevice);
protected:
	virtual void Free() override;

};

_END

#endif // FireBall_h__