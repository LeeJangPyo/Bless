#ifndef Quatra_Wait_h__
#define Quatra_Wait_h__

#include "State.h"

_BEGIN(Client)
class CQuatra;
class CQuatra_Wait : public CState
{
private:
	explicit CQuatra_Wait();
	virtual ~CQuatra_Wait() = default;

public:

	// CState을(를) 통해 상속됨
	virtual HRESULT Begin() override;
	virtual _int Update(const _float & fTimeDelta) override;
	virtual _int LastUpdate(const _float& fTimeDelta) override;
	virtual HRESULT End() override;
	
private:
	HRESULT	 Ready_State(CQuatra* pQuatra);

private:
	CQuatra*		m_pQuatra;

public:
	static CQuatra_Wait*	Create(CQuatra* pQuatra);
protected:
	virtual void Free() override;

};

_END


#endif // Ghost_Soldier_Wait_h__
