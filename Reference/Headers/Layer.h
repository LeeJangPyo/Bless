#pragma once

// 객체들을 모아서 관리한다.

#include "Base.h"
#include "GameObject.h"

_BEGIN(Engine)

class CCollider_Sphere;
class CLayer final : public CBase
{
private:
	explicit CLayer();
	virtual ~CLayer() = default;

public:	//Get
	CGameObject*	Get_GameObject(const _uint& iIndex);
	CComponent*		Get_Component(const _tchar* pComponentTag, const _uint& iIndex);
	const list<CGameObject*>*	Get_Layer_ObjectList() const { return &m_ObjectList; }
public:
	HRESULT Add_Object(CGameObject* pGameObject);
	HRESULT	Remove_AllObject();
	HRESULT Remove_GameObject(CGameObject* pGameObject);

public:
	HRESULT Ready_Layer();
	_int Update_Layer(const _float& fTimeDelta);
	_int LastUpdate_Layer(const _float& fTimeDelta);

private:
	list<CGameObject*>			m_ObjectList;
public:
	static CLayer* Create();
protected:
	virtual void Free();
};

_END