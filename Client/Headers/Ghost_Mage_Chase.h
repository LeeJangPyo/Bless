#ifndef Ghost_Mage_Chase_h__
#define Ghost_Mage_Chase_h__

#include "State.h"

_BEGIN(Engine)
class CTransform;
_END

_BEGIN(Client)
class CGhost_Mage;
class CGhost_Mage_Chase : public CState
{
private:
	explicit CGhost_Mage_Chase();
	virtual ~CGhost_Mage_Chase() = default;

public:

	// CState을(를) 통해 상속됨
	virtual HRESULT Begin() override;
	virtual _int Update(const _float & fTimeDelta) override;
	virtual	_int LastUpdate(const _float& fTimeDelta) override;
	virtual HRESULT End() override;
	
private:
	HRESULT	 Ready_State(CGhost_Mage* pGhost_Mage);

private:
	CGhost_Mage*		m_pGhost_Mage = nullptr;
	CTransform*			m_pTransform = nullptr;
	const _vec3*		m_pTarget_Position = nullptr;

public:
	static CGhost_Mage_Chase*	Create(CGhost_Mage* pGhost_Mage);
protected:
	virtual void Free() override;

};

_END


#endif // Ghost_Mage_Chase_h__
