#ifndef Object_Mesh_h__
#define Object_Mesh_h__


#include "Defines.h"
#include "GameObject.h"
#include "Management.h"
#include "Input_Device.h"
_BEGIN(Engine)
	class CTransform;
	class CRenderer;
	class CCalculator;
	class CShader;
//	class COptimization;
_END

class CMeshTab;

_BEGIN(Client)
class CObject_Mesh abstract : public CGameObject
{
protected:
	explicit	CObject_Mesh(LPDIRECT3DDEVICE9 pDevice);
	explicit	CObject_Mesh(const CObject_Mesh& rhs);
	virtual		~CObject_Mesh(void) = default;

public:
	void	Set_Position(const _vec3* pPosition);
	void	Set_Rotation(const _vec3* pRotation);
	void	Set_Scale(const _vec3* pScale);

	void	Set_Position(const _float& fX, const _float& fY, const _float& fZ);
	void	Set_Rotation(const _float& fX, const _float& fY, const _float& fZ);
	void	Set_Scale(const _float& fX, const _float& fY, const _float& fZ);

	HRESULT	SetUp_FileInfo(const _tchar* pFilePath, const _tchar* pFileName, const _tchar* pGameObjectTag, const _tchar* pComponentMeshTag);

	const _tchar*	Get_ComponentMeshTag() { return m_szComponentMeshTag; }
	const _tchar*	Get_FilePath() {		return m_szFilePath;}
	const _tchar*	Get_FileName() { return m_szFileName; }
	const _tchar*	Get_GameObjecTag() { return m_szGameObjectTag; }

protected:
	virtual HRESULT	Ready_Component();

protected:
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CShader*			m_pShaderCom = nullptr;

	CInput_Device*				m_pInput_Device = nullptr;

	CMeshTab*					m_pMeshTab = nullptr;

	_bool						m_bIsPick;
	_bool						m_bIsMouseDown;


	//File ÀúÀå¿ë
	_tchar				m_szFilePath[MIN_PATH];
	_tchar				m_szFileName[MIN_PATH];
	_tchar				m_szGameObjectTag[MIN_PATH];
	_tchar				m_szComponentMeshTag[MIN_PATH];
public:
	virtual CGameObject* Clone_GameObject() = 0;

protected:
	virtual void Free(void) override;
};

_END

#endif // Object_Mesh_h__
