#ifndef Sire_Meteo_Strike_h__
#define Sire_Meteo_Strike_h__

#include "State.h"

_BEGIN(Engine)
class CTransform;
_END

_BEGIN(Client)
class CSire;
class CEffectGroup;
class CSire_Meteo_Strike : public CState
{
private:
	explicit CSire_Meteo_Strike();
	virtual ~CSire_Meteo_Strike() = default;

public:

	// CState을(를) 통해 상속됨
	virtual HRESULT Begin() override;
	virtual _int Update(const _float & fTimeDelta) override;
	virtual _int LastUpdate(const _float& fTimeDelta) override;
	virtual HRESULT End() override;
	
private:
	HRESULT	 Ready_State(CSire* pSire);
private:
	CSire*	m_pSire = nullptr;

public:
	static CSire_Meteo_Strike*	Create(CSire* pSire);
protected:
	virtual void Free() override;

};

_END


#endif // Sire_Meteo_Strike_h__
