#ifndef Ghost_Soldier_Wait_h__
#define Ghost_Soldier_Wait_h__

#include "State.h"

_BEGIN(Client)
class CGhost_Soldier;
class CGhost_Soldier_Wait : public CState
{
private:
	explicit CGhost_Soldier_Wait();
	virtual ~CGhost_Soldier_Wait() = default;

public:

	// CState을(를) 통해 상속됨
	virtual HRESULT Begin() override;
	virtual _int Update(const _float & fTimeDelta) override;
	virtual _int LastUpdate(const _float& fTimeDelta) override;
	virtual HRESULT End() override;
	
private:
	HRESULT	 Ready_State(CGhost_Soldier* pGhost_Soldier);

private:
	CGhost_Soldier*		m_pGhost_Solider;

public:
	static CGhost_Soldier_Wait*	Create(CGhost_Soldier* pGhost_Soldier);
protected:
	virtual void Free() override;

};

_END


#endif // Ghost_Soldier_Wait_h__
