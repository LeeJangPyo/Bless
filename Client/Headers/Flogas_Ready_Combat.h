#ifndef Flogas_Ready_Combat_h__
#define Flogas_Ready_Combat_h__

#include "State.h"

_BEGIN(Client)
class CFlogas;
class CFlogas_Ready_Combat : public CState
{
private:
	explicit CFlogas_Ready_Combat();
	virtual ~CFlogas_Ready_Combat() = default;

public:

	// CState을(를) 통해 상속됨
	virtual HRESULT Begin() override;
	virtual _int Update(const _float & fTimeDelta) override;
	virtual _int LastUpdate(const _float& fTimeDelta) override;
	virtual HRESULT End() override;
	
private:
	HRESULT	 Ready_State(CFlogas* pFlogas);

private:
	CFlogas*		m_pFlogas;

public:
	static CFlogas_Ready_Combat*	Create(CFlogas* pFlogas);
protected:
	virtual void Free() override;

};

_END


#endif // Ghost_Soldier_Wait_h__
