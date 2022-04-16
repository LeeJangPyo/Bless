#pragma once

#include "Base.h"

#include "Component.h"

_BEGIN(Engine)

class _ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

	
public:	//Get
	CComponent*		Get_Component(const _tchar* pComponentTag);

	//Set
	void Set_IsDead() { m_bIsDead = true; }
public:
	//GameObject Component Replace
	virtual HRESULT	Replace_Component(const _tchar* pComponentTag, const _tchar* pComponentProtoTag);
	void Compute_ViewZ(const _vec3* pWorldPos);
	_float Get_ViewZ() const {
		return m_fViewZ;
	}
public:
	virtual HRESULT Ready_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject();
protected:
	HRESULT			Add_Component(const _tchar* pComponentTag, CComponent* pComponent);
	HRESULT			Remove_Component(const _tchar* pComponentTag);
protected:
	LPDIRECT3DDEVICE9		m_pDevice = nullptr;

	map<const _tchar*, CComponent*>			m_mapComponent;
	typedef	map<const _tchar*, CComponent*>	MAPCOMPONENT;


	_bool			m_bIsDead;
	_bool			m_bDraw;
	_float			m_fViewZ = 0.f;

private:
	CComponent*		Find_Component(const _tchar* pComponentTag);
public:
	virtual CGameObject* Clone_GameObject() = 0;
protected:
	virtual void Free();
	
};

_END