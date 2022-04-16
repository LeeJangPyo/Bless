#ifndef DynamicMesh_h__
#define DynamicMesh_h__


#include "Mesh.h"
#include "Hierachy_Loader.h"
#include "Animation_Controller.h"

_BEGIN(Engine)


class _ENGINE_DLL CDynamicMesh : public CMesh
{
private:
	explicit	CDynamicMesh(LPDIRECT3DDEVICE9 pDevice);
	explicit	CDynamicMesh(const CDynamicMesh& rhs);
	virtual		~CDynamicMesh(void);

public:
	_bool						Is_AnimationSetEnd(void);
	const D3DXFRAME_DERIVED*	Get_FrameByName(const char* pFrameName);
	_uint						Get_MaxAnimation(void);
	D3DXFRAME*					Get_RootFrame(void) { return m_pRootFrame; }


public:
	HRESULT		 Ready_DynamicMesh(const _tchar* pFilePath, const _tchar* pFileName);
	void		 Render_DynamicMesh(void);
	void		 Render_DynamicMesh(LPD3DXEFFECT	pEffect);
	void		 Set_AnimationSet(const _uint& iIndex);
	void		 Play_AnimationSet(const _float& fTimeDelta);


private:
	//��� ������ ��ȸ�ϸ鼭 ������ �ִ� ��� ������ �����ϴ� �Լ�
	void		Update_FrameMatrices(D3DXFRAME_DERIVED* pFrame,
									const _matrix* pParentMatrix);
	void		SetUp_FrameMatrixPointer(D3DXFRAME_DERIVED* pFrame);

private:
	D3DXFRAME*							m_pRootFrame;
	CHierachy_Loader*					m_pHierachy_Loader;
	CAnimation_Controller*				m_pAnimation_Controller;
	list<D3DXMESHCONTAINER_DERIVED*>	m_MeshContainerList;


public:
	static CDynamicMesh*		Create(LPDIRECT3DDEVICE9	pDevice,
		const _tchar* pFilePath,
		const _tchar* pFileName);
	virtual CComponent*			Clone_Component(void) override;
	virtual void				Free(void);
};

_END

#endif // DynamicMesh_h__


//			D3DXFRAME		//����
//LPSTR                   Name;					// ���� �̸�(x���Ͽ��� �о�� ���� �̸�)
//D3DXMATRIX              TransformationMatrix;	// �� �ϳ��� ���� �ε� ���¿����� ���(ex: ���� ���� �������� �󸶸�ŭ �̵��� �ִ°�)
//
//LPD3DXMESHCONTAINER     pMeshContainer;			// �޽� �����̸� �����ϴ� ���� �޽�(������ static �޽�)�� ������ �����ϴ� ����ü (���� ���� : ��� �����̵��� �޽��� �� ������ �� ���� ������ �ʴ�)
//
//struct _D3DXFRAME       *pFrameSibling;			// ���� ���� �ּ�
//struct _D3DXFRAME       *pFrameFirstChild;		// �ڽ� ���� �ּ�(���� ������ �ּҸ� �˸� ������ ������� ��ȸ�� �� �ְ� �� �����ϰ� ���� �������� �����ϴ� ������)
//
//// �޽� �����̳� ���� ���
//
//LPSTR                   Name;		// �̸�
//
//D3DXMESHDATA            MeshData;	// ��ǻ� �� ����ü �ϳ��� static �޽� ������ �ǹ���
//
//LPD3DXMATERIAL          pMaterials;	// �޽� ������ ���ϰ� �ִ� ��������
//LPD3DXEFFECTINSTANCE    pEffects;	// ������� ����
//DWORD                   NumMaterials;	// ����� ����
//DWORD                  *pAdjacency;		// �̿� ������ ���� ������
//
//LPD3DXSKININFO          pSkinInfo;		// ��Ű�� ������ �����ϱ� ���� ������
//
//struct _D3DXMESHCONTAINER *pNextMeshContainer;