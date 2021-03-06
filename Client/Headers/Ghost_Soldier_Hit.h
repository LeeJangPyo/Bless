#ifndef Ghost_Soldier_Hit_h__
#define Ghost_Soldier_Hit_h__

#include "State.h"

_BEGIN(Engine)
class CTransform;
_END

_BEGIN(Client)
class CGhost_Soldier;
class CGhost_Soldier_Hit : public CState
{
private:
	explicit CGhost_Soldier_Hit();
	virtual ~CGhost_Soldier_Hit() = default;

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
	CTransform*			m_pTransform;

public:
	static CGhost_Soldier_Hit*	Create(CGhost_Soldier* pGhost_Soldier);
protected:
	virtual void Free() override;

};

_END


#endif // Ghost_Soldier_Hit_h__
