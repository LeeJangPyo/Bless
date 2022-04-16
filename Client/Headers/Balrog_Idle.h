#ifndef Balrog_Idle_h__
#define Balrog_Idle_h__

#include "State.h"

_BEGIN(Engine)
class CTransform;
_END

_BEGIN(Client)
class CBalrog;
class CBalrog_Idle : public CState
{
private:
	explicit CBalrog_Idle();
	virtual ~CBalrog_Idle() = default;

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
	CTransform*			m_pPlayerTransform;
	CTransform*			m_pTransform;

	_int			m_iAnimationIndex;
public:
	static CBalrog_Idle*	Create(CBalrog* pBalrog);
protected:
	virtual void Free() override;

};

_END


#endif // Balrog_Idle_h__
