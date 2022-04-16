#ifndef Scene_Stage_h__
#define Scene_Stage_h__

#include "Defines.h"
#include "Scene.h"

_BEGIN(Client)

class CScene_Stage final : public CScene
{
private:
	explicit CScene_Stage(LPDIRECT3DDEVICE9 pDeivce);
	virtual ~CScene_Stage() = default;

public:
	virtual HRESULT Ready_Scene() override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual _int LastUpdate_Scene(const _float& fTimeDelta) override;
	virtual void Render_Scene() override;

private:
	HRESULT		Ready_LightInfo();
	HRESULT		Ready_GameEvent();
	HRESULT		Ready_Camera();
public:
	static CScene_Stage* Create(LPDIRECT3DDEVICE9 pDevice);


protected:
	virtual void Free() override;
};


_END
#endif // Scene_Stage_h__