#ifndef Lups_Taunt_h__
#define Lups_Taunt_h__

#include "State.h"

_BEGIN(Engine)
class CTransform;
_END

_BEGIN(Client)
class CEffectGroup;
class CLups;
class CUI_Frame;
class CLups_Taunt : public CState
{
private:
	explicit CLups_Taunt();
	virtual ~CLups_Taunt() = default;

public:

	// CState을(를) 통해 상속됨
	virtual HRESULT Begin() override;
	virtual _int Update(const _float & fTimeDelta) override;
	virtual _int LastUpdate(const _float& fTimeDelta) override;
	virtual HRESULT End() override;

private:
	HRESULT	 Ready_State(CLups* pLups);

private:
	CLups*	m_pLups;
	CUI_Frame*	m_pUIFrame;

	DAMAGE			m_Damage;
	CEffectGroup*	m_pEffect_Taunt;
public:
	static CLups_Taunt*	Create(CLups* pLups);
protected:
	virtual void Free() override;

};

_END


#endif // Lups_Taunt_h__
