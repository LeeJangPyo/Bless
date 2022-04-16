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
	//모든 뼈들을 순회하면서 가지고 있는 행렬 정보를 갱신하는 함수
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


//			D3DXFRAME		//설명
//LPSTR                   Name;					// 뼈의 이름(x파일에서 읽어온 뼈의 이름)
//D3DXMATRIX              TransformationMatrix;	// 뼈 하나의 최초 로드 상태에서의 행렬(ex: 뼈가 원점 기준으로 얼마만큼 이동해 있는가)
//
//LPD3DXMESHCONTAINER     pMeshContainer;			// 메쉬 컨테이를 구성하는 원소 메쉬(부위별 static 메쉬)의 정보를 저장하는 구조체 (주의 사항 : 모든 컨테이들의 메쉬의 뼈 정보를 다 갖고 있지는 않다)
//
//struct _D3DXFRAME       *pFrameSibling;			// 형제 뼈의 주소
//struct _D3DXFRAME       *pFrameFirstChild;		// 자식 뼈의 주소(시작 뼈대의 주소를 알면 나머지 뼈대들을 순회할 수 있게 끔 설계하고 싶은 이유에서 존재하는 포인터)
//
//// 메쉬 컨테이너 구성 요소
//
//LPSTR                   Name;		// 이름
//
//D3DXMESHDATA            MeshData;	// 사실상 이 구조체 하나가 static 메쉬 정보를 의미함
//
//LPD3DXMATERIAL          pMaterials;	// 메쉬 정점이 지니고 있는 재질정보
//LPD3DXEFFECTINSTANCE    pEffects;	// 사용하지 않음
//DWORD                   NumMaterials;	// 서브셋 개수
//DWORD                  *pAdjacency;		// 이웃 정점에 대한 포인터
//
//LPD3DXSKININFO          pSkinInfo;		// 스키닝 정보를 보관하기 위한 포인터
//
//struct _D3DXMESHCONTAINER *pNextMeshContainer;