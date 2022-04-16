#ifndef Effect_Rect_h__
#define Effect_Rect_h__

#include "Effect.h"

_BEGIN(Engine)
class CTexture;
class CBuffer_RcTex;
_END

_BEGIN(Client)

class CEffect_Rect final : public CEffect
{
public:
	explicit CEffect_Rect(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CEffect_Rect() = default;


public:

	virtual HRESULT Ready_Effect() override;
	virtual _int Update_Effect(const _float& fTimeDelta) override;
	virtual _int LastUpdate_Effect(const _float& fTimeDelta) override;
	virtual void Render_Effect(LPD3DXEFFECT pEffect, const _matrix* pmatParent) override;
	virtual void Initalize_Effect() override;
public:

	void		SetUp_EffectData(const EFFECTDATA& EffectData, const EFFECTRECT_DATA& Effect_RectData);
private:
	HRESULT			Ready_Component();

private:
	CBuffer_RcTex*	m_pBufferCom;

	EFFECTRECT_DATA	m_EffectRect_Data;
public:
	static CEffect_Rect* Create(LPDIRECT3DDEVICE9 pDevice);
protected:
	virtual void Free() override;

};

_END

#endif // Effect_Rect_h__
