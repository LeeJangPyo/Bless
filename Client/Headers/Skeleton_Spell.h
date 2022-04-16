#ifndef Skeleton_Spell_h__
#define Skeleton_Spell_h__

#include "State.h"

_BEGIN(Engine)
class CTransform;
_END

_BEGIN(Client)
class CSkeleton;
class CSkeleton_Spell : public CState
{
private:
	explicit CSkeleton_Spell();
	virtual ~CSkeleton_Spell() = default;

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
	CTransform*		m_pLups_Transfrom = nullptr;
	CTransform*		m_pSire_Transfrom = nullptr;

	_float			m_fSpellAccTime;
	_float			m_fTimeAcc;

	DAMAGE			m_Damage;
public:
	static CSkeleton_Spell*	Create(CSkeleton* pSkeleton);
protected:
	virtual void Free() override;

};

_END


#endif // Ghost_Soldier_Wait_h__
