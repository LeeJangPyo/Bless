#ifndef Loading_h__
#define Loading_h__

#include "Base.h"
#include "Defines.h"

_BEGIN(Engine)
class CGameObject;
_END

_BEGIN(Client)

class CLoading final : public CBase
{
public:
	enum LOADINGID
	{
		LOADING_STAGE, LOADING_BOSS, LOADING_END
	};

public:
	explicit CLoading(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CLoading() = default;


public:
	LOADINGID			GetLoadingID(void) const { return m_eLoading; }
	CRITICAL_SECTION*	Get_CritialSection(void) { return &m_CrticalSection; }
	_bool				Get_IsFinish(void) const { return m_bIsFinish; }
	const _tchar*		Get_String(void) const { return m_szLoadingString; }
public:
	HRESULT				Ready_Loading(LOADINGID eLoadingID);
	
	//Stage
	_uint				Loading_ForStage(void);
	HRESULT				Loading_StageInfoFormFile();
	HRESULT				Loading_EffectFile();


private:
	HRESULT				Ready_Stage_Prototype_Component();
	HRESULT				Ready_Stage_Prototype_GameObject();
	HRESULT				Ready_Stage_GameObject_Layer();

	HRESULT				Ready_Layer_Terrain(const _tchar* pLayerTag);
	HRESULT				Ready_Layer_SkyBox(const _tchar* pLayerTag);
	HRESULT				Ready_Layer_Character();
	HRESULT				Ready_Layer_Weapon();
	HRESULT				Ready_Layer_Etc();


	
	HRESULT		Create_Prototype_Component_Mesh(OBJECTTYPE eType, const _tchar* pFilePath, const _tchar* pFileName, const _tchar* pComponentMeshTag);
	HRESULT		Create_Prototype_GameObject(OBJECTTYPE eType, const _tchar* pGameObjectTag, CGameObject** ppGameObject = nullptr);
	HRESULT		Add_Layer_GameObject(const _tchar* pLayerTag, const _tchar* pGameObjectTag, CGameObject** ppGameObject = nullptr);


	HRESULT		Load_GameObject_Static_FromFile(const _tchar* pFilePath);
	HRESULT		Load_GameObject_Dynamic_FromFile(const _tchar* pFilePath);
	HRESULT		Load_Camera_CutScene_FromFile(const _tchar* pFilePath);
	HRESULT		Load_UI_FromFile(const _tchar* pFilePath, const _tchar* pLayerTag);
	HRESULT		Load_Effect_FromFile(const _tchar* pFilePath, const _tchar* pLayerTag);
private:
	LPDIRECT3DDEVICE9	m_pDevice;
	LOADINGID			m_eLoading;
	HANDLE				m_hThread;
	CRITICAL_SECTION	m_CrticalSection;
	_bool				m_bIsFinish;


	_tchar				m_szLoadingString[MIN_PATH];

public:
	static	size_t	CALLBACK	Thread_Main(void *pArg);

public:
	static CLoading*	Create(LPDIRECT3DDEVICE9 pDevice, LOADINGID	eLoadingID);
private:
	virtual void		Free(void);
};

_END

#endif // Loading_h__