#pragma once
#include "afxcmn.h"
#include "afxwin.h"

// CMeshTab 대화 상자입니다.

#include "Object_Mesh.h"

//전방선언

_BEGIN(Engine)
	class CCalculator;
_END

class CMeshTab : public CDialogEx
{
	DECLARE_DYNAMIC(CMeshTab)

public:
	enum DATATYPE
	{
		DATA_SCALE, DATA_POSITION, DATA_ROTATION
	};
	enum OBJECTTYPE
	{
		STATIC_OBJECT, DYNAMIC_OBJECT
	};

public:
	CMeshTab(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMeshTab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MESH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:

	//Get
	OBJECTTYPE	Get_ObjectType() {
		return m_eObjectType;	}
	
	CCalculator*				m_pCalculatorCom = nullptr;
	Client::CObject_Mesh*		m_pPickedMeshObject;
	_vec3		PickUp_OnTerrain(void);
	void		SetUp_OnTerrain(D3DXVECTOR3& vPos);
	HRESULT		Ready_Component();

	CTreeCtrl	m_TreeStaticMeshList;
	CTreeCtrl	m_TreeDynamicMeshList;
	HRESULT		Set_ObjectList(void);
	HRESULT		FindDirAddMeshList(	const wstring& wstrPath,
				CTreeCtrl&	rTreeCtrl,
				HTREEITEM hTreeItem);

	CButton m_btRenderState[2];
	afx_msg void OnBnClickedRbSolid();
	afx_msg void OnBnClickedRbWire();
	CButton m_RbObjectSelect[2];
	afx_msg void OnBnClickedRbStatic();
	afx_msg void OnBnClickedRbDynamic();

	float m_fRotation[3];

	virtual BOOL OnInitDialog();

	afx_msg void OnNMDblclkStaticObjectList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkDynamicObjectList(NMHDR *pNMHDR, LRESULT *pResult);

	void			Update_Transform(DATATYPE eDataType, const D3DXVECTOR3* pTransform);
	void			Get_Trasnform(DATATYPE eDataTyope, D3DXVECTOR3* pTransform);


	virtual void PostNcDestroy();
	afx_msg void OnBnClickedBtApply();
	afx_msg void OnBnClickedSaveStaticMeshList();
	afx_msg void OnBnClickedLoadStaticMesh();
//	afx_msg void OnBnClickedSaveDynamicMesh();
//	afx_msg void OnBnClickedLoadDynamicMesh();
	CButton m_btTransformType[3];
	afx_msg void OnBnClickedRadioScale();
	afx_msg void OnBnClickedRadioRotation();
	afx_msg void OnBnClickedRadioPosition();
	CSpinButtonCtrl m_Spin_Scale_X;
	afx_msg void OnDeltaposSpinScaleX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinScaleY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinScaleZ(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnDeltaposSpinPositionY(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnDeltaposSpinPositionZ2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinRotationY(NMHDR *pNMHDR, LRESULT *pResult);

	enum MeshType
	{
		MESH_DYNAMIC, MESH_STATIC, MESH_END
	};
	HRESULT		Create_Prototype_Component_Mesh(MeshType eType, const _tchar* pFilePath, const _tchar* pFileName, const _tchar* pComponentMeshTag);
	HRESULT		Create_Prototype_GameObject(MeshType eType, const _tchar* pGameObjectTag);
	HRESULT		Add_Layer_GameObject(const _tchar* pLayerTag, const _tchar* pGameObjectTag, CGameObject** ppGameObject);

	void		SetUp_EditBox_DefaultInfo(const _vec3* pPosition);
	afx_msg void OnBnClickedBtTransformApply();
	float m_fPositionX;
	float m_fPositionY;
	float m_fPositionZ;
	afx_msg void OnBnClickedBtDelete();
	afx_msg void OnBnClickedBtSaveDynamic();
	afx_msg void OnBnClickedBtLoadDyanmic();
	float m_fScaleX;
	float m_fScaleY;
	float m_fScaleZ;
	CString m_strDynamic_LayerTag;


	

	OBJECTTYPE m_eObjectType;
	afx_msg void OnBnClickedBtLayertag();
};
