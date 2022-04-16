#ifndef Quatra_Breath_h__
#define Quatra_Breath_h__

#include "State.h"

_BEGIN(Engine)
class CTransform;
_END

_BEGIN(Client)
class CQuatra;
class CEffect_QuatraBreath;
class CQuatra_Breath : public CState
{
private:
	explicit CQuatra_Breath();
	virtual ~CQuatra_Breath() = default;

public:

	// CState을(를) 통해 상속됨
	virtual HRESULT Begin() override;
	virtual _int Update(const _float & fTimeDelta) override;
	virtual _int LastUpdate(const _float& fTimeDelta) override;
	virtual HRESULT End() override;
	
private:
	HRESULT	 Ready_State(CQuatra* pQuatra);

private:
	CQuatra*		m_pQuatra = nullptr;

	_float			m_fTimeAcc;

	CTransform*		m_pQuatra_Transform = nullptr;

	CEffect_QuatraBreath*	m_pEffect_Breath = nullptr;

public:
	static CQuatra_Breath*	Create(CQuatra* pQuatra);
protected:
	virtual void Free() override;

};

_END


#endif // Ghost_Soldier_Breath_h__
