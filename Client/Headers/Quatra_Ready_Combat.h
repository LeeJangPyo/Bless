#ifndef Quatra_Ready_Combat_h__
#define Quatra_Ready_Combat_h__

#include "State.h"

_BEGIN(Client)
class CQuatra;
class CQuatra_Ready_Combat : public CState
{
private:
	explicit CQuatra_Ready_Combat();
	virtual ~CQuatra_Ready_Combat() = default;

public:

	// CState을(를) 통해 상속됨
	virtual HRESULT Begin() override;
	virtual _int Update(const _float & fTimeDelta) override;
	virtual _int LastUpdate(const _float& fTimeDelta) override;
	virtual HRESULT End() override;
	
private:
	HRESULT	 Ready_State(CQuatra* pQuatra);

private:
	CQuatra*		m_pQuatra = nullptr;

	_bool			m_bPlayOnce;
public:
	static CQuatra_Ready_Combat*	Create(CQuatra* pQuatra);
protected:
	virtual void Free() override;

};

_END


#endif // Ghost_Soldier_Wait_h__
