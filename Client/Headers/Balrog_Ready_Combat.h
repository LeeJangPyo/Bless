#ifndef Balrog_Ready_Combat_h__
#define Balrog_Ready_Combat_h__

#include "State.h"

_BEGIN(Client)
class CBalrog;
class CBalrog_Ready_Combat : public CState
{
private:
	explicit CBalrog_Ready_Combat();
	virtual ~CBalrog_Ready_Combat() = default;

public:

	// CState을(를) 통해 상속됨
	virtual HRESULT Begin() override;
	virtual _int Update(const _float & fTimeDelta) override;
	virtual _int LastUpdate(const _float& fTimeDelta) override;
	virtual HRESULT End() override;
	
private:
	HRESULT	 Ready_State(CBalrog* pBalrog);

private:
	CBalrog*		m_pBalrog;

public:
	static CBalrog_Ready_Combat*	Create(CBalrog* pBalrog);
protected:
	virtual void Free() override;

};

_END


#endif // Ghost_Soldier_Wait_h__
