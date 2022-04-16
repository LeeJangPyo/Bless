#ifndef Flogas_Dead_h__
#define Flogas_Dead_h__

#include "State.h"

_BEGIN(Client)
class CFlogas;
class CFlogas_Dead : public CState
{
private:
	explicit CFlogas_Dead();
	virtual ~CFlogas_Dead() = default;

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

	_float			m_fWaitTime;
	_float			m_fTimeAcc;
	_bool			m_bIsAnimationEnd;

public:
	static CFlogas_Dead*	Create(CFlogas* pFlogas);
protected:
	virtual void Free() override;

};

_END


#endif // Ghost_Soldier_Wait_h__
