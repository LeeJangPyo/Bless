#ifndef Flogas_Chase_h__
#define Flogas_Chase_h__

#include "State.h"

_BEGIN(Engine)
class CTransform;
_END

_BEGIN(Client)
class CFlogas;
class CFlogas_Chase : public CState
{
private:
	explicit CFlogas_Chase();
	virtual ~CFlogas_Chase() = default;

public:

	// CState을(를) 통해 상속됨
	virtual HRESULT Begin() override;
	virtual _int Update(const _float & fTimeDelta) override;
	virtual _int LastUpdate(const _float& fTimeDelta) override;
	virtual HRESULT End() override;
	
private:
	HRESULT	 Ready_State(CFlogas* pFlogas);

private:
	CFlogas*		m_pFlogas = nullptr;

	CTransform*		m_pFlogasTransform = nullptr;
	const _vec3*	m_pTarget_Position = nullptr;

public:
	static CFlogas_Chase*	Create(CFlogas* pFlogas);
protected:
	virtual void Free() override;

};

_END


#endif // Ghost_Soldier_Wait_h__
