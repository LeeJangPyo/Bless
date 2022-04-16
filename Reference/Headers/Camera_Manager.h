
#ifndef Camera_Manager_h__
#define Camera_Manager_h__

#include "Base.h"
#include "Camera.h"

_BEGIN(Engine)

class _ENGINE_DLL CCamera_Manager final : public CBase
{
	_DECLARE_SINGLETON(CCamera_Manager)
public:
	explicit CCamera_Manager();
	virtual ~CCamera_Manager() = default;
	
	//Get Camera
	CCamera* Get_Camera(const _uint& iCameraType, const _tchar* pCameraTag);

public:
	HRESULT Reserve_Camera_Manager(const _uint& iNumCameraType);
	HRESULT Add_Prototype_Camera(const _tchar* pProtoTag, CCamera* pCamera);
	HRESULT Add_Camera(const _uint & iCameraType, const _tchar * pProtoTag, const _tchar* pCameraTag, CCamera ** ppCamera);
	
	void	Render_Debug();
	
private:
	CCamera*	Find_Prototype(const _tchar* pProtoTag);
	CCamera*	Find_Camera(const _uint& iCameraType, const _tchar* pCameraTag);
private:
	map<wstring, CCamera*>				m_mapPrototype;
	typedef map<wstring*, CCamera*>		MAPPROTOTYPE;

	map<wstring, CCamera*>*				m_pmapCamera = nullptr;
	typedef map<wstring, CCamera*>		MAPCAMERAS;

private:
	_uint		m_iNumCameraType = 0;
protected:
	virtual void Free() override;
};

_END

#endif // Camera_Manager_h__
