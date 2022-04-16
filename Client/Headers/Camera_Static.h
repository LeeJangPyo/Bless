#ifndef Camera_Static_h__
#define Camera_Static_h__

#include "Defines.h"
#include "Camera.h"

_BEGIN(Engine)
class CTransform;
class CCalculator;
_END

_BEGIN(Client)

class CCamera_Static final : public CCamera
{
public:
	explicit CCamera_Static(LPDIRECT3DDEVICE9 pDevice);
	explicit CCamera_Static(const CCamera_Static& rhs);
	virtual ~CCamera_Static() = default;

public:
	virtual HRESULT Ready_Prototype() override;
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

public:
	HRESULT SetUp_CameraInfo(CAMERADESC CamDesc, PROJDESC ProjDesc);
	void	Set_CameraShaking(const _float& fShakeTime);

private:
	HRESULT	Ready_Component();
	void	Check_KeyInput(const _float& fTimeDelta);
private:
	CCalculator*	m_pCalculatorCom;


	_vec3			m_vPrevTarget_Pos;
	_vec3			m_vTarget_Pos;

	//
	_float			m_fTimeAcc;
	_float			m_fShakingTime;

	_bool			m_bIsRButtonDown;
	_bool			m_bIsShakeing;
public:
	virtual CGameObject* Clone_GameObject() override;
	static	CCamera_Static* Create(LPDIRECT3DDEVICE9 pDevice);
protected:
	virtual void Free() override;

};
_END

#endif // Camera_Static_h__
