#ifndef Lups_DownAndUp_h__
#define Lups_DownAndUp_h__

#include "State.h"

_BEGIN(Engine)
class CTransform;
_END

_BEGIN(Client)
class CLups;
class CLups_DownAndUp : public CState
{
private:
	explicit CLups_DownAndUp();
	virtual ~CLups_DownAndUp() = default;

public:

	// CState을(를) 통해 상속됨
	virtual HRESULT Begin() override;
	virtual _int Update(const _float & fTimeDelta) override;
	virtual _int LastUpdate(const _float& fTimeDelta) override;
	virtual HRESULT End() override;
	
private:
	HRESULT	 Ready_State(CLups* pLups);

private:
	CLups*				m_pLups = nullptr;

	_int				m_iAnimationIndex;

public:
	static CLups_DownAndUp*	Create(CLups* pLups);
protected:
	virtual void Free() override;

};

_END


#endif // Lups_DownAndUp_h__
