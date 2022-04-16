#ifndef Ghost_Soldier_Attack_h__
#define Ghost_Soldier_Attack_h__

#include "State.h"

_BEGIN(Engine)
class CTransform;
_END

_BEGIN(Client)
class CGhost_Soldier;
class CGhost_Soldier_Attack : public CState
{
private:
	explicit CGhost_Soldier_Attack();
	virtual ~CGhost_Soldier_Attack() = default;

public:

	// CState을(를) 통해 상속됨
	virtual HRESULT Begin() override;
	virtual _int Update(const _float & fTimeDelta) override;
	virtual _int LastUpdate(const _float& fTimeDelta) override;
	virtual HRESULT End() override;
	
private:
	HRESULT	 Ready_State(CGhost_Soldier* pGhost_Soldier);

private:
//	_float				m_fColliderRadius;
	CGhost_Soldier*		m_pGhost_Solider = nullptr;
	CTransform*			m_pTransform = nullptr;
	

	const _vec3*		m_pTarget_Position = nullptr;

	const DAMAGE*		m_pGhost_Solider_Damage = nullptr; 

	_int				m_iAttackIndex;

public:
	static CGhost_Soldier_Attack*	Create(CGhost_Soldier* pGhost_Soldier);
protected:
	virtual void Free() override;

};

_END


#endif // Ghost_Soldier_Attack_h__
