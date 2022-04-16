#include "..\Headers\Management.h"
#include "Scene.h"
#include "Frame_Manager.h"
#include "Timer_Manager.h"
#include "Graphic_Device.h"
#include "Target_Manager.h"
#include "Light_Manager.h"
#include "Sound_Manager.h"
#include "Camera.h"

_IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement()
	: m_pObject_Manager(CObject_Manager::GetInstance())
	, m_pComponent_Manager(CComponent_Manager::GetInstance())
	, m_pCamera_Manager(CCamera_Manager::GetInstance())

{
	m_pObject_Manager->AddRef();
	m_pComponent_Manager->AddRef();
	m_pCamera_Manager->AddRef();
}
CGameObject * CManagement::Get_GameObject(const _uint & iSceneID, const _tchar * pLayerTag, const _uint& iIndex)
{
	if (m_pObject_Manager == nullptr)
		return nullptr;

	return m_pObject_Manager->Get_GameObject(iSceneID, pLayerTag, iIndex);

}

CComponent * CManagement::Get_Component(const _uint & iSceneID, const _tchar * pLayerTag, const _tchar * pComponentTag, const _uint& iIndex)
{
	if (m_pObject_Manager == nullptr)
		return nullptr;

	return m_pObject_Manager->Get_Component(iSceneID, pLayerTag, pComponentTag, iIndex);
}

const list<CGameObject*>* CManagement::Get_Layer_ObjectList(const _uint & iSceneID, const _tchar * pLayerTag)
{
	if (m_pObject_Manager == nullptr)
		return nullptr;

	return m_pObject_Manager->Get_Layer_ObjectList(iSceneID, pLayerTag);
}

HRESULT CManagement::SetUp_ScenePointer(CScene * pScene)
{
	if (pScene == nullptr)
	{
		_TAGMSG_BOX(L"pScene is Nullptr in SetUp_ScenePointer", L"CManagement");
		return E_FAIL;
	}

	_ulong dwRefCnt = Safe_Release(m_pScene);
	if (0 != dwRefCnt)
		_TAGMSG_BOX(L"Scene Release Failed in Setup_Pointer", L"CManagement");

	m_pScene = pScene;

	m_pScene->AddRef();


	return S_OK;
}

HRESULT CManagement::Add_Prototype_Camera(const _tchar * pProtoTag, CCamera * pCamera)
{
	if (m_pCamera_Manager == nullptr)
	{
		_TAGMSG_BOX(L"m_pCamera_Manager is Nullptr in Add_Prototype_Camera", L"CManagement");
		return E_FAIL;
	}
	if (pCamera == nullptr)
	{
		_TAGMSG_BOX(L"m_pCamera_Manager is Nullptr in Add_Prototype_Camera", L"CManagement");
		return E_FAIL;
	}

	return m_pCamera_Manager->Add_Prototype_Camera(pProtoTag, pCamera);
}

HRESULT CManagement::Add_Camera(const _uint & iCameraType, const _tchar * pProtoTag, const _tchar * pCameraTag, CCamera ** ppCamera)
{
	if (m_pCamera_Manager == nullptr)
	{
		_TAGMSG_BOX(L"m_pCamera_Manager is Nullptr in Add_Camera", L"CManagement");
		return E_FAIL;
	}

	return m_pCamera_Manager->Add_Camera(iCameraType, pProtoTag, pCameraTag, ppCamera);
}

HRESULT CManagement::Add_Prototype_GameObject(const _tchar * pProtoTag, CGameObject * pGameObject, CGameObject** ppGameObject)
{
	if (m_pObject_Manager == nullptr)
	{
		_TAGMSG_BOX(L"Object_Manager is Nullptr in Add_Prototype_GameObject", L"CManagement");
		return E_FAIL;
	}
	if (pGameObject == nullptr)
	{
		_TAGMSG_BOX(L"pGameObject is Nullptr in Add_Prototype_GameObject", L"CManagement");
		return E_FAIL;
	}

	return m_pObject_Manager->Add_Prototype_GameObject(pProtoTag, pGameObject,ppGameObject);
}

HRESULT CManagement::Add_Prototype_Component(const _uint & iSceneID, const _tchar * pProtoTag, CComponent * pComponent)
{
	if (m_pComponent_Manager == nullptr)
	{
		_TAGMSG_BOX(L"Object_Manager is Nullptr in Add_Prototype_GameObject", L"CManagement");
		return E_FAIL;
	}
	if (pComponent == nullptr)
	{
		_TAGMSG_BOX(L"pComponent is Nullptr in Add_Prototype_GameObject", L"CManagement");
		return E_FAIL;
	}
	return m_pComponent_Manager->Add_Prototype_Component(iSceneID,pProtoTag,pComponent);
}

