#ifndef Lups_Guard_h__
#define Lups_Guard_h__

#include "State.h"

_BEGIN(Engine)
class CTransform;
class CInput_Device;
_END

_BEGIN(Client)
class CEffectGroup;
class CLups;
class CUI_Frame;
class CLups_Guard : public CState
{
private:
	explicit CLups_Guard();
	virtual ~CLups_Guard() = default;

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

	CEffectGroup*	m_pEffect_Guard = nullptr;

public:
	static CLups_Guard*	Create(CLups* pLups);
protected:
	virtual void Free() override;

};

_END


#endif // Lups_Guard_h__
