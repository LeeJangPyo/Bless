#pragma once

#include "Light.h"

_BEGIN(Engine)

class _ENGINE_DLL CLight_Manager final : public CBase
{
	_DECLARE_SINGLETON(CLight_Manager)
private:
	explicit CLight_Manager(void);
	virtual ~CLight_Manager(void) = default;
public:
	const D3DLIGHT9* Get_LightInfo(const _uint& iIndex = 0);
	CLight*		Get_Light(const _uint& iIndex = 0);
	const list<CLight*>*	Get_LightList() const {
		return &m_LightList;	}

	const _matrix* Get_LightViewMatrix() {
		return &m_matShadowLight;	}

	HRESULT	 Remove_All_Light();

	void	Set_ShadowLightMatrix(const _vec3* pLightPosition, const _vec3* pLightAt, const _vec3* pLightUp);

public:
	HRESULT Ready_Light(LPDIRECT3DDEVICE9	pDevice, const D3DLIGHT9* pLightInfo, const _uint& iIndex);
	void	Render_Light(LPD3DXEFFECT pEffect);

	HRESULT	Load_LightInfo_FromFile(LPDIRECT3DDEVICE9 pDevice,const _tchar* pFilePath);
private:
	list<CLight*>			m_LightList;
	
	_matrix					m_matShadowLight;
private:
	virtual void Free(void);
};

_END

