#ifndef Skeleton_Attack_h__
#define Skeleton_Attack_h__

#include "State.h"

_BEGIN(Engine)
class CTransform;
_END

_BEGIN(Client)
class CSkeleton;
class CSkeleton_Attack : public CState
{
private:
	explicit CSkeleton_Attack();
	virtual ~CSkeleton_Attack() = default;

public:

	// CState을(를) 통해 상속됨
	virtual HRESULT Begin() override;
	virtual _int Update(const _float & fTimeDelta) override;
	virtual _int LastUpdate(const _float& fTimeDelta) override;
	virtual HRESULT End() override;
	
private:
	HRESULT	 Ready_State(CSkeleton* pSkeleton);

private:
	CSkeleton*		m_pSkeleton = nullptr;
	CTransform*		m_pSkeletonTransform = nullptr;
	const DAMAGE*	m_pSkeleton_Damage = nullptr;
	const _vec3*	m_pTarget_Position = nullptr;

	_int		m_iAttackIndex;
public:
	static CSkeleton_Attack*	Create(CSkeleton* pSkeleton);
protected:
	virtual void Free() override;

};

_END


#endif // Ghost_Soldier_Wait_h__
