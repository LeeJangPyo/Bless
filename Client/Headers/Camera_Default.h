#ifndef Camera_Default_h__
#define Camera_Default_h__

#include "Defines.h"

#include "Camera.h"


_BEGIN(Engine)
class CInput_Device;
class CTransform;
_END

_BEGIN(Client)

class CCamera_Default final : public CCamera
{
public:
	explicit CCamera_Default(LPDIRECT3DDEVICE9 pDevice);
	explicit CCamera_Default(const CCamera_Default& rhs);
	virtual	~CCamera_Default() = default;


public:
	virtual HRESULT Ready_Prototype()override;
	virtual	HRESULT Ready_GameObject()override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

public:
	HRESULT SetUp_CameraInfo(CAMERADESC CamDesc, PROJDESC ProjDesc);

public:
	virtual CGameObject* Clone_GameObject() override;
	static CCamera_Default* Create(LPDIRECT3DDEVICE9 pDevice);

protected:
	virtual void Free() override;

};

_END
#endif // Camera_Default_h__