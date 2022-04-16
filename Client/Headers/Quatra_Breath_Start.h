#ifndef Quatra_Breath_Start_h__
#define Quatra_Breath_Start_h__

#include "State.h"

_BEGIN(Engine)
class CTransform;
_END

_BEGIN(Client)
class CQuatra;
class CEffectGroup;
class CQuatra_Breath_Start : public CState
{
private:
	explicit CQuatra_Breath_Start();
	virtual ~CQuatra_Breath_Start() = default;

public:

	// CState을(를) 통해 상속됨
	virtual HRESULT Begin() override;
	virtual _int Update(const _float & fTimeDelta) override;
	virtual _int LastUpdate(const _float& fTimeDelta) override;
	virtual HRESULT End() override;
	
private:
	HRESULT	 Ready_State(CQuatra* pQuatra);

private:
	CQuatra*		m_pQuatra;
	CTransform*		m_pQuatraTransform;


public:
	static CQuatra_Breath_Start*	Create(CQuatra* pQuatra);
protected:
	virtual void Free() override;

};

_END


#endif // Ghost_Soldier_Breath_Start_h__
