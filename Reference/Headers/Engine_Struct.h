#pragma once

namespace Engine
{	
	typedef struct tagVertexColor
	{
		D3DXVECTOR3			vPos;
		DWORD				dwColor;

	}VTXCOL;

	const DWORD		VTXFVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

	typedef struct tagVertex_ViewPort
	{
		D3DXVECTOR4			vPosition;
		D3DXVECTOR2			vTexUV;
	}VTXVIEWPORT;

	const DWORD		VTXFVF_VIEWPORT = D3DFVF_XYZRHW | D3DFVF_TEX1;

	typedef struct tagVertexTexture
	{
		D3DXVECTOR3			vPos;
		D3DXVECTOR3			vNormal;
		D3DXVECTOR2			vTex;

	}VTXTEX;

	const DWORD		VTXFVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef struct tagVertexTextureTangent
	{
		D3DXVECTOR3			vPos;
		D3DXVECTOR3			vNormal;
		D3DXVECTOR2			vTex;
		D3DXVECTOR4			vTangent;

	}VTXTANGENT;

	const DWORD		VTXFVF_TANGENT = D3DFVF_XYZ | D3DFVF_NORMAL  | D3DFVF_TEX2;

	typedef	struct tagVertexCube
	{
		D3DXVECTOR3		vPos;
		D3DXVECTOR3		vTex;

	}VTXCUBE;

	const DWORD		VTXFVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

	typedef	struct	tagIndex16
	{
		WORD		_1, _2, _3;

	}INDEX16;

	typedef	struct	tagIndex32
	{
		DWORD		_1, _2, _3;

	}INDEX32;


	typedef struct tagInfo
	{
		D3DXVECTOR3		vPos;
		D3DXVECTOR3		vDir;
		D3DXVECTOR3		vLook;

		D3DXMATRIX		matWorld;

	}INFO;


	typedef struct tagCamera_Desc
	{
		D3DXVECTOR3			vEye; // ī�޶��� ��ġ.
		D3DXVECTOR3			vAt; // ī�޶� �ٶ󺸰� �ִ� ��.
		D3DXVECTOR3			vUp; // ����ϰ� �ִ� ��ǥ���������� ��(y)����
	}CAMERADESC;

	typedef struct tagProjection_Desc
	{
		float				fFovy; // �� ����ü�� y����
		float				fAspect; // wincx / wincy
		float				fNear;
		float				fFar;
	}PROJDESC;

	typedef	struct tagRay
	{
		D3DXVECTOR3		vRayDir;
		D3DXVECTOR3		vRayPos;
	}RAY;

	// �θ��� ��� �������� ���ԵǾ� �ִ� ����� �����ϱ� ���� ����ü

	typedef struct D3DXFRAME_DERIVED : public D3DXFRAME
	{
		_matrix				CombinedTransformMatrix;

	}D3DXFRAME_DERIVED;

	typedef struct tagMeshTextures
	{
		LPDIRECT3DTEXTURE9			pDiffuse;
		LPDIRECT3DTEXTURE9			pNormal;
		LPDIRECT3DTEXTURE9			pSpecular;
	}MESHTEXTURE;

	typedef struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER
	{
		MESHTEXTURE*			pTextures;

		LPD3DXMESH				pOriginMesh; // ������ ���¸� ������ �ִ� �޽�(�Һ�)

		_ulong					dwNumBones; // 

		_matrix*				pFrameOffsetMatrix;
		// ���� �迭�� �Ҵ��ؼ� �������� ����� �����ϴ� �뵵
		// �ִϸ��̼��� ������ ���·� �������� �� �� ������ ������� ������ �ִ� ��� ����

		_matrix**				ppFrameCombinedMatrix;
		// ������ ������ �ִ� CombinedTransformMatrix�� ��� ���ŵǱ� ������ �ƿ� CombinedTransformMatrix 
		// ��� �ּҸ� ���� �����ؼ� �Ź� ���ŵ� ��� ������ ���� ����

		_matrix*				pRenderingMatrix;

	}D3DXMESHCONTAINER_DERIVED;	

	typedef struct tagEffect
	{
		_float	fFadeIn;
		_float	fFadeOut;

		_vec3	vStart_Rotation;
		_vec3	vEnd_Rotation;

		_vec3	vStart_Position;
		_vec3	vEnd_Position;

		_vec3	vStart_Scale;
		_vec3	vEnd_Scale;

		_float	fStartTime;
		_float	fEndTime;


		_tchar	szAlphaTextureTag[128];
		_int	iAlpha_Width;
		_int	iAlpha_Height;
		_tchar	szBaseTextureTag[128];
		_int	iBase_Width;
		_int	iBase_Height;
		_tchar	szNoiseTextureTag[128];

		_int	iPassIndex;
		_float	fUSpeed;
		_float	fVSpeed;

		_matrix	matTransform;

		_float	fInit_Alpha;
		_float  fAlpha;
	}EFFECTDATA;

	typedef struct tagEFFECT_RECT
	{


	}EFFECTRECT_DATA;

	typedef struct tagEFFECT_MESH
	{
		_tchar	szMeshTag[128];
	}EFFECTMESH_DATA;

	typedef struct tagEffectParticle
	{
		_tchar	szTextureTag[128];

		_int	iParticleNum;
		//Particle Info

		
		_float fMax_Theta;
		_float fMin_Theta;
		_float fMin_Phi;
		_float fMax_Phi;
		_float fInit_Speed;
		_float fSpeed_Range;
		_float fInit_LifeTime;
		_float fLifeTime_Range;
		_float fFrame_Const;
		_vec3 vInital_Acceleration;
	}EFFECTPARTICLE_DATA;

	typedef	struct tagStats
	{
		_int	iCurrentHp;
		_int	iCurrentMp;

		_int	iMaxHp;
		_int	iMaxMp;

		_float	fMoveSpeed;
	}BASE_STATS;

	typedef struct tagDamage
	{
		_int	iDamage;
		_int	iDamage_Range;
	}DAMAGE;

	typedef struct tagTrailPosition
	{
		_vec3	vNearPosition;
		_vec3	vFarPosition;
	}TRAILPOS;
}
