#ifndef Camera_h__
#define Camera_h__

#include "GameObject.h"
#include "Input_Device.h"

_BEGIN(Engine)

class _ENGINE_DLL CCamera abstract : public CGameObject
{
protected:
	explicit	CCamera(LPDIRECT3DDEVICE9 pDevice);
	explicit	CCamera(const CCamera& rhs);
	virtual		~CCamera(void) = default;

public:
	void Set_CameraDesc(const CAMERADESC& Camera_Desc) {m_Camera_Desc = Camera_Desc;}
	void Set_ProjectionDesc(const PROJDESC& Projection_Desc) {m_Projection_Desc = Projection_Desc;}
	void Set_Fovy(const _float& fFovy);
	_matrix Get_BillBoardMatrix();


	HRESULT	SetUp_OnShader_FarPlane(LPD3DXEFFECT pEffect, const char* pConstantName);
	HRESULT SetUp_OnShader_View(LPD3DXEFFECT pEffect, const char* pConstantName);
	HRESULT SetUp_OnShader_Projection(LPD3DXEFFECT pEffect, const char* pConstantName);
	HRESULT	SetUp_Matrix();
	HRESULT	SetUp_OnShader_WorldCameraPos(LPD3DXEFFECT pEffect, const char* pConstantName);

	const _vec3*	Get_Camera_Position() {
		return &m_Camera_Desc.vEye;	}
public:
	virtual HRESULT Ready_Prototype() override;
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;
	
protected:
	CAMERADESC			m_Camera_Desc;
	PROJDESC			m_Projection_Desc;
	_matrix				m_matView, m_matProj, m_matWorld;

	CInput_Device*		m_pInput_Device = nullptr;
public:
	virtual CGameObject* Clone_GameObject() = 0;
protected:
	virtual void Free() override;

};

_END
#endif // Camera_h__