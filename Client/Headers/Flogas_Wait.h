#ifndef Flogas_Wait_h__
#define Flogas_Wait_h__

#include "State.h"

_BEGIN(Client)
class CFlogas;
class CFlogas_Wait : public CState
{
private:
	explicit CFlogas_Wait();
	virtual ~CFlogas_Wait() = default;

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
	static CFlogas_Wait*	Create(CFlogas* pFlogas);
protected:
	virtual void Free() override;

};

_END


#endif // Ghost_Soldier_Wait_h__
