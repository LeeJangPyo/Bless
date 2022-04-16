#pragma once
#include "afxwin.h"

_BEGIN(Engine)
class CGameObject;
_END

_BEGIN(Client)
// class CEffect_Mesh;
// class CEffect_Particle;
// class CEffect_Rect;
class CEffect;
class CEffectGroup;
_END

// CEffectTab 대화 상자입니다.

class CEffectTab : public CDialogEx
{
	DECLARE_DYNAMIC(CEffectTab)

public:
	CEffectTab(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEffectTab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EFFECT };
#endif
private:
	enum EFFECT_TYPE	
	{
		EFFECT_MESH, EFFECT_PARTICLE, EFFECT_RECT
	};
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
 	Client::CEffectGroup*		m_pEffectGroup = nullptr;
	Client::CEffect*			m_pEffect = nullptr;


	float m_fPositionX;
	float m_fPositionY;
	float m_fPositionZ;
	float m_fScaleX;
	float m_fScaleY;
	float m_fScaleZ;
	float m_fRotationX;
	float m_fRotationY;
	float m_fRotationZ;

	CButton m_bt_Default;
	CButton m_bt_Alpha;
	CButton m_bt_TwoTexutre;
	CButton m_bt_NoisePass;


	float m_fUSpeed;
	float m_fVSpeed;

	float m_fMaxTheta;
	float m_fMinTheta;
	float m_fMaxPhi;
	float m_fMinPhi;
	float m_fInit_Speed;
	float m_fSpeed_Range;
	float m_fInit_LifeTime;
	float m_fLifeTime_Range;
	float m_fInit_Acc_X;
	float m_fInit_Acc_Y;
	float m_fInit_Acc_Z;
	float m_fFrame_Const;

	float m_fAlpha;
	float m_fInit_Alpha;

	int m_iAlpha_Width;
	int m_iAlpha_Height;
	int m_iBase_Width;
	int m_iBase_Height;

	int m_iParticleNum;

	float m_fFadeOut;
	float m_fFadeIn;

	CListBox m_Mesh_List;
	CListBox m_Rect_List;
	CListBox m_Particle_List;

	afx_msg void OnBnClickedCreateMesh();
	afx_msg void OnBnClickedCreateRect();

	HRESULT Create_Prototype_GameObject(const _tchar * pGameObjectTag);
	HRESULT Add_Layer_GameObject(const _tchar* pLayerTag, const _tchar * pGameObjectTag, CGameObject** ppGameObject);

	afx_msg void OnBnClickedCheckPass0();
	afx_msg void OnBnClickedCheckPass1();
	afx_msg void OnBnClickedCheckPass2();
	afx_msg void OnBnClickedCheckPass3();


	afx_msg void OnBnClickedBtUvanimationApply();
	afx_msg void OnBnClickedCreateParticle();
	afx_msg void OnBnClickedBtParticleInfoAllpy();

	afx_msg void OnLbnSelchangeListMesh();
	afx_msg void OnLbnSelchangeListRect();
	afx_msg void OnLbnSelchangeListParticle();
	afx_msg void OnBnClickedBtTransformApply();

	float m_fStart_Position_X;
	float m_fStart_Position_Y;
	float m_fStart_Position_Z;
	float m_fEnd_Position_X;
	float m_fEnd_Position_Y;
	float m_fEnd_Position_Z;
	afx_msg void OnBnClickedBtPlay();
	afx_msg void OnBnClickedBtSPositionSet();
	afx_msg void OnBnClickedBtEPositionSet();

	float m_fRotation_Start_X;
	float m_fRotation_Start_Y;
	float m_fRotation_Start_Z;
	float m_fRotation_End_X;
	float m_fRotation_End_Y;
	float m_fRotation_End_Z;
	afx_msg void OnBnClickedBtSRotationSet();
	afx_msg void OnBnClickedBtERotationSet();

	float m_fScale_Start_X;
	float m_fScale_Start_Y;
	float m_fScale_Start_Z;
	float m_fScale_End_X;
	float m_fScale_End_Y;
	float m_fScale_End_Z;
	afx_msg void OnBnClickedBtSScaleSet();
	afx_msg void OnBnClickedBtEScaleSet();

	afx_msg void OnBnClickedEffectSave();
	afx_msg void OnBnClickedEffectLoad();
	CString m_strTag;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCreateGroup();

	void	Add_EffectMesh_List();
	void	Add_EffectRect_List();
	void	Add_EffectParticle_List();

	
	CString m_strGroupName;
	float m_fGroupEffect_LifeTime;
	afx_msg void OnBnClickedBtGroupeffectLifetimeApply();
	float m_fStartTime;
	float m_fEndTime;
	afx_msg void OnBnClickedBtEffectDeleteParticle();
	afx_msg void OnBnClickedBtEffectDeleteRect();
	afx_msg void OnBnClickedBtEffectDeleteMesh();
	afx_msg void OnBnClickedBtSelectClear();



	CButton m_bt_Recycle;
	afx_msg void OnBnClickedCheckRecycle();


	afx_msg void OnBnClickedBtSetTag();
	afx_msg void OnBnClickedCheckPass4();
	CButton m_bt_NoneAlhpa;
	CString m_strMeshTag;
	afx_msg void OnBnClickedMeshTagBt();
};
