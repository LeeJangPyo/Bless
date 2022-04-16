#ifndef Skeleton_Chase_h__
#define Skeleton_Chase_h__

#include "State.h"

_BEGIN(Engine)
class CTransform;
_END

_BEGIN(Client)
class CSkeleton;
class CSkeleton_Chase : public CState
{
private:
	explicit CSkeleton_Chase();
	virtual ~CSkeleton_Chase() = default;

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
	const _vec3*	m_pTarget_Position = nullptr;


public:
	static CSkeleton_Chase*	Create(CSkeleton* pSkeleton);
protected:
	virtual void Free() override;

};

_END


#endif // Ghost_Soldier_Wait_h__
