#ifndef Balrog_Wait_h__
#define Balrog_Wait_h__

#include "State.h"

_BEGIN(Client)
class CBalrog;
class CBalrog_Wait : public CState
{
private:
	explicit CBalrog_Wait();
	virtual ~CBalrog_Wait() = default;

public:

	// CState을(를) 통해 상속됨
	virtual HRESULT Begin() override;
	virtual _int Update(const _float & fTimeDelta) override;
	virtual _int LastUpdate(const _float& fTimeDelta) override;
	virtual HRESULT End() override;
	
private:
	HRESULT	 Ready_State(CBalrog* pBarlog);

private:
	CBalrog*		m_pBarlog;

public:
	static CBalrog_Wait*	Create(CBalrog* pBarlog);
protected:
	virtual void Free() override;

};

_END


#endif // Balrog_Wait_h__
