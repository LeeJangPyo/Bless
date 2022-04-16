#ifndef Ghost_Mage_Hit_h__
#define Ghost_Mage_Hit_h__

#include "State.h"

_BEGIN(Engine)
class CTransform;
_END

_BEGIN(Client)
class CGhost_Mage;
class CGhost_Mage_Hit : public CState
{
private:
	explicit CGhost_Mage_Hit();
	virtual ~CGhost_Mage_Hit() = default;

public:

	// CState을(를) 통해 상속됨
	virtual HRESULT Begin() override;
	virtual _int Update(const _float & fTimeDelta) override;
	virtual _int LastUpdate(const _float& fTimeDelta) override;
	virtual HRESULT End() override;
	
private:
	HRESULT	 Ready_State(CGhost_Mage* pGhost_Mage);

private:
	CGhost_Mage*		m_pGhost_Mage;
	CTransform*			m_pTransform;

public:
	static CGhost_Mage_Hit*	Create(CGhost_Mage* pGhost_Mage);
protected:
	virtual void Free() override;

};

_END


#endif // Ghost_Mage_Hit_h__
