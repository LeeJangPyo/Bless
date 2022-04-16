#ifndef Renderer_h__
#define Renderer_h__

//화면에 그려야하는 객체들을 그려야하는 순서대로 모아놓은 클래스

#include "Component.h"

#include "Target_Manager.h"
#include "Light_Manager.h"
_BEGIN(Engine)

class CShader;
class CGameObject;
class CBuffer_Screen;
class _ENGINE_DLL CRenderer final : public CComponent
{
public:
	enum RENDERGROUP { RENDER_PRIORITY, RENDER_NONEALPHA, RENDER_ALPHA, RENDER_UI,RENDER_SHADOW, RENDER_END };
public:
	explicit CRenderer(LPDIRECT3DDEVICE9 pDevice);
	explicit CRenderer(const CRenderer& rhs);
	virtual ~CRenderer(void) = default;

public:
	HRESULT		Ready_Renderer(void);
public:

	void		Add_RenderGroup(RENDERGROUP eGroup, CGameObject* pGameObject);
	void		Render_ObjectGroup(_bool bDebug, _bool bDeferred);
	void		Clear_RenderGroup(void);

	RENDERGROUP	Get_RenderGroup() {
		return m_eCurrent_RenderGroup;	}
	
private:
	void		Render_Priority();
	void		Render_NoneAlpha();
	void		Render_Alpha();
	void		Render_UI();

private:
	void		Render_Deferred();
	void		Render_LightAcc();
	void		Render_Blend();
	void		Render_Shadow();
	void		Render_Bloom();
private:
	list<CGameObject*>		m_RenderList[RENDER_END];
	typedef list<CGameObject*>	RENDERLIST;
	
	RENDERGROUP				m_eCurrent_RenderGroup;

	CTarget_Manager*		m_pTarget_Manager;
	CShader*				m_pShader_LightAcc;
	CShader*				m_pShader_Blend;
	CShader*				m_pShader_Bloom;
	CBuffer_Screen*			m_pBuffer_Screen;
protected:
	virtual	void	Free(void) override;

public:
	static CComponent* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent* Clone_Component() override;

};

_END

#endif // Renderer_h__