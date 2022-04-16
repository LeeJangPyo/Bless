#pragma once

#include "GameObject.h"
#include "Defines.h"

_BEGIN(Engine)
class CTransform;
class CBuffer_RcTex;
class CTexture;
class CRenderer;
class CShader;
_END

_BEGIN(Client)

class CTitle_Logo final : public CGameObject
{
private:
	explicit CTitle_Logo(LPDIRECT3DDEVICE9 pDevice);
	explicit CTitle_Logo(const CTitle_Logo& rhs);
	virtual ~CTitle_Logo() = default;
public:
	virtual HRESULT Ready_Prototype() override;
	virtual HRESULT Ready_GameObject() override;
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual _int	LastUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void	Render_GameObject() override;

private:
	HRESULT			Ready_Component(void);


private:
	CRenderer*		m_pRendererCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CBuffer_RcTex*	m_pBufferCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
	CShader*		m_pShaderCom = nullptr;

	_float			m_fFrame;
	_float			m_fMaxFrame;

public:
	static CTitle_Logo* Create(LPDIRECT3DDEVICE9 pDevice); // 우너형객체 샏성
	virtual CGameObject* Clone_GameObject() override; // 실제 사용할 객체를 생서아.

public:
	virtual void Free(void) override;
};

_END