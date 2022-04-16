#pragma once

#include "Defines.h"
#include "Base.h"
#include "Graphic_Device.h"

_BEGIN(Engine)
class CManagement;
class CInput_Device;
class CRenderer;
_END

_BEGIN(Client)
class CMainApp final : public CBase
{
private:
	explicit CMainApp();
	virtual ~CMainApp() = default;
public:
	HRESULT Ready_MainApp();
	_int	Update_MainApp(const _float& fTimeDelta);
	void	Render_MainApp();
private:

	LPDIRECT3DDEVICE9			m_pDevice = nullptr;
	CInput_Device*				m_pInput_Device = nullptr;
	CManagement*				m_pManagement = nullptr;
	CRenderer*					m_pRenderer = nullptr;

private:
	_tchar		m_szTmp[128] = L"";
	_int		m_iNumRenderCall = 0;
	_float		m_fTimeAcc = 0;
private:
	HRESULT Ready_Default_Setting(CGraphic_Device::WINMODE eMode, const _ushort& nSizeX, const _ushort& nSizeY);
	HRESULT Ready_Start_Scene(SCENEID eSceneID);
	HRESULT	Ready_Prototype_Component();		//For Static
	
public:
	static CMainApp* Create();
protected:
	virtual void Free();
};

_END