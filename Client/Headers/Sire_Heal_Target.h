#ifndef Sire_Heal_Target_h__
#define Sire_Heal_Target_h__

#include "State.h"


_BEGIN(Engine)
class CTransform;
class CInput_Device;
class CCalculator;

_END

_BEGIN(Client)
class CUI_Frame;
class CSire;
class CEffectGroup;
class CSire_Heal_Target : public CState
{
private:
	explicit CSire_Heal_Target();
	virtual ~CSire_Heal_Target() = default;

public:

	// CState을(를) 통해 상속됨
	virtual HRESULT Begin() override;
	virtual _int Update(const _float & fTimeDelta) override;
	virtual _int LastUpdate(const _float& fTimeDelta) override;
	virtual HRESULT End() override;


private:
	void	 Check_KeyInput(const _float& fTimeDelta);
private:
	HRESULT	 Ready_State(CSire* pSire);
	_vec3	 PickUp_OnTerrain();

private:
	CSire*			m_pSire = nullptr;
	CInput_Device*	m_pInput_Device = nullptr;
	CCalculator*	m_pCalculator = nullptr;
	CUI_Frame*		m_pUIFrame = nullptr;

	CEffectGroup*	m_pEffect_TargetZone = nullptr;

public:
	static CSire_Heal_Target*	Create(CSire* pSire);
protected:
	virtual void Free() override;

};

_END


#endif // Sire_Spell_Target_h__
