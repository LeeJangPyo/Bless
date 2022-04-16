#ifndef Scene_MapEdit_h__
#define Scene_MapEdit_h__

#include "Scene.h"

_BEGIN(Client)

class CScene_MapEdit final : public CScene
{
private:
	explicit	CScene_MapEdit(LPDIRECT3DDEVICE9	pDevice);
	virtual		~CScene_MapEdit(void) = default;


public:
	virtual HRESULT Ready_Scene() override;
	virtual _int	Update_Scene(const _float& fTimeDelta) override;
	virtual _int	LastUpdate_Scene(const _float& fTimeDelta) override;
	virtual void	Render_Scene() override;



private:
	HRESULT Ready_Prototype_GameObject();
	HRESULT Ready_Prototype_Component();
	HRESULT	Ready_GameObject_Layer();

private:

public:
	static	CScene_MapEdit*		Create(LPDIRECT3DDEVICE9	pDevice);

public:
	virtual void Free(void) override;
};

_END


#endif // Scene_MapEdit_h__