HRESULT CManagement::Add_Layer(const _uint & iSceneID, const _tchar * pLayerTag, const _tchar * pProtoTag, CGameObject** ppGameObject)
{
	if (m_pObject_Manager == nullptr)
	{
		_TAGMSG_BOX(L"Object_Manager is Nullptr in Add_Layer", L"CManagement");
		return E_FAIL;
	}

	return m_pObject_Manager->Add_Layer(iSceneID, pLayerTag, pProtoTag, ppGameObject);
}

HRESULT CManagement::Clear_Scene_Container(const _uint & iSceneID)
{
	if (nullptr == m_pObject_Manager ||
		nullptr == m_pComponent_Manager)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Clear_Scene_Container(iSceneID)))
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Clear_Scene_Container(iSceneID)))
		return E_FAIL;

	return S_OK;
}

HRESULT CManagement::Find_Prototype_GameObject(const _tchar * pProtoTag)
{
	if (m_pObject_Manager == nullptr)
		return E_FAIL;
	if (FAILED(m_pObject_Manager->Find_Prototype_GameObject(pProtoTag)))
		return E_FAIL;
	return S_OK;
}

HRESULT CManagement::Find_Prototype_Component(const _uint & iSceneID, const _tchar * pProtoTag)
{
	if (m_pComponent_Manager == nullptr)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Find_Prototype_Component(iSceneID,pProtoTag)))
		return E_FAIL;
	return S_OK;
}

HRESULT CManagement::Remove_Layer_AllObject(const _uint & iSceneID, const _tchar * pLayerTag)
{
	if (m_pObject_Manager == nullptr)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Remove_AllObject(iSceneID, pLayerTag)))
		return E_FAIL;
	return S_OK;
}

HRESULT CManagement::Remove_Layer_GameObject(const _uint & iSceneID, const _tchar* pLayerTag, CGameObject * pGameObject)
{
	if (m_pObject_Manager == nullptr)
		return E_FAIL;

	return m_pObject_Manager->Remove_Layer_GameObject(iSceneID, pLayerTag, pGameObject);
}

HRESULT CManagement::Check_Collision(const _uint & iSceneID, const _tchar * pLayerTag, const CCollider_Sphere * pCollider)
{
	if (m_pObject_Manager == nullptr)
		return E_FAIL;

	

	return S_OK;
}



CComponent * CManagement::Clone_Component(const _uint & iSceneID, const _tchar * pComponentTag)
{
	if(m_pComponent_Manager == nullptr)
	{
		_TAGMSG_BOX(L"Component_manager Is Nullptr, in Get_Component", L"CManagement");
		return nullptr;
	}

	return m_pComponent_Manager->Clone_Component(iSceneID, pComponentTag);
}

HRESULT CManagement::Set_CameraMain(const _uint & iCameraType, const _tchar * pCameraTag, CCamera** ppCamera)
{
	if (m_pCamera_Manager == nullptr)
		return E_FAIL;

	Safe_Release(m_pCamera_Main);

	m_pCamera_Main = m_pCamera_Manager->Get_Camera(iCameraType, pCameraTag);

	if (m_pCamera_Main == nullptr)
		return E_FAIL;
	m_pCamera_Main->AddRef();

	if (ppCamera != nullptr)
		*ppCamera = m_pCamera_Main;

	m_pCamera_Main->SetUp_Matrix();

	return S_OK;
}

