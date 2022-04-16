#pragma once

#include "Defines.h"
#include "Scene.h"

_BEGIN(Client)

class CScene_Logo final : public CScene
{
private:
	explicit CScene_Logo(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CScene_Logo() = default;
public:
	virtual HRESULT Ready_Scene() override;
	virtual _int	Update_Scene(const _float& fTimeDelta) override;
	virtual _int	LastUpdate_Scene(const _float& fTimeDelta) override;
	virtual void	Render_Scene() override;
private:
	HRESULT Ready_Prototype_GameObject();
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
public:
	static CScene_Logo* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
protected:
	virtual void Free();
};

_END