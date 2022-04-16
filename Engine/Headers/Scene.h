#pragma once

#include "Base.h"
#include "Input_Device.h"
#include "Renderer.h"

_BEGIN(Engine)

class CObject_Manager;
class _ENGINE_DLL CScene abstract : public CBase
{
protected:		
	explicit CScene(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CScene() = default;
public:
	virtual HRESULT Ready_Scene();
	virtual _int	Update_Scene(const _float& fTimeDelta);
	virtual _int	LastUpdate_Scene(const _float& fTimeDelta);
	virtual _int	Update_Scene(const _uint& iSceneID, const _float& fTimeDelta);
	virtual _int	LastUpdate_Scene(const _uint& iSceneID, const _float& fTimeDelta);
	virtual void	Render_Scene();
protected:
	LPDIRECT3DDEVICE9		m_pDevice = nullptr;
	CInput_Device*			m_pInput_Device = nullptr;
	CObject_Manager*		m_pObject_Manager = nullptr;
	
protected:
	virtual void Free();
};

_END