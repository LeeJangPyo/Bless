#pragma once

#include "Base.h"

#include "Object_Manager.h"
#include "Component_Manager.h"
#include "Font_Manager.h"
#include "Input_Device.h"
#include "Line_Manager.h"
#include "Camera_Manager.h"
#include "Linear_Interpolation.h"

_BEGIN(Engine)

class CScene;
class CGameObject;
class CComponent;
class CCollider_Sphere;
class _ENGINE_DLL CManagement final : public CBase
{
	_DECLARE_SINGLETON(CManagement)
private:
	explicit CManagement();
	virtual ~CManagement() = default;

public:
	CGameObject*	Get_GameObject(const _uint& iSceneID, const _tchar* pLayerTag, const _uint& iIndex = 0);
	CComponent*		Get_Component(const _uint& iSceneID, const _tchar* pLayerTag, const _tchar* pComponentTag, const _uint& iIndex = 0);
	const	list<CGameObject*>*	Get_Layer_ObjectList(const _uint& iSceneID, const _tchar* pLayerTag);

public:
	HRESULT SetUp_ScenePointer(CScene* pScene);
	HRESULT Add_Prototype_Camera(const _tchar* pProtoTag, CCamera* pCamera);
	HRESULT	Add_Camera(const _uint& iCameraType, const _tchar* pProtoTag, const _tchar* pCameraTag, CCamera** ppCamera = nullptr);
	HRESULT Add_Prototype_GameObject(const _tchar* pProtoTag, CGameObject* pGameObject, CGameObject** ppGameObject = nullptr);
	HRESULT	Add_Prototype_Component(const _uint & iSceneID,const _tchar* pProtoTag, CComponent* pComponent);
	HRESULT Add_Layer(const _uint& iSceneID, const _tchar* pLayerTag, const _tchar* pProtoTag, CGameObject** ppGameObject = nullptr);
	HRESULT Clear_Scene_Container(const _uint& iSceneID);

	//Find Prototype Object if exist, E_FAIL return;
	HRESULT	Find_Prototype_GameObject(const _tchar* pProtoTag);
	//Find Prototype Component if exist, E_FAIL return;
	HRESULT	Find_Prototype_Component(const _uint& iSceneID, const _tchar* pProtoTag);
	
	//Delete
	HRESULT Remove_Layer_AllObject(const _uint& iSceneID, const _tchar* pLayerTag);
	HRESULT	Remove_Layer_GameObject(const _uint& iSceneID, const _tchar* pLayerTag, CGameObject* pGameObject);

	HRESULT	Check_Collision(const _uint& iSceneID, const _tchar* pLayerTag, const CCollider_Sphere* pCollider);

	//Get
	CComponent*		Clone_Component(const _uint& iSceneID, const _tchar* pComponentTag);

	//Camera
	HRESULT	Set_CameraMain(const _uint& iCameraType, const _tchar* pCameraTag, CCamera** ppCamera = nullptr);
	CCamera*	Get_CameraMain() { return m_pCamera_Main; }
public:
	HRESULT Ready_Management(const _uint& iNumScene, const _uint& iNumCameraType);
	_int Update_Management(const _float& fTimeDelta);
	void Render_Management(LPDIRECT3DDEVICE9	pDevice);
public:
	static void Release_Engine();
public:
	// 현재 화면에 보여지는 씬의 주소.
	CScene*				m_pScene = nullptr;

	CObject_Manager*	m_pObject_Manager = nullptr;
	CComponent_Manager* m_pComponent_Manager = nullptr;
	CCamera_Manager*	m_pCamera_Manager = nullptr;
	CCamera*			m_pCamera_Main = nullptr;
protected:
	virtual void Free();

};

_END