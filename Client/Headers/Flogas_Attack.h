#ifndef Flogas_Attack_h__
#define Flogas_Attack_h__

#include "State.h"

_BEGIN(Engine)
class CTransform;
_END

_BEGIN(Client)
class CFlogas;
class CFlogas_Attack : public CState
{
private:
	explicit CFlogas_Attack();
	virtual ~CFlogas_Attack() = default;

public:

	// CState을(를) 통해 상속됨
	virtual HRESULT Begin() override;
	virtual _int Update(const _float & fTimeDelta) override;
	virtual _int LastUpdate(const _float& fTimeDelta) override;
	virtual HRESULT End() override;
	
private:
	HRESULT	 Ready_State(CFlogas* pFlogas);

private:
	CFlogas*			m_pFlogas = nullptr;
	CTransform*			m_pFlogasTransform = nullptr;
	const DAMAGE*		m_pFlogas_Damage = nullptr;
	const _vec3*		m_pTarget_Position = nullptr;

	_int				m_iAttackIndex;
public:
	static CFlogas_Attack*	Create(CFlogas* pFlogas);
protected:
	virtual void Free() override;

};

_END


#endif // Ghost_Soldier_Wait_h__
