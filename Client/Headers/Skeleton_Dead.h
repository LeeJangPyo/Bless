#ifndef Skeleton_Dead_h__
#define Skeleton_Dead_h__

#include "State.h"

_BEGIN(Client)
class CSkeleton;
class CSkeleton_Dead : public CState
{
private:
	explicit CSkeleton_Dead();
	virtual ~CSkeleton_Dead() = default;

public:

	// CState을(를) 통해 상속됨
	virtual HRESULT Begin() override;
	virtual _int Update(const _float & fTimeDelta) override;
	virtual _int LastUpdate(const _float& fTimeDelta) override;
	virtual HRESULT End() override;
	
private:
	HRESULT	 Ready_State(CSkeleton* pSkeleton);

private:
	CSkeleton*			m_pSkeleton;

	_float				m_fWaitTime;
	_float				m_fTimeAcc;
	_bool				m_bIsAnimationEnd;
public:
	static CSkeleton_Dead*	Create(CSkeleton* pSkeleton);
protected:
	virtual void Free() override;

};

_END


#endif // Ghost_Soldier_Wait_h__