HRESULT CManagement::Ready_Management(const _uint& iNumScene, const _uint& iNumCameraType)
{

	//Object_Manager Ready
	if (m_pObject_Manager == nullptr)
	{
		_TAGMSG_BOX(L"Object_Manager is Nullptr in Ready_Management", L"CManagement");
		return E_FAIL;
	}

	if (FAILED(m_pObject_Manager->Reserve_Object_Manager(iNumScene)))
	{
		_TAGMSG_BOX(L"Object_Manager Reserve Failed in Ready_Management", L"CManagement");
		return E_FAIL;
	}

	//Component_Manager Ready
	if (m_pComponent_Manager == nullptr)
	{
		_TAGMSG_BOX(L"Component_Manager is Nullptr in Ready_Management", L"CManagement");
		return E_FAIL;
	}

	if (FAILED(m_pComponent_Manager->Reserve_Component_Manager(iNumScene)))
	{
		_TAGMSG_BOX(L"Component_Manager Reserve Failed in Ready_Management", L"CManagement");
		return E_FAIL;
	}

	//Camera_Manager Ready
	if (m_pCamera_Manager == nullptr)
	{
		_TAGMSG_BOX(L"m_pCamera_Manager is Nullptr in Ready_Management", L"CManagement");
		return E_FAIL;
	}

	if (FAILED(m_pCamera_Manager->Reserve_Camera_Manager(iNumCameraType)))
	{
		_TAGMSG_BOX(L"Component_Manager Reserve Failed in Ready_Management", L"CManagement");
		return E_FAIL;
	}

	
	return S_OK;
}

_int CManagement::Update_Management(const _float & fTimeDelta)
{
	if (m_pScene == nullptr)
	{
		_TAGMSG_BOX(L"Scene is Nullptr, in Update_Management", L"CManagement");
		return -1;
	}

	_int		iProcessCode = 0;

	iProcessCode = m_pScene->Update_Scene(fTimeDelta);
	if (iProcessCode & 0x80000000)
	{
		_TAGMSG_BOX(L"Update Failed, in Update_Management", L"CManagement");
		return -1;
	}

	iProcessCode = m_pScene->LastUpdate_Scene(fTimeDelta);
	if (iProcessCode & 0x80000000)
	{
		_TAGMSG_BOX(L"LastUpdate Failed, in Update_Management", L"CManagement");
		return -1;
	}
	if (m_pCamera_Main == nullptr)
		return E_FAIL;
	
	//메인 카메라
	m_pCamera_Main->Update_GameObject(fTimeDelta);


	return iProcessCode;

}

void CManagement::Render_Management(LPDIRECT3DDEVICE9	pDevice)
{
	
	if (m_pScene == nullptr)
	{
		_TAGMSG_BOX(L"Scene is Nullptr, in Update_Management", L"CManagement");
		return;
	}

	m_pScene->Render_Scene();

#if _DEBUG
	m_pCamera_Manager->Render_Debug();
#endif

}

void CManagement::Release_Engine()
{
	_ulong		dwRefCnt = 0;


	if (dwRefCnt = CManagement::GetInstance()->DestroyInstance())
		_MSG_BOX(L"CManagement Release Failed");

	if (dwRefCnt = CObject_Manager::GetInstance()->DestroyInstance())
		_MSG_BOX(L"CObject_Manager Release Failed");

	if (dwRefCnt = CComponent_Manager::GetInstance()->DestroyInstance())
		_MSG_BOX(L"CComponent_Manager Release Failed");

	if (dwRefCnt = CCamera_Manager::GetInstance()->DestroyInstance())
		_MSG_BOX(L"CCamera_Manager Release Failed");

	if (dwRefCnt = CInput_Device::GetInstance()->DestroyInstance())
		_MSG_BOX(L"CInput_Device Release Failed");

	if (dwRefCnt = CTarget_Manager::GetInstance()->DestroyInstance())
		_MSG_BOX(L"CTarget_Manager Release Failed");

	if (dwRefCnt = CFont_Manager::GetInstance()->DestroyInstance())
		_MSG_BOX(L"CFont Manager Release Failed");

	if (dwRefCnt = CLine_Manager::GetInstance()->DestroyInstance())
		_MSG_BOX(L"CLine_Manager Release Failed");

	if (dwRefCnt = CLight_Manager::GetInstance()->DestroyInstance())
		_MSG_BOX(L"CLight_Manager Release Failed");

	if (dwRefCnt = CGraphic_Device::GetInstance()->DestroyInstance())
		_MSG_BOX(L"CGraphic_Device Release Failed");

	if (dwRefCnt = CTimer_Manager::GetInstance()->DestroyInstance())
		_MSG_BOX(L"CTimer_Manager Release Failed");

	if (dwRefCnt = CFrame_Manager::GetInstance()->DestroyInstance())
		_MSG_BOX(L"CFrame_Manager Release Failed");

	if (dwRefCnt = CSound_Manager::GetInstance()->DestroyInstance())
		_MSG_BOX(L"CSound_Manager Release Failed");
}

void CManagement::Free()
{
	Safe_Release(m_pCamera_Main);
	Safe_Release(m_pCamera_Manager);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pScene);
}
