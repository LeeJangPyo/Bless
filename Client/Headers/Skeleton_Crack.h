#ifndef Skeleton_Crack_h__
#define Skeleton_Crack_h__

#include "State.h"

_BEGIN(Client)
class CSkeleton;
class CEffectGroup;
class CCamera_Static;
class CSkeleton_Crack : public CState
{
private:
	explicit CSkeleton_Crack();
	virtual ~CSkeleton_Crack() = default;

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


	DAMAGE			m_Damage;
	CEffectGroup*	m_pEffect_Crack = nullptr;
	
	CCamera_Static* m_pCamera_Static = nullptr;
	_bool			m_bPlayOnce = false;
public:
	static CSkeleton_Crack*	Create(CSkeleton* pSkeleton);
protected:
	virtual void Free() override;

};

_END


#endif // Ghost_Soldier_Wait_h__
