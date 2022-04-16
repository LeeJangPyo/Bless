#ifndef Mesh_Navigation_Controller_h__
#define Mesh_Navigation_Controller_h__

#include "Defines.h"
#include "GameObject.h"

_BEGIN(Engine)
	class CTransform;
	class CRenderer;
	class CCalculator;
	class CMesh_Navigation;
	class CCell;

	class CFont_Manager;
	class CInput_Device;
_END

//MFC 전방선언
class CNavigationMeshTab;

_BEGIN(Client)

class CMesh_Navigation_Controller : public CGameObject
{
public:
	enum MOUSESELECTTYPE
	{
		INSERT_POINT, SELECT_POINT, SELECT_CELL, MOUSETYPE_END
	};

	struct PICKPOINT
	{
		_ulong			m_SelectIndex;
		_ulong			m_SelectPoint;
	};


public:
	explicit	CMesh_Navigation_Controller(LPDIRECT3DDEVICE9 pDevice);
	explicit	CMesh_Navigation_Controller(const CMesh_Navigation_Controller& rhs);
	virtual		~CMesh_Navigation_Controller(void) = default;

	//set
public:
	void		Set_MouseSelectType(MOUSESELECTTYPE eType) { m_eMouseType = eType; }
	void		Set_MeshTap(CNavigationMeshTab* pNavigationMeshTab) { m_pNavigationMeshTab = pNavigationMeshTab; }
	void		Set_PickCellPoints(_bool bIsPick, const _ulong& dwCellIndex);
	void		Set_PickCellPoint(_bool bIsPick, const _ulong& dwCellIndex, const _ulong& dwPoint);
	void		Set_PickCell(_bool bIsPick, const _ulong& dwCellIndex);

public:
	//Get
	const vector<Engine::CCell*>*	Get_VectorCell(void) const;
	LPDIRECT3DDEVICE9				Get_Device(void) { return m_pDevice; }


public:

	virtual HRESULT Ready_Prototype() override;
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;


	HRESULT	Add_Cell(_vec3* vPointBuffer);

private:
	HRESULT		Ready_Component(void);
	void		Key_Input(const _float fTimeDelta);
	_vec3		PickUp_OnTerrain(void);

	//Related MouseCheck;
	void		Insert_Point();
	void		Select_Point();
	void		Select_Cell();



private:
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CCalculator*			m_pCalculatorCom = nullptr;
	CMesh_Navigation*		m_pNaviMeshCom = nullptr;

	CFont_Manager*			m_pFont_Manager = nullptr;
	CInput_Device*			m_pInput_Device = nullptr;


	_vec3							m_vPointBuffer[3];
	_int							m_iBufferIndex;

	CNavigationMeshTab*				m_pNavigationMeshTab;

	MOUSESELECTTYPE					m_eMouseType;

	//픽한 리스트 저장
	list<PICKPOINT>					m_PickList;
	//픽한 셀 저장
	_ulong							m_dwPickCellIndex;

	_bool							m_bIsMouseDown;

	//
	_bool							m_bIsPick;
		
public:
	static CMesh_Navigation_Controller* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject* Clone_GameObject() override;

protected:
	virtual void Free(void) override;

};

_END

#endif // NavigationMeshControl_h__
