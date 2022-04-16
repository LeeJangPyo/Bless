#ifndef Skeleton_Stab_h__
#define Skeleton_Stab_h__

#include "State.h"

_BEGIN(Engine)
class CTransform;
_END

_BEGIN(Client)
class CSkeleton;
class CSkeleton_Stab : public CState
{
private:
	explicit CSkeleton_Stab();
	virtual ~CSkeleton_Stab() = default;

public:

	// CState을(를) 통해 상속됨
	virtual HRESULT Begin() override;
	virtual _int Update(const _float & fTimeDelta) override;
	virtual _int LastUpdate(const _float& fTimeDelta) override;
	virtual HRESULT End() override;
	
private:
	HRESULT	 Ready_State(CSkeleton* pSkeleton);

private:
	CSkeleton*		m_pSkeleton;
	CTransform*			m_pPlayerTransform;
	CTransform*			m_pSkeletonTransform;
	const DAMAGE*	m_pSkeleton_Damage = nullptr;
public:
	static CSkeleton_Stab*	Create(CSkeleton* pSkeleton);
protected:
	virtual void Free() override;

};

_END


#endif // Ghost_Soldier_Wait_h__
