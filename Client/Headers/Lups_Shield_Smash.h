#ifndef Lups_Shield_Smash_h__
#define Lups_Shield_Smash_h__

#include "State.h"

_BEGIN(Engine)
class CTransform;
_END

_BEGIN(Client)
class CEffectGroup;
class CLups;
class CUI_Frame;
class CLups_Shield_Smash : public CState
{
private:
	explicit CLups_Shield_Smash();
	virtual ~CLups_Shield_Smash() = default;

public:

	// CState을(를) 통해 상속됨
	virtual HRESULT Begin() override;
	virtual _int Update(const _float & fTimeDelta) override;
	virtual _int LastUpdate(const _float& fTimeDelta) override;
	virtual HRESULT End() override;
	
private:
	HRESULT	 Ready_State(CLups* pLups);

private:
	CLups*			m_pLups = nullptr;
	
	CUI_Frame*		m_pUIFrame = nullptr;
	CEffectGroup*	m_pEffect_Shield_Smash = nullptr;

	DAMAGE m_Damage;
public:
	static CLups_Shield_Smash*	Create(CLups* pLups);

protected:
	virtual void Free() override;

};

_END


#endif // Lups_Shield_Smash_h__
