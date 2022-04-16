#ifndef Lups_General_h__
#define Lups_General_h__

#include "State.h"

_BEGIN(Engine)
class CTransform;
_END

_BEGIN(Client)
class CLups;
class CLups_General : public CState
{
private:
	explicit CLups_General();
	virtual ~CLups_General() = default;

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

public:
	static CLups_General*	Create(CLups* pLups);
protected:
	virtual void Free() override;

};

_END


#endif // Lups_General_h__
