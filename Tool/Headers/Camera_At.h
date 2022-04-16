
#ifndef Camera_At_h__
#define Camera_At_h__

#include "Defines.h"
#include "Base.h"

_BEGIN(Engine)
class CCalculator;
class CCollider_Box;
class CInput_Device;
_END

class CCameraTab;

_BEGIN(Client)
class CCamera_At final : public CBase
{
	explicit CCamera_At(LPDIRECT3DDEVICE9	pDevice);
	virtual~CCamera_At() = default;

public:
	//set
	void	Set_IsPick(_bool bIsPick) { m_bIsPick = bIsPick; }
	//get
	const _vec3*	Get_Position();

	void	Set_CameraTab(CCameraTab* pCameraTab);
public:
	HRESULT	Ready_At(const _vec3& vPosition);
	_int	Update(const _float& fTimeDelta);
	void	Render();

	HRESULT	SetUp_ConstantTable(LPD3DXEFFECT pEffect);
private:
	HRESULT	Ready_Component();
	void	Check_KeyInput(const _float& fTimeDelta);

private:
	_vec3				m_vPosition;
	_matrix				m_matWorld;

	CCameraTab*			m_pCameraTab;
	CInput_Device*		m_pInput_Device;
	CCalculator*		m_pCalculatorCom;
	CCollider_Box*		m_pColliderCom;
	LPDIRECT3DDEVICE9	m_pDevice;

	_bool				m_bIsPick;
	_bool				m_bIsMouseDown;
public:
	static CCamera_At*	Create(LPDIRECT3DDEVICE9 pDevice, const _vec3& vPosition);

protected:
	virtual void Free() override;

};

_END

#endif // Camera_At_h__