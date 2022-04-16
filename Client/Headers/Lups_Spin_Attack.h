#ifndef Lups_Spin_Attack_h__
#define Lups_Spin_Attack_h__

#include "State.h"

_BEGIN(Engine)
class CTransform;
_END

_BEGIN(Client)
class CEffectGroup;
class CLups;
class CUI_Frame;
class CLups_Spin_Attack : public CState
{
private:
	explicit CLups_Spin_Attack();
	virtual ~CLups_Spin_Attack() = default;

public:

	// CState을(를) 통해 상속됨
	virtual HRESULT Begin() override;
	virtual _int Update(const _float & fTimeDelta) override;
	virtual _int LastUpdate(const _float& fTimeDelta) override;
	virtual HRESULT End() override;

private:
	HRESULT	 Ready_State(CLups* pLups);

private:
	CLups*	m_pLups = nullptr;
	CUI_Frame*	m_pUIFrame = nullptr;

	DAMAGE			m_Damage;
	CEffectGroup*	m_pEffect_Spin_Attack = nullptr;
public:
	static CLups_Spin_Attack*	Create(CLups* pLups);
protected:
	virtual void Free() override;

};

_END


#endif // Lups_Spin_Attack_h__
