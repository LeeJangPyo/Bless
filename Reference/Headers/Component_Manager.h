#pragma once


// 컴포넌트 원형들을 모아놓는다.

#include "Base.h"
//Static
#include "Transform.h"
#include "Renderer.h"
#include "Calculator.h"
#include "Collider_Box.h"
#include "Collider_Sphere.h"

//Texture
#include "Texture.h"

//Buffer
#include "Buffer_RcTex.h"
#include "Buffer_TerrainTex.h"
#include "Buffer_CubeTex.h"
#include "Buffer_Screen.h"

//Mesh
#include "Mesh_Static.h"
#include "Mesh_Dynamic.h"
#include "Mesh_Navigation.h"

//Utility
#include "Shader.h"
#include "Optimization.h"


_BEGIN(Engine)

class CComponent_Manager final : public CBase
{
	_DECLARE_SINGLETON(CComponent_Manager)
public:
	explicit CComponent_Manager();
	virtual ~CComponent_Manager() = default;
public:
	HRESULT			Reserve_Component_Manager(const _uint& iNumScene);
	HRESULT			Add_Prototype_Component(const _uint& iSceneID, const _tchar* pComponentTag, CComponent* pPrototype);
	CComponent*		Clone_Component(const _uint& iSceneID, const _tchar* pComponentTag);
	HRESULT			Clear_Scene_Container(const _uint& iSceneID);

	HRESULT			Find_Prototype_Component(const _uint& iSceneID, const _tchar* pProtoTag);
private:
	map<wstring, CComponent*>*			m_pMapComponent = nullptr;
	typedef map<wstring, CComponent*>		MAPCOMPONENT;
private:
	_uint			m_iNumScene = 0;
private:
	CComponent* Find_Component(const _uint& iSceneID, const _tchar* pComponentTag);
protected:
	virtual void Free();
};

_END