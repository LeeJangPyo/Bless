#ifndef Lups_AI_h__
#define Lups_AI_h__

#include "State.h"


_BEGIN(Engine)
class CTransform;
class CInput_Device;
_END

_BEGIN(Client)
class CLups;
class CLups_AI : public CState
{
private:
	explicit CLups_AI();
	virtual ~CLups_AI() = default;

public:

	// CState을(를) 통해 상속됨
	virtual HRESULT Begin() override;
	virtual _int Update(const _float & fTimeDelta) override;
	virtual _int LastUpdate(const _float& fTimeDelta) override;
	virtual HRESULT End() override;
	
private:
	HRESULT	 Ready_State(CLups* pLups);
private:
	CLups*			m_pLups;
	CTransform*		m_pLups_Transform;
	CInput_Device*	m_pInput_Device;
public:
	static CLups_AI*	Create(CLups* pLups);
protected:
	virtual void Free() override;

};

_END


#endif // Lups_AI_h__
