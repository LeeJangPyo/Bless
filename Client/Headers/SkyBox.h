#ifndef SkyBox_h__
#define SkyBox_h__


#include "Defines.h"
#include "GameObject.h"


_BEGIN(Engine)
class CTexture;
class CTransform;
class CBuffer_CubeTex;
class CRenderer;
class CShader;
_END


_BEGIN(Client)
class CSkyBox final: public CGameObject
{
private:
	explicit	CSkyBox(LPDIRECT3DDEVICE9 pDevice);
	explicit	CSkyBox(const CSkyBox& rhs);
	virtual		~CSkyBox(void) = default;


public:
	virtual HRESULT Ready_Prototype();
	virtual HRESULT Ready_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject();

private:
	HRESULT	Ready_Component(void);
	HRESULT	SetUp_ConstantTable(LPD3DXEFFECT pEffect);


private:
	CTexture*				m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CBuffer_CubeTex*		m_pBufferCom  =nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CShader*				m_pShaderCom = nullptr;

public:
	static CSkyBox* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual	CGameObject* Clone_GameObject() override;
protected:
	virtual void Free(void) override;

};
_END
#endif