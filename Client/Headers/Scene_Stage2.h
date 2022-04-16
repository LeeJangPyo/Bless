#ifndef Scene_Stage2_h__
#define Scene_Stage2_h__

#include "Defines.h"
#include "Scene.h"

_BEGIN(Client)


class CScene_Stage2 final : public CScene
{
public:
	explicit CScene_Stage2(LPDIRECT3DDEVICE9 pDeivce);
	virtual ~CScene_Stage2() = default;

public:
	virtual HRESULT Ready_Scene() override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual _int LastUpdate_Scene(const _float& fTimeDelta) override;
	virtual void Render_Scene() override;

private:
	HRESULT		Ready_LightInfo();
	HRESULT		Ready_GameEvent();

	HRESULT		Create_Prototype_Component_Mesh(OBJECTTYPE eType, const _tchar * pFilePath, const _tchar * pFileName, const _tchar * pComponentMeshTag);
	HRESULT		Create_Prototype_GameObject(const _tchar * pGameObjectTag);
	HRESULT		Add_Layer_GameObject(const _tchar* pLayerTag, const _tchar * pGameObjectTag, CGameObject ** ppGameObject);
	HRESULT		Load_GameObject_Static_FromFile(const _tchar* pFilePath);	
	HRESULT		Load_GameObject_Dynamic_FromFile(const _tchar* pFilePath);
public:
	static CScene_Stage2* Create(LPDIRECT3DDEVICE9 pDevice);


protected:
	virtual void Free() override;
};


_END


#endif // Scene_Stage2_h__