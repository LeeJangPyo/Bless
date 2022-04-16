#ifndef MainApp_h__
#define MainApp_h__

#include "Defines.h"

#include "Base.h"
#include "Graphic_Device.h"
#include "MainFrm.h"

_BEGIN(Engine)
class CManagement;
class CInput_Device;
class CRenderer;
_END

_BEGIN(Client)
class CMainApp final : public CBase
{
private:
	explicit	CMainApp(void);
	virtual		~CMainApp(void) = default;


public:
	HRESULT Ready_MainApp();
	_int	Update_MainApp(const _float& fTimeDelta);
	void	Render_MainApp();

private:
	HRESULT Ready_Default_Setting(CGraphic_Device::WINMODE eMode, const _ushort& nSizeX, const _ushort& nSizeY);
	HRESULT Ready_Start_Scene(SCENEID eSceneID);
	HRESULT	Ready_Prototype_Component();		//For Static
	HRESULT Ready_Prototype_GameObject();
	HRESULT	Ready_GameObject_Layer();

private:
	CManagement*			m_pManagement;
	CInput_Device*			m_pInput_Device;
	CRenderer*				m_pRenderer;
	LPDIRECT3DDEVICE9		m_pDevice;

private:
	_tchar		m_szTmp[128] = L"";
	_int		m_iNumRenderCall = 0;
	_float		m_fTimeAcc = 0;

public:
	static CMainApp*		Create(void);

private:
	virtual		void		Free(void);

};
_END


#endif // MainApp_h__
