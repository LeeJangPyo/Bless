#pragma once

#include "Base.h"
#include "Layer.h"

_BEGIN(Engine)
class CCollider_Sphere;
class CObject_Manager final : public CBase
{
	_DECLARE_SINGLETON(CObject_Manager)
private:
	explicit CObject_Manager();
	virtual ~CObject_Manager() = default;
	
public:	//Get	
	CGameObject*		Get_GameObject(const _uint& iSceneID, const _tchar* pLayerTag, const _uint& iIndex = 0);
	CComponent*			Get_Component(const _uint& iSceneID, const _tchar* pLayerTag, const _tchar* pComponentTag, const _uint& iIndex = 0);
	const list<CGameObject*>*		Get_Layer_ObjectList(const _uint& iSceneID, const _tchar* pLayerTag) ;
public:
	HRESULT Reserve_Object_Manager(const _uint& iNumScene);
	HRESULT Add_Prototype_GameObject(const _tchar* pProtoTag, CGameObject* pGameObject, CGameObject** ppGameObject = nullptr);
	HRESULT Add_Layer(const _uint& iSceneID, const _tchar* pLayerTag, const _tchar* pProtoTag, CGameObject** ppGameObject = nullptr);
	HRESULT	Remove_Layer(const _uint& iSceneID, const _tchar* pLayerTag);
	HRESULT Clear_Scene_Container(const _uint& iSceneID);
	
	// Find
	HRESULT	Find_Prototype_GameObject(const _tchar* pProtoTag);

	//Delete
	HRESULT Remove_AllObject(const _uint& iSceneID, const _tchar* pLayerTag);
	HRESULT Remove_Layer_GameObject(const _uint& iSceneID, const _tchar* pLayerTag, CGameObject* pObject);

	//Pool
	HRESULT	Reserve_ObjectPool(const _tchar* pObjectTag, const _int& iNumObject);

public:
	_int Update_Object_Manager(const _float& fTimeDelta);
	_int LastUpdate_Object_Manager(const _float& fTimeDelta);
	_int Update_Object_Manager(const _uint& iSceneID, const _float& fTimeDelta);
	_int LastUpdate_Object_Manager(const _uint& iSceneID, const _float& fTimeDelta);
private:
//	map<const _tchar*, CGameObject*>			m_mapPrototype;
//	typedef map<const _tchar*, CGameObject*>	MAPPROTOTYPE;
	map<wstring, CGameObject*>			m_mapPrototype;
	typedef map<wstring*, CGameObject*>	MAPPROTOTYPE;
private: // 실제 게임에 사용되는 객체들을 모아놓은 레이어들
//	map<const _tchar*, CLayer*>*				m_pMapLayer = nullptr;
//	typedef map<const _tchar*, CLayer*>			MAPLAYERS;
	map<wstring, CLayer*>*				m_pMapLayer = nullptr;
	typedef map<wstring, CLayer*>			MAPLAYERS;
private:
	//오브젝트풀
	map <const _tchar*, vector<CGameObject*>>	m_mapObjectPool;
	

private:
	_uint		m_iNumScene = 0;
private:
	CGameObject* Find_Prototype(const _tchar* pProtoTag);
	CLayer* Find_Layer(const _uint& iSceneID, const _tchar* pLayerTag);
protected:
	virtual void Free();
};

_END
