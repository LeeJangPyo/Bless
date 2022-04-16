#pragma once

#include "Defines.h"
#include "Scene.h"

_BEGIN(Engine)
class CFont_Manager;
_END

_BEGIN(Client)
class CLoading;
class CScene_Logo final : public CScene
{
private:
	explicit CScene_Logo(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CScene_Logo() = default;
public:
	virtual HRESULT Ready_Scene() override;
	virtual _int	Update_Scene(const _float& fTimeDelta) override;
	virtual _int	LastUpdate_Scene(const _float& fTimeDelta) override;
	virtual void	Render_Scene() override;
private:
	HRESULT Ready_Prototype_GameObject();
	HRESULT Ready_Prototype_Component();
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);

	HRESULT	Ready_Camera();
private:
	CLoading*			m_pLoading = nullptr;
	CFont_Manager*		m_pFont_Manager = nullptr;

	_bool				m_bLogoAddOnce;
public:
	static CScene_Logo* Create(LPDIRECT3DDEVICE9 pDevice);
protected:
	virtual void Free();
};

_END