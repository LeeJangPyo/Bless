#ifndef Skeleton_Idle_h__
#define Skeleton_Idle_h__

#include "State.h"

_BEGIN(Engine)
class CTransform;
_END

_BEGIN(Client)
class CSkeleton;
class CSkeleton_Idle : public CState
{
private:
	explicit CSkeleton_Idle();
	virtual ~CSkeleton_Idle() = default;

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

	CTransform*		m_pPlayerTransform = nullptr;
	CTransform*		m_pSkeletonTransform =nullptr;

	_float			m_fIdelTime;

public:
	static CSkeleton_Idle*	Create(CSkeleton* pSkeleton);
protected:
	virtual void Free() override;

};

_END


#endif // Ghost_Soldier_Wait_h__
