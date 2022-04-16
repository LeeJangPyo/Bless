#ifndef Target_Manager_h__
#define Target_Manager_h__


#include "Base.h"
#include "Target.h"
// 타겟들을 모아놓은 매니저 클래스
_BEGIN(Engine)


class _ENGINE_DLL CTarget_Manager final : public CBase
{
	_DECLARE_SINGLETON(CTarget_Manager)
public:
	explicit CTarget_Manager();
	virtual ~CTarget_Manager() = default;

public:
	HRESULT		Ready_Target(LPDIRECT3DDEVICE9 pDevice, const _tchar* pTargetTag, const _float& fSizeX, const _float& fSizeY, D3DFORMAT Format, D3DXCOLOR Clear_Color,CTarget::TARGETTYPE eType);
	HRESULT		Ready_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag);


	HRESULT		Begin_MRT(const _tchar* pMRTTag);
	HRESULT		End_MRT(const _tchar* pMRTTag);

	HRESULT		Begin_Shadow_MRT(const _tchar* pMRTTag);
	HRESULT		End_Shadow_MRT(const _tchar* pMRTTag);

	HRESULT		Ready_DebugBuffer(const _tchar* pTargetTag, const _float& fStartX, const _float& fStartY, const _float& fSizeX, const _float& fSizeY);
	void		Render_DebugBuffer(const _tchar* pMRTTag);

	HRESULT		SetUp_Texture_OnShader(LPD3DXEFFECT pEffect, const _tchar* pTargetTag, const _char* pConstantName);
private:
	map<const _tchar*, CTarget*>		m_mapTarget;
	typedef map<const _tchar*, CTarget*>		MAPTARGET;
private:
	map<const _tchar*, list<CTarget*>>			m_mapMRT;
	typedef map<const _tchar*, list<CTarget*>>	MAPMRT;
private:
	CTarget*		Find_Target(const _tchar* pTargetTag);
	list<CTarget*>*	Find_MRT(const _tchar* pMRTTag);

protected:
	virtual void Free();
};
_END

#endif // Target_Manager_h__
