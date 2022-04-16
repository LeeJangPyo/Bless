#ifndef Ghost_Soldier_Chase_h__
#define Ghost_Soldier_Chase_h__

#include "State.h"

_BEGIN(Engine)
class CTransform;
_END

_BEGIN(Client)
class CGhost_Soldier;
class CGhost_Soldier_Chase : public CState
{
private:
	explicit CGhost_Soldier_Chase();
	virtual ~CGhost_Soldier_Chase() = default;

public:

	// CState을(를) 통해 상속됨
	virtual HRESULT Begin() override;
	virtual _int Update(const _float & fTimeDelta) override;
	virtual	_int LastUpdate(const _float& fTimeDelta) override;
	virtual HRESULT End() override;
	
private:
	HRESULT	 Ready_State(CGhost_Soldier* pGhost_Soldier);

private:

	CGhost_Soldier*		m_pGhost_Solider = nullptr;
	CTransform*			m_pTransform = nullptr;
	const _vec3*		m_pTarget_Position = nullptr;

public:
	static CGhost_Soldier_Chase*	Create(CGhost_Soldier* pGhost_Soldier);
protected:
	virtual void Free() override;

};

_END


#endif // Ghost_Soldier_Chase_h__
