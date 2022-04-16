#ifndef Ghost_Soldier_Idle_h__
#define Ghost_Soldier_Idle_h__

#include "State.h"

_BEGIN(Engine)
class CTransform;
_END

_BEGIN(Client)
class CGhost_Soldier;
class CGhost_Soldier_Idle : public CState
{
private:
	explicit CGhost_Soldier_Idle();
	virtual ~CGhost_Soldier_Idle() = default;

public:

	// CState을(를) 통해 상속됨
	virtual HRESULT Begin() override;
	virtual _int Update(const _float & fTimeDelta) override;
	virtual _int LastUpdate(const _float& fTimeDelta) override;
	virtual HRESULT End() override;
	
private:
	HRESULT	 Ready_State(CGhost_Soldier* pGhost_Soldier);

private:
	CGhost_Soldier*		m_pGhost_Solider = nullptr;
	CTransform*			m_pPlayerTransform = nullptr;
	CTransform*			m_pTransform = nullptr; 

	_float				m_fIdelTime;


public:
	static CGhost_Soldier_Idle*	Create(CGhost_Soldier* pGhost_Soldier);
protected:
	virtual void Free() override;

};

_END


#endif // Ghost_Soldier_Idle_h__
