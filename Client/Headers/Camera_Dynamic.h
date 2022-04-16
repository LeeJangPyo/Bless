#ifndef Camera_Dynamic_h__
#define Camera_Dynamic_h__

#include "Defines.h"

#include "Camera.h"


_BEGIN(Engine)
class CInput_Device;
class CTransform;
_END

_BEGIN(Client)

class CCamera_Dynamic final : public CCamera
{
public:
	explicit CCamera_Dynamic(LPDIRECT3DDEVICE9 pDevice);
	explicit CCamera_Dynamic(const CCamera_Dynamic& rhs);
	virtual	~CCamera_Dynamic() = default;


public:
	virtual HRESULT Ready_Prototype()override;
	virtual	HRESULT Ready_GameObject()override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

public:
	HRESULT SetUp_CameraInfo(CAMERADESC CamDesc, PROJDESC ProjDesc);
private:
	void	Check_InputKey(const _float& fTimeDelta);

private:
	_float	m_fCamSpeed;

public:
	virtual CGameObject* Clone_GameObject() override;
	static CCamera_Dynamic* Create(LPDIRECT3DDEVICE9 pDevice);

protected:
	virtual void Free() override;

};

_END
#endif // Camera_Dynamic_h__