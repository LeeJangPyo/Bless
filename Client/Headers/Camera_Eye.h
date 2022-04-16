#ifndef Camera_Eye_h__
#define Camera_Eye_h__

#include "Defines.h"
#include "Base.h"


_BEGIN(Engine)
class CCalculator;
class CCollider_Sphere;
class CInput_Device;
_END


_BEGIN(Client)

class CCamera_Eye final : public CBase
{
public:
	explicit CCamera_Eye(LPDIRECT3DDEVICE9	pDevice);
	virtual~ CCamera_Eye() = default;


public:
	//set
	void	Set_IsPick(_bool bIsPick) { m_bIsPick = bIsPick; }
	//get
	const _vec3*	Get_Position();
public:
	HRESULT	Ready_Eye(const _vec3& vPosition);
	_int	Update(const _float& fTimeDelta);
	void	Render();

	HRESULT	SetUp_ConstantTable(LPD3DXEFFECT pEffect);
private:
	HRESULT	Ready_Component();

private:
	_vec3				m_vPosition;
	_matrix				m_matWorld;
		
	
	CInput_Device*		m_pInput_Device;
	CCalculator*		m_pCalculatorCom;
	CCollider_Sphere*	m_pColliderCom;
	LPDIRECT3DDEVICE9	m_pDevice;

	_bool				m_bIsPick;
	_bool				m_bIsMouseDown;
public:
	static CCamera_Eye*	Create(LPDIRECT3DDEVICE9 pDevice, const _vec3& vPosition);

protected:
	virtual void Free() override;

	

};

_END

#endif // Camera_Eye_h__