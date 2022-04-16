#ifndef Skeleton_Ready_Combat_h__
#define Skeleton_Ready_Combat_h__

#include "State.h"

_BEGIN(Client)
class CSkeleton;
class CSkeleton_Ready_Combat : public CState
{
private:
	explicit CSkeleton_Ready_Combat();
	virtual ~CSkeleton_Ready_Combat() = default;

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


	_bool			m_bPlayOnce;
public:
	static CSkeleton_Ready_Combat*	Create(CSkeleton* pSkeleton);
protected:
	virtual void Free() override;

};

_END


#endif // Ghost_Soldier_Wait_h__
