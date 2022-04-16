#ifndef Flogas_Idle_h__
#define Flogas_Idle_h__

#include "State.h"

_BEGIN(Engine)
class CTransform;
_END

_BEGIN(Client)
class CFlogas;
class CFlogas_Idle : public CState
{
private:
	explicit CFlogas_Idle();
	virtual ~CFlogas_Idle() = default;

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

	CTransform*			m_pPlayerTransform;
	CTransform*			m_pFlogasTransform;


	_float			m_fIdleTime;

public:
	static CFlogas_Idle*	Create(CFlogas* pFlogas);
protected:
	virtual void Free() override;

};

_END


#endif // Ghost_Soldier_Wait_h__
