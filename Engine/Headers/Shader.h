#ifndef Shader_h__
#define Shader_h__

#include "Component.h"

_BEGIN(Engine)

class _ENGINE_DLL CShader final : public CComponent
{
private:
	explicit CShader(LPDIRECT3DDEVICE9 pDevice);
	explicit CShader(const CShader& rhs);
	virtual ~CShader() = default;

public:
	LPD3DXEFFECT	Get_EffectHandle() const { return m_pEffect; }

public:
	HRESULT		Ready_Shader(const _tchar* pFileName);

private:
	LPD3DXEFFECT	m_pEffect = nullptr;
	LPD3DXBUFFER	m_pErrMsg = nullptr;
public:
	static CShader* Create(LPDIRECT3DDEVICE9 pDevice, const _tchar* pFileName);
	virtual CComponent* Clone_Component();
protected:
	virtual void Free();
};


_END

#endif // Shader_h__